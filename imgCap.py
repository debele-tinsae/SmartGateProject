import socket               # Import socket module
import picamera
import time
import os.path
sigCap = "1"
import serial
import socket, sys, time

ser = serial.Serial('/dev/ttyACM0',9600)
#ser2 = serial.Serial('/dev/ttyACM0',9600)

while True:
    s = ser.read()
    s = s.decode()
    if s == sigCap:
        try:
            
            formats = ['png']
            camera = picamera.PiCamera()
            print('Capture form')
            camera.start_preview()
            camera.capture('img/imgToSent.png',format = formats[0])
            camera.stop_preview()
        finally:
            camera.close()  
        #time.sleep(5)
        s = socket.socket()
        # Create a socket object
        host = '10.0.0.10' # Get local machine name
        port = 1010                # Reserve a port for your service.
        
        s.connect((host, port))
        f = open('img/imgToSent.png','rb')
        print ('Sending...')
        l = f.read(1024)
        while (l):
            s.send(l)
            l = f.read(1024)
        f.close()
        print("Done Sending")
        s.shutdown(socket.SHUT_WR)
        print(s.recv(1024))
        s.close()                     # Close the socket when done
        #os.remove('img/imgToSent.png')
        
        
       #=================================== 
        textport = 1080
        t = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        port2 = int(textport)
        server_address = ('', port2)
        t.bind(server_address)

        
        print ("Waiting to receive on port %d :" % port)

        buf, address = t.recvfrom(port)
        if not len(buf):
            break
        print("\n-------------------------------------------\n")    
        print ("Received  licPlate Number = %s : " % (buf))
        print("\n-------------------------------------------\n")
        print(buf)
        
        if buf == 'accessGranted':
            sentTo = '1'
            print('I am in')
            ser.write(sentTo.encode())
        elif buf == 'accessdenied':
            sentTo ='0'
            ser.write(sentTo.encode())
            print(buf)
        t.close()
        time.sleep(5)

