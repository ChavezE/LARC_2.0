import cv2

import numpy as np
import Larc_vision_2016 as rb

from copy import deepcopy

##-------------------------------##

##-----------GLOBAL VARIABLES-----------##

mainFrame = []	# Initialize global variable for image

##--------------------------------------##

##-----------SETUP-----------##
cap = cv2.VideoCapture(0)


def isThereACow():
	global mainFrame
	global binValue
	maxLenT = [] # maximumLenghtTissue
	allSquares = [] # Store, in each iteration of the binarization, the squares found in the image
	minNumSquares = 4
	gF = True
	gF = takePicture() # returns boolean to know if the picture is OK
	if gF:
		filteredFrame = rb.clearImage(mainFrame)	# Clear the image with a GaussianBlur
		equalizedFrame = cv2.equalizeHist(filteredFrame)
		
		# FOR: search for the best threshold value
		for binValueT in range(3,131,3):
			cp0 = cp1 = cp2 = deepcopy(equalizedFrame)

			thresFrame0 = rb.doThresHold(cp0, binValueT,7,1) 
			contours0 = rb.findContours(thresFrame0) 
			cowRectangles0 = rb.getGoodSquares(contours0,thresFrame0) 
			findEquals(allSquares,cowRectangles0,15)

			thresFrame1 = rb.doThresHold(cp1, binValueT,3,3) # Thresholds the image and erodes it
			contours1 = rb.findContours(thresFrame1) # Finds all the contours inside the image
			cowRectangles1 = rb.getGoodSquares(contours1,thresFrame1) # From contours, extract possile cow squares
			findEquals(allSquares,cowRectangles1,15)

			thresFrame2 = rb.doThresHold(cp2, binValueT,5,2) 
			contours2 = rb.findContours(thresFrame2) 
			cowRectangles2 = rb.getGoodSquares(contours2,thresFrame2) 
			findEquals(allSquares,cowRectangles2,15)

			del cp0
			del cp1
			del cp2


			print "im done with thres"


			
			if len(allSquares) > minNumSquares:
				tempAllSquares = deepcopy(allSquares)
				maxLenT = rb.doTissue(tempAllSquares)
				print "THIS"
			
				if len(maxLenT) > minNumSquares:
					print "THIS ONE"
					left,right,_=rb.calcCowLimits(maxLenT)
					if(right - left) > (mainFrame.shape[1]*.10):
						print "THIS OTHER ONE"
						return True,maxLenT,allSquares
	return False,[],[]

def takePicture():
	global mainFrame
	# mainFrame=cv2.imread('TMR2017/'+str(frameNumber)+'.jpg')
	# cv2.imshow("frame: " + str(frameNumber),mainFrame)
	# cv2.waitKey(0)
	for i in range(10):
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

def getCowXCenter(maxLenT):
	left,right,_=rb.calcCowLimits(maxLenT)
	return (left+right)/2

def getXCenterFrame():
	return (mainFrame.shape[1])/2

def checkingTurningR():
	turnRight(50)
	for x in range(1,3):
		turnRight(15)
		missingAngles=((3-x)*15)+75
		foundCow,_,_ = isThereACow()
		if foundCow:
			break
	turnRight(missingAngles)
	return foundCow

def checkingTurningL():
	turnLeft(50)
	for x in range(1,3):
		turnLeft(15)
		missingAngles=((3-x)*15)+75
		foundCow,_,_ = isThereACow()
		if foundCow:
			break
	turnLeft(missingAngles)
	return foundCow


def walkingDetecting(terZonPos):
	foundCow=False
	missingAngles=0
	startedLeft=0

	if terZonPos=='l':
		startedLeft=True
		BackwardCms(75)
	else:
		ForwardCms(75)
		turnRight(180)

	while foundCow == False:
		if startedLeft == True:
			
			foundCow=checkingTurningR()
			if foundCow:
				break

			ForwardCms(75)
			foundCow=checkingTurningL()
			if foundCow:
				break


			BackwardCms(75)
			foundCow=checkingTurningR()
			if foundCow:
				break

		
		foundCow=checkingTurningL()
		if foundCow:
			break

		ForwardCms(75)
		foundCow=checkingTurningR()
		if foundCow:
			break			

		BackwardCms(75)
		foundCow=checkingTurningL()
		startedLeft=True
				
def alignWithCow():
	centerFrame=vs.getXCenterFrame()
	foundCow,maxLenTissue,_ = isThereACow()
	if foundCow:
		cowCenter = getCowXCenter(maxLenTissue)
		degrDif= abs(cowCenter - centerFrame)

		if (cowCenter < centerFrame - 10):
			#gira a la izquierda x grados
			pass
		elif(cowCenter > centerFrame + 10):
			#gira a la derecha x grados
			pass

validation,_,_=isThereACow()
print (validation)


			
