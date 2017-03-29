import cv2

import numpy as np
import Larc_vision as rb

from copy import deepcopy
##-------------------------------##

##-----------GLOBAL VARIABLES-----------##
binValue = 70
headingWall = "E"	# GLOBAL DIRECTION VARIABLE
mainFrame = []	# Initialize global variable for image
lastTurn = "left"
firstWalk = "225"
##--------------------------------------##

##-----------SETUP-----------##
cap = cv2.VideoCapture(0)


def isThereACow():
	global mainFrame
	global binValue
	maxLenT = [] # maximumLenghtTissue
	allSquares = [] # Store, in each iteration of the binarization, the squares found in the image
	minNumSquares = 4
	# mainFrame = cv2.imread('/home/pi/pruebasVision/FotosVaca/img25.jpg') 
	gF = True
	gF = takePicture() # returns boolean to know if the picture is OK
	if gF:
		filteredFrame = rb.clearImage(mainFrame)	# Clear the image with a GaussianBlur
		equalizedFrame = cv2.equalizeHist(filteredFrame)
		
		# FOR: search for the best threshold value
		for binValueT in range(3,131,3):
			cp0 = cp1 = cp2 = deepcopy(equalizedFrame)
			main_copy2=mainFrame.copy()

			thresFrame0 = rb.doThresHold(cp0, binValueT,7,1) 
			contours0 = rb.findContours(thresFrame0) 
			cowRectangles0,_ = rb.getGoodSquares(contours0,mainFrame,thresFrame0,main_copy2) 
			findEquals(allSquares,cowRectangles0,15)

			thresFrame1 = rb.doThresHold(cp1, binValueT,3,3) # Thresholds the image and erodes it
			contours1 = rb.findContours(thresFrame1) # Finds all the contours inside the image
			cowRectangles1,_ = rb.getGoodSquares(contours1,mainFrame,thresFrame1,main_copy2) # From contours, extract possile cow squares
			findEquals(allSquares,cowRectangles1,15)

			thresFrame2 = rb.doThresHold(cp2, binValueT,5,2) 
			contours2 = rb.findContours(thresFrame2) 
			cowRectangles2,_ = rb.getGoodSquares(contours2,mainFrame,thresFrame2,main_copy2) 
			findEquals(allSquares,cowRectangles2,15)

			del cp0
			del cp1
			del cp2


			print "im done with thres"


			#-------END FOR------
			# for c in allSquares:
			# 	cv2.rectangle(mainFrame,(c.getX(),c.getY()),(c.getX()+c.getW(),c.getY()+c.getH()),(255,255,255),4)
			# When there are more than 'minNumSquares', it can be found at least one tissue
			if len(allSquares) > minNumSquares:
				tempAllSquares = deepcopy(allSquares)
				maxLenT = rb.makeTissue(tempAllSquares,[],10,0,[0,0],0)

				for sqr in maxLenT:
					cv2.rectangle(main_copy2, (sqr.getTopLeftC()[0],sqr.getTopLeftC()[1]), (sqr.getBotRightC()[0],sqr.getBotRightC()[1]), (127,50,127), 2)
					cv2.imshow("squares of " + str(binValueT),main_copy2)
					cv2.waitKey(50)
				# drawGreatestTissue(maxLenT)
				# cv2.imshow("m",mainFrame)
				# cv2.waitKey(5000)
				# cv2.destroyAllWindows()
				if len(maxLenT) > minNumSquares:
					return True,maxLenT,allSquares
	return False,[],[]

def takePicture():
	global mainFrame
	for i in range(4):
		cap.grab()
	goodFrm, mainFrame = cap.read()
	print "I took a pic"
	return goodFrm

def findEquals(allSqrs,partial,epsilon):
	while len(partial) > 0:
		testSqr = partial.pop(0)
		found = False
		for compSqr in allSqrs:
			if (distanceBCorners(compSqr.getTopLeftC(),testSqr.getTopLeftC()) < epsilon or distanceBCorners(compSqr.getTopRightC(),testSqr.getTopRightC()) < epsilon or distanceBCorners(compSqr.getBotLeftC(),testSqr.getBotLeftC()) < epsilon or distanceBCorners(compSqr.getBotRightC(),testSqr.getBotRightC()) < epsilon) and not found:
				found = True 
				if testSqr.getArea() > compSqr.getArea():
					compSqr.x = testSqr.x
					compSqr.y = testSqr.y
					compSqr.w = testSqr.w
					compSqr.h = testSqr.h
					compSqr.area = testSqr.area
					compSqr.level = testSqr.level
					compSqr.topLeftC = testSqr.topLeftC
					compSqr.topRightC = testSqr.topRightC
					compSqr.botLeftC = testSqr.botLeftC
					compSqr.botRightC = testSqr.botRightC
				
		if not found:
			allSqrs.append(testSqr)

def distanceBCorners(c1,c2):
	x1 = c1[0]
	x2 = c2[0]
	y1 = c1[1]
	y2 = c2[1]
	return rb.distance(x1,y1,x2,y2)

validation=False

validation,_,_=isThereACow()
print validation
cv2.waitKey(0)