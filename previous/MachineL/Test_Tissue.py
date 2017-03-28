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



# mainFrame = cv2.imread("/Users/Emilio/Documents/Robotica_Todo/Tissue_2.0/MachineL/lastPhotos/80cm.jpg")
# mainFrame = cv2.resize(mainFrame,None,fx=0.20,fy=0.20,interpolation=cv2.INTER_AREA)
cap = cv2.VideoCapture(0)

ret , mainFrame = cap.read()



def getDistance(x):
	return 0.2255*x + 61.474


def main():
	global mainFrame
	secondT = deepcopy(mainFrame)

	clearedImage = rb.clearImage(mainFrame)
	equalizedFrame = cv2.equalizeHist(clearedImage)
	# #cv2.imshow('c',clearedImage)        
	#cv2.imshow('eq', equalizedFrame)
	cowFound, cowTissue, cowTissue2, allSquares = rb.isThereACow(equalizedFrame)

	mainFrame = rb.drawCowSquares(mainFrame,0,0,255,allSquares)
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
		# mainFrame = rb.drawLimits(mainFrame,lL,lR,lT)
		# Tissue 2
		A,B, theta = rb.ajusteDeCurvas(cowTissue2)
		# secondT = rb.drawSlope(secondT,A,B)
		# secondT = rb.drawLimits(secondT,lL,lR,lT)



		cv2.imshow('tissue1',mainFrame)
		cv2.imshow('tissue2',secondT)
		cv2.waitKey(0)

	else:
		print "Cow not found"

	
##-------------------------------------##
##---------RUNNIGING MAIN LOOP---------##
##-------------------------------------##
main()
print "DONE"
cv2.destroyAllWindows()






























