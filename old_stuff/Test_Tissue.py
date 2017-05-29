'''
This is version 2.0 of Tissue algorithm, firts aproached by Sebastian Rivera 
in help with Emilio Chavez to be used in Latin American Robotics Competition 
Open Challenge, October 2016.
LARC 2.0 is being updated at the same time
'''

##--------------LIBRARIES--------------##

import cv2
import time
import math
import numpy as np
import LARC2 as rb
import random
import serial
import statistics
#import gtk
import subprocess
from copy import deepcopy


##-----------GLOBAL VARIABLES-----------##

mainFrame = cv2.imread("/photos/image1.jpg")
mainFrame = cv2.resize(mainFrame,None,fx=0.20,fy=0.20,interpolation=cv2.INTER_AREA)


def main():
	global mainFrame

	clearedImage = rb.clearImage(mainFrame)
	#cv2.imshow('c',clearedImage)        
	equalizedFrame = cv2.equalizeHist(clearedImage)
	#cv2.imshow('eq', equalizedFrame)
	cowFound, cowTissue, allSquares = rb.isThereACow(equalizedFrame)
	mainFrame = rb.drawCowSquares(mainFrame,0,0,255,allSquares)
	mainFrame = rb.drawGreatestTissue(mainFrame,cowTissue)
	#cv2.imshow('2',mainFrame)

	if cowFound:
		print "Cow Found"
		go,lL,lR,lT,theta = rb.isCowMilkeable(cowTissue,allSquares)
		if go:
			print "Cow can be milked"
		else:
			print "Cow cannot be milked"
		A,B, theta = rb.ajusteDeCurvas(cowTissue)
		mainFrame = rb.drawSlope(mainFrame,A,B)
		mainFrame = rb.drawLimits(mainFrame,lL,lR,lT)

		cv2.imshow('img',mainFrame)

	else:
		print "Cow not found"

	cv2.waitKey(0)
##-------------------------------------##
##---------RUNNIGING MAIN LOOP---------##
##-------------------------------------##
main()
print "DONE"
cv2.destroyAllWindows()






























