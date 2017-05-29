'''
This is version 2.0 of Tissue algorithm, firts aproached by Sebastian Rivera 
in help with Emilio Chavez to be used in Latin American Robotics Competition 
Open Challenge, October 2016.
LARC 2.0 is being updated at the same time
'''

##--------------LIBRARIES--------------##

import cv2
import glob
import time
import math
import numpy as np
import LARC2 as rb
import random
import serial
import statistics
#import gtk
import subprocess
import getGoodSquares as sqr
from copy import deepcopy


##-----------GLOBAL VARIABLES-----------##




# mainFrame = cv2.imread("/Users/Emilio/Documents/Robotica_Todo/LARC_2017/LARC_2.0/old_stuff/MachineL/lastPhotos/80cm.jpg")
# mainFrame = cv2.resize(mainFrame,None,fx=0.50,fy=0.50,interpolation=cv2.INTER_AREA)


mainFrame = cv2.imread("./photos/cow1.jpg")
mainFrame = cv2.resize(mainFrame,None,fx=0.50,fy=0.50,interpolation=cv2.INTER_AREA)

#cap = cv2.VideoCapture(0)

# ret , mainFrame = cap.read()



def getCowXcenter(maxLenT):
	left,right,up=rb.calcCowLimits(maxLenT)
	print "cow x dist: ", right - left
	print "y: ", up
	drawLimits(left,right,up)
	return (left+right)/2

def drawLimits(left,right,y):
	font = cv2.FONT_HERSHEY_SIMPLEX
	cv2.line(mainFrame,(left,0),(left,480),(255,0,0),3)
	cv2.line(mainFrame,(right,0),(right,480),(255,0,0),3)
	cv2.line(mainFrame,(left,y),(right,y),(255,0,0),3)


def getDistance(x):
	return 0.2255*x + 61.474


def main():
	global mainFrame
	secondT = deepcopy(mainFrame)

	clearedImage = rb.clearImage(mainFrame)
	equalizedFrame = cv2.equalizeHist(clearedImage)
	# cv2.imshow('c',clearedImage)        
	# cv2.imshow('eq', equalizedFrame)
	cowFound, cowTissue, cowTissue2, allSquares = rb.isThereACow(equalizedFrame)

	mainFrame = rb.drawCowSquares(mainFrame,0,0,255,allSquares)
	secondT = rb.drawCowSquares(secondT,0,255,0,allSquares)
	
	getCowXcenter(cowTissue2)
	# cv2.putText(mainFrame,("diff L: " + str(left)),(30,20), font, 0.8,(0,0,255),1,cv2.LINE_AA)
	# cv2.putText(mainFrame,("diff R: " + str(640-right)),(30,50), font, 0.8,(0,0,255),1,cv2.LINE_AA)
	# cv2.putText(mainFrame,("diff Top: " + str(y)),(30,80), font, 0.8,(0,0,255),1,cv2.LINE_AA)

	mainFrame = rb.drawGreatestTissue(mainFrame,cowTissue)
	secondT = rb.drawGreatestTissue(secondT,cowTissue2)

	if cowFound:
		print "Cow Found"
		go,lL,lR,lT,theta = rb.isCowMilkeable(cowTissue,allSquares)
		print "top :", lT
		y = getDistance(lT)
		print "DISTANCIA A LA VACA : " ,y
		if go:
			print "Cow can be milked"
		else:
			print "Cow cannot be milked"
		# Tissue 1
		A,B, theta = rb.ajusteDeCurvas(cowTissue)
		# mainFrame = rb.drawSlope(mainFrame,A,B)
		mainFrame = rb.drawLimits(mainFrame,lL,lR,lT)
		# Tissue 2
		A,B, theta = rb.ajusteDeCurvas(cowTissue2)
		# secondT = rb.drawSlope(secondT,A,B)
		secondT = rb.drawLimits(secondT,lL,lR,lT)



		cv2.imshow('tissue1',mainFrame)
		cv2.imshow('tissue2',secondT)
		cv2.waitKey(0)

	else:
		print "Cow not found"

	
##--------------------------------------##
##-----------	RUNNIGING MAIN ---------##
##--------------------------------------##
main()
print "DONE"
cv2.destroyAllWindows()























