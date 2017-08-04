# standar libs
import cv2
import numpy as np
import serial
import time
from copy import deepcopy

# Roborregos libs
import sys
sys.path.insert(0, '../lib/')
import Larc_vision_2017 as rb
import communication as com

# CODE


mainFrame = []
clearedMainFrame = []

cap = cv2.VideoCapture(0)
# let camara calibrate light
for i in range(10):
        cap.read()


def takePicture():
	global mainFrame
	global clearedMainFrame
	# clear internal buffer
	for i in range(4):
		cap.grab()
	# get new image
	goodFrm, mainFrame = cap.read()
	print "I took a pic"
	if goodFrm:
		clearedMainFrame = rb.clearImage(mainFrame)

	else:
		print ("There is an error with the camera")
	return goodFrm

def getCowXcenter(left,right):
        center = (left+right)/2
        cv2.line(mainFrame,(center,0),(center,480),(0,255,255),1)
        return center

def getXCenterFrame():
        center = (mainFrame.shape[1])/2
        cv2.line(mainFrame,(center,0),(center,480),(255,255,0),1)
        return center

def getLimits(maxLenT):
        left,right,up=rb.calcCowLimits(maxLenT)
        drawLimits(left,right,up)
        return left,right,up

def drawLimits(left,right,y):
        global mainFrame
        font = cv2.FONT_HERSHEY_SIMPLEX
        cv2.line(mainFrame,(left,0),(left,480),(255,0,0),3)
        cv2.line(mainFrame,(right,0),(right,480),(255,0,0),3)
        cv2.line(mainFrame,(0,y),(640,y),(255,0,0),3)
        # cv2.putText(mainFrame,("diff L: " + str(left)),(30,20), font, 0.8,(0,0,255),1,cv2.LINE_AA)
        # cv2.putText(mainFrame,("diff R: " + str(640-right)),(30,50), font, 0.8,(0,0,255),1,cv2.LINE_AA)
        # cv2.putText(mainFrame,("diff Top: " + str(y)),(30,80), font, 0.8,(0,0,255),1,cv2.LINE_AA)
def paralelism(maxLenTissue):
        global mainFrame
        tLevel = rb.getTissueTopLevel(maxLenTissue)
        A,B,theta = rb.ajusteDeCurvas(tLevel)
        rb.drawSlope(mainFrame,A,B)
        degrees = int(abs(theta))
        print degrees
        print A
'''
    MAIN
'''
if __name__ == "__main__":

    while True:

        analyse = raw_input("process photo? ")
        if analyse == '1':
                takePicture()
                filteredImage = rb.clearImage(mainFrame)
                this_time = time.time()
                validation,maxLenT,_ = rb.isThereACow(mainFrame,filteredImage)
                if validation:
                        paralelism(maxLenT)
                cv2.imshow('im',mainFrame)
                print validation, len(maxLenT)
                print "center camera: ",(mainFrame.shape[1])/2

                
                k = cv2.waitKey(0)
                if k ==27:
                        break
                cv2.destroyAllWindows()

        # navegation instructions
        mov = raw_input("type instruction ")

        if mov == 'f':
                cm = input("cm ")
                com.forwardNCm(cm)
        elif mov == 'b':
                cm = input("cm ")
                com.backwardNCm(cm)
        elif mov == "tr":
                dg = input("dgs ")
                com.turnNDegrees(dg,0)
        elif mov == "tl":
                dg = input("dgs ")
                com.turnNDegrees(dg,1)
        elif mov == "t":
	                com.goGrabTerrine()
    cap.release()
    cv2.destroyAllWindows()

