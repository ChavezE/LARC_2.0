# standar libs
import cv2
import numpy as np
import serial
import time
import glob
from copy import deepcopy

# Roborregos libs
import sys
sys.path.insert(0, '../lib/')
import Larc_vision_2017 as rb


# CODE


mainFrame = []
clearedMainFrame = []



if __name__ == "__main__":

    for path in glob.glob("../TestPhotos/480-640/*.jpg"): #13, 19
        try:
            mainFrame = cv2.imread(path)

            
            filteredImage = rb.clearImage(mainFrame)
            this_time = time.time()
            validation2, filtered = rb.detectCow(filteredImage)
            validation,maxLenT,_ = rb.isThereACow(mainFrame,filtered)
            print "HAAR: ",validation2
            print "ALGORITHM: ",validation, len(maxLenT)
            
            if validation:
                print "COW FOUND"

                tLevel = rb.getTissueTopLevel(maxLenT)
                # cv2.line(mainFrame,(0,int(mean)),(640,int(mean)),(0,0,255),5)
                # cv2.line(mainFrame,(0,int(mean+std)),(640,int(mean+std)),(0,255,0),3)
                # cv2.line(mainFrame,(0,int(mean-std)),(640,int(mean-std)),(0,255,0),3)
                rb.drawCowSquares(mainFrame,200,50,200,tLevel)
                A,B,theta = rb.ajusteDeCurvas(tLevel)
                rb.drawSlope(mainFrame,A,B)
                left,right,up=rb.calcCowLimits(maxLenT)
                rb.drawLimits(mainFrame,left,right,up)
                # for sqr in maxLenT:
                #         print sqr.getX(), sqr.getY()

            cv2.imshow('im',mainFrame)
            
            k = cv2.waitKey(0)
            if k ==27:
                    break
        except:
            print "there was an error"            
        cv2.destroyAllWindows()


        
    cv2.destroyAllWindows()

