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

mainFrame = cv2.imread('image9.jpg')


def main():
	global mainFrame

	clearedImage = rb.clearImage(mainFrame)        
	equalizedFrame = cv2.equalizeHist(clearedImage)

	cowFound, cowTissue, allSquares = rb.isThereACow(equalizedFrame)
	mainFrame = rb.drawGreatestTissue(mainFrame,cowTissue)
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
		cv2.waitKey(0)

	
##-------------------------------------##
##---------RUNNIGING MAIN LOOP---------##
##-------------------------------------##
main()
print "DONE"
cv2.destroyAllWindows()






























