

import os
import PossiblePlate
import cv2
import numpy as np
import math
import random
import os.path
import time
import DetectChars
import PossiblePlate
import PossibleChar
import socket, sys, time
#this code is based on udemy @ Jose Portilla and  and youtube @ chris Dahms 
#I edited and modife the code in order to meet our goal 
PLATE_WIDTH_PADDING_FACTOR = 1.3
PLATE_HEIGHT_PADDING_FACTOR = 1.5

SCALAR_BLACK = (0.0, 0.0, 0.0)
SCALAR_WHITE = (255.0, 255.0, 255.0)
SCALAR_YELLOW = (0.0, 255.0, 255.0)
SCALAR_GREEN = (0.0, 255.0, 0.0)
SCALAR_RED = (0.0, 0.0, 255.0)

GAUSSIAN_SMOOTH_FILTER_SIZE = (5, 5)
ADAPTIVE_THRESH_BLOCK_SIZE = 19
ADAPTIVE_THRESH_WEIGHT = 9

recLicPlate = [] 
def preprocess(imgOriginal):
    imgGrayscale = extractValue(imgOriginal)
    imgMaxContrastGrayscale = maximizeContrast(imgGrayscale)
    height, width = imgGrayscale.shape
    imgBlurred = np.zeros((height, width, 1), np.uint8)
    imgBlurred = cv2.GaussianBlur(imgMaxContrastGrayscale, GAUSSIAN_SMOOTH_FILTER_SIZE, 0)
    imgThresh = cv2.adaptiveThreshold(imgBlurred, 255.0, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, ADAPTIVE_THRESH_BLOCK_SIZE, ADAPTIVE_THRESH_WEIGHT)
    return imgGrayscale, imgThresh
# end function
def extractValue(imgOriginal):
    height, width, numChannels = imgOriginal.shape
    imgHSV = np.zeros((height, width, 3), np.uint8)
    imgHSV = cv2.cvtColor(imgOriginal, cv2.COLOR_BGR2HSV)
    imgHue, imgSaturation, imgValue = cv2.split(imgHSV)
    return imgValue
# end function
def maximizeContrast(imgGrayscale):
    height, width = imgGrayscale.shape
    imgTopHat = np.zeros((height, width, 1), np.uint8)
    imgBlackHat = np.zeros((height, width, 1), np.uint8)
    structuringElement = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
    imgTopHat = cv2.morphologyEx(imgGrayscale, cv2.MORPH_TOPHAT, structuringElement)
    imgBlackHat = cv2.morphologyEx(imgGrayscale, cv2.MORPH_BLACKHAT, structuringElement)
    imgGrayscalePlusTopHat = cv2.add(imgGrayscale, imgTopHat)
    imgGrayscalePlusTopHatMinusBlackHat = cv2.subtract(imgGrayscalePlusTopHat, imgBlackHat)
    return imgGrayscalePlusTopHatMinusBlackHat
# end function
def findPossibleCharsInScene(imgThresh):
    listOfPossibleChars = []                # this will be the return value
    intCountOfPossibleChars = 0
    imgThreshCopy = imgThresh.copy()
    imgContours, contours, npaHierarchy = cv2.findContours(imgThreshCopy, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)   # find all contours
    height, width = imgThresh.shape
    imgContours = np.zeros((height, width, 3), np.uint8)

    for i in range(0, len(contours)):                       # for each contour
        possibleChar = PossibleChar.PossibleChar(contours[i])
        if DetectChars.checkIfPossibleChar(possibleChar):                   # if contour is a possible char, note this does not compare to other chars (yet) . . .
            intCountOfPossibleChars = intCountOfPossibleChars + 1           # increment count of possible chars
            listOfPossibleChars.append(possibleChar)                        # and add to list of possible chars
    return listOfPossibleChars
# end function

def extractPlate(imgOriginal, listOfMatchingChars):
    possiblePlate = PossiblePlate.PossiblePlate()           # this will be the return value
    listOfMatchingChars.sort(key = lambda matchingChar: matchingChar.intCenterX)        # sort chars from left to right based on x position
            # calculate the center point of the plate
    fltPlateCenterX = (listOfMatchingChars[0].intCenterX + listOfMatchingChars[len(listOfMatchingChars) - 1].intCenterX) / 2.0
    fltPlateCenterY = (listOfMatchingChars[0].intCenterY + listOfMatchingChars[len(listOfMatchingChars) - 1].intCenterY) / 2.0
    ptPlateCenter = fltPlateCenterX, fltPlateCenterY
            # calculate plate width and height
    intPlateWidth = int((listOfMatchingChars[len(listOfMatchingChars) - 1].intBoundingRectX + listOfMatchingChars[len(listOfMatchingChars) - 1].intBoundingRectWidth - listOfMatchingChars[0].intBoundingRectX) * PLATE_WIDTH_PADDING_FACTOR)
    intTotalOfCharHeights = 0
    for matchingChar in listOfMatchingChars:
        intTotalOfCharHeights = intTotalOfCharHeights + matchingChar.intBoundingRectHeight
    # end for
    fltAverageCharHeight = intTotalOfCharHeights / len(listOfMatchingChars)
    intPlateHeight = int(fltAverageCharHeight * PLATE_HEIGHT_PADDING_FACTOR)
            # calculate correction angle of plate region
    fltOpposite = listOfMatchingChars[len(listOfMatchingChars) - 1].intCenterY - listOfMatchingChars[0].intCenterY
    fltHypotenuse = DetectChars.distanceBetweenChars(listOfMatchingChars[0], listOfMatchingChars[len(listOfMatchingChars) - 1])
    fltCorrectionAngleInRad = math.asin(fltOpposite / fltHypotenuse)
    fltCorrectionAngleInDeg = fltCorrectionAngleInRad * (180.0 / math.pi)
            # pack plate region center point, width and height, and correction angle into rotated rect member variable of plate
    possiblePlate.rrLocationOfPlateInScene = ( tuple(ptPlateCenter), (intPlateWidth, intPlateHeight), fltCorrectionAngleInDeg )
            # final steps are to perform the actual rotation
            # get the rotation matrix for our calculated correction angle
    rotationMatrix = cv2.getRotationMatrix2D(tuple(ptPlateCenter), fltCorrectionAngleInDeg, 1.0)
    height, width, numChannels = imgOriginal.shape      # unpack original image width and height
    imgRotated = cv2.warpAffine(imgOriginal, rotationMatrix, (width, height))       # rotate the entire image
    imgCropped = cv2.getRectSubPix(imgRotated, (intPlateWidth, intPlateHeight), tuple(ptPlateCenter))
    possiblePlate.imgPlate = imgCropped         # copy the cropped plate image into the applicable member variable of the possible plate

    return possiblePlate
# end function
def getLicPlate():
    return recLicPlate.pop()
def detectPlatesInScene(imgOriginalScene):
    listOfPossiblePlates = []                   # this will be the return value
    height, width, numChannels = imgOriginalScene.shape
    imgGrayscaleScene = np.zeros((height, width, 1), np.uint8)
    imgThreshScene = np.zeros((height, width, 1), np.uint8)
    imgContours = np.zeros((height, width, 3), np.uint8)
    cv2.destroyAllWindows() 
    imgGrayscaleScene, imgThreshScene = preprocess(imgOriginalScene)         
    listOfPossibleCharsInScene = findPossibleCharsInScene(imgThreshScene)
    listOfListsOfMatchingCharsInScene = DetectChars.findListOfListsOfMatchingChars(listOfPossibleCharsInScene)

    for listOfMatchingChars in listOfListsOfMatchingCharsInScene:                   # for each group of matching chars
        possiblePlate = extractPlate(imgOriginalScene, listOfMatchingChars)         # attempt to extract plate

        if possiblePlate.imgPlate is not None:                          # if plate was found
            listOfPossiblePlates.append(possiblePlate)                  # add to list of possible plates
        # end if
    # end for

    #print("\n" + str(len(listOfPossiblePlates)) + " possible plates found")  # 13 with MCLRNF1 image

    return listOfPossiblePlates
# end function
def main():
    
    blnKNNTrainingSuccessful = DetectChars.loadKNNDataAndTrainKNN()         
    if blnKNNTrainingSuccessful == False:                             
        print("\nerror: KNN traning was not successful\n") 
        return       
    #rec.imgReceiver()                                                   
    #plateNum = str(input("Please input images Number"))

    s = socket.socket()         # Create a socket object
    host = ''                   # Get local machine name
    port = 1010                 # Reserve a port for your service.
    s.bind((host, port))        # Bind to the port
    f = open('LicPlateImages/recImg.png','wb')
    s.listen(5)                 # Now wait for client connection.
    while True:
        c, addr = s.accept()     # Establish connection with client.
        print ('Got connection from', addr)
        print("Receiving...")
        l = c.recv(1024)
        while (l):
            #print ("Receiving...")
            f.write(l) 
            l = c.recv(1024)
        f.close()
        print("Data Received")
        #c.send('Data Received')
        c.close()
        s.close()
        break 
    #--------------------------------
    file_path = "LicPlateImages/recImg.png" #"LicPlateImages/" + plateNum + ".png"
    while not os.path.exists(file_path):
        time.sleep(5)
    imgOriginalScene  = cv2.imread(file_path)             # mothod us for  to read the imread()   
    if imgOriginalScene is None:                           
        print("\nerror: image not read from file \n\n")  
        os.remove('LicPlateImages/recImg.png')
        print("Please send Picture again\n")
        main()
        #os.system("pause")                                  
        #return                                              
    listOfPossiblePlates =detectPlatesInScene(imgOriginalScene)          # method for detectPlatesInScene() 
    listOfPossiblePlates = DetectChars.detectCharsInPlates(listOfPossiblePlates)       #method for detectCharsInPlates() 
    cv2.imshow("imgOriginalScene", imgOriginalScene)            
    if len(listOfPossiblePlates) == 0:                          
        print("\nNo license plates were detected\n")
        host = '10.0.0.25'
        textport = 1006
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        port = int(textport)
        server_address = (host, port)
        try:
            data = 'No license plates were detected'
            if not len(data):
                print("waiting for correct answere")
            s.sendto(data.encode('utf-8'), server_address)
        finally:
            s.close()
        main() 
    else:                                                       
        listOfPossiblePlates.sort(key = lambda possiblePlate: len(possiblePlate.strChars), reverse = True)
        licPlate = listOfPossiblePlates[0]                                                               #method for listing all plates listPossiblePlate()
        #cv2.imshow("imgPlate", licPlate.imgPlate)         
        #cv2.imshow("imgThresh", licPlate.imgThresh)
        if len(licPlate.strChars) == 0:                     
            print("\nNo characters were detected\n\n") 
            os.remove('LicPlateImages/recImg.png')
            main()                                         
        # end if
        print("----------------------------------------")
        print("\nlicense plate read from image = " + licPlate.strChars + "\n")  # write license plate text to std out
        recLicPlate.append(licPlate.strChars)
        print("----------------------------------------")
        
        #cv2.imshow("imgOriginalScene", imgOriginalScene)               
        cv2.imwrite("imgOriginalScene.png", imgOriginalScene)           # write image out to file
   
    
        host = '10.0.0.25'
        textport = 1006
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        port = int(textport)
        server_address = (host, port)
        try:
            data = licPlate.strChars
            if not len(data):
                print("waiting for correct answere")
            s.sendto(data.encode('utf-8'), server_address)
        finally:
            s.close()
    os.remove('LicPlateImages/recImg.png')
    main()			
    return

if __name__ == "__main__":
    main()













