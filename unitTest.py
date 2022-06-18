import unittest
import index
import flagStore

class testAll(unittest.TestCase):
    '''

    '''
    def testImgCaptured(self):          # this one test the images is caputered and recived by image recognition 
        self.assertAlmostEqual(index.getFlag(),'Captured')
    def testSentInfo(self):
        #mock_recog = Mock()
        #index.sendLicPlate((mock_recog))
        #print(index.mock_calls)
        self.assertEqual(index.main(),'U8NTBAD')    # test the image recognition 
    def testRecProData(self):
        self.assertAlmostEqual(index.getFlag(), 'Processed')  # test the processed image to star and sent 
    ''' This testing will test all of the reciving and sending part of the program at once and also test the image recoginstion aswell
    '''
if __name__ == '__main__':
    unittest.main()