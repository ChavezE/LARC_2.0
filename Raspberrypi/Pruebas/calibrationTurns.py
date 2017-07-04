

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
cap = cv2.VideoCapture(0)
# let camara calibrate light
for i in range(10):
	cap.read()


def takePicture():
	global mainFrame
	# mainFrame=cv2.imread('test_photos/'+str(frameNumber)+'.jpg')
	# mainFrame=cv2.pyrDown(mainFrame)
	for i in range(4):
		cap.grab()

	goodFrm, mainFrame = cap.read()
	print "I took a pic"

def colorFilterCalibration():
	imagen = mainFrame.copy()
	hsv = cv2.cvtColor(imagen, cv2.COLOR_BGR2HSV)

	bajos = np.array([5,100,100], dtype=np.uint8)
	altos = np.array([20, 255, 255], dtype=np.uint8)

	mask = cv2.inRange(hsv, bajos, altos)
	 
	moments = cv2.moments(mask)
	area = moments['m00']
	x = 0

	print area
	if(area > 10):

		x = int(moments['m10']/moments['m00'])
		y = int(moments['m01']/moments['m00'])
		
		cv2.rectangle(imagen, (x, y), (x+2, y+2),(0,200,255), 2)
		cv2.line(mainFrame,(x,0),(x,480),(0,200,0),1)

	#cv2.imshow('mask', mask)

	return imagen, x

def getCowXcenter(left,right):
	center = (left+right)/2
	cv2.line(mainFrame,(center,0),(center,480),(0,255,255),1)
	return center

def getXCenterFrame():
	center = (mainFrame.shape[1])/2
	cv2.line(mainFrame,(center,0),(center,480),(0,0,255),2)
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

'''
    MAIN
'''
if __name__ == "__main__":

	while True:

		takePicture()	
		frame, x =  colorFilterCalibration()
		center = getXCenterFrame()
		cv2.imshow("Allignment", mainFrame)
		print ("pixels:  ", center - x)
		cv2.waitKey(0)
		

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

cv2.destroyAllWindows()
