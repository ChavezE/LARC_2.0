# ------ LIBRARIES ----------
import cv2
import time
import math
import numpy as np
import random
import LARC1 as rb
# ---------------------------

filename = 'cow1.jpg'

cap = cv2.VideoCapture(0)


# loads image as imgOriginal
# This will have to change to taking snap everytime needed
# in order to do this we must initialice cv2.videoCapture in the future
# and remove parameter filename
def loadImage(capName):
    return  cv2.imread(capName)

imgOriginal = loadImage(filename)
imgOriginal = cv2.resize(imgOriginal,None,fx=0.20,fy=0.20,interpolation=cv2.INTER_AREA)

def nothing(x):
    pass

cv2.namedWindow('image')
# create trackbars for color change
cv2.createTrackbar('Thres','image',0,255,nothing)
# create switch for ON/OFF functionality
switch = '0 : OFF \n1 : ON'
cv2.createTrackbar(switch, 'image',0,1,nothing)
ret = 1
def goLive():
    global imgOriginal
    ret = 1
    while (1 and ret ):
    
        ret , imgOriginal = cap.read()
        #imgOriginal = cv2.resize(imgOriginal,None,fx=0.5,fy=0.5,interpolation=cv2.INTER_AREA)
        cv2.imshow('imgOriginal',imgOriginal)
        filteredImage = rb.clearImage(imgOriginal)

        k = cv2.waitKey(10) & 0xFF
        if k == 27:
            break

        # get current positions of four trackbars
        binValue = cv2.getTrackbarPos('Thres','image')
        s = cv2.getTrackbarPos(switch,'image')


        if s == 0:
            pass 
        else:
            thresImage = rb.doThresHold(filteredImage,binValue,3,1)
            #cv2.resize(thresImage,None,fx=0.5,fy=0.5,interpolation=cv2.INTER_AREA)
            cv2.imshow('img', thresImage)
        cv2.waitKey(100)
goLive()
cv2.destroyAllWindows()
