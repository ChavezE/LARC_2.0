'''
	Driver to test the algorithm that find the cow
'''

# importing external libs
import cv2
import numpy as np
import serial
import time
from copy import deepcopy
import math
import random
# imporging Roborregos libs
import Larc_vision_2016 as rb


######### GOBAL VARIABLES #############
mainFrame = []	# frame

def isThereACow():
	global mainFrame
	maxLenT = [] # maximumLenghtTissue
	allSquares = [] # Store in each iteration of the binarization the squares found in the image
	minNumSquares = 4
	takePicture()
	# iterate to get max squares from the image
	# best way so far to counter ligh strokes 
	filteredFrame = rb.clearImage(mainFrame)
	equalizedFrame = cv2.equalizeHist(filteredFrame)

	for binValueT in range(5,131,3):


		cp0 = deepcopy(equalizedFrame)
		main_copy2=mainFrame.copy()
		thresFrame0 = rb.doThresHold(cp0, binValueT,3,1)
		cv2.imshow("thres0: ", thresFrame0)
		cv2.waitKey(5)
		contours0 = rb.findContours(thresFrame0)
		cowRectangles0,_ = rb.getGoodSquares(contours0,thresFrame0,main_copy2) # From contours, extract possile cow squares
		# cowRectangles0 = getGoodSquares2(thresFrame0,contours0,corners)
		rb.findEquals(allSquares,cowRectangles0,15)
		del cp0
		
		# cp0 = cp1 = cp2 = deepcopy(equalizedFrame)
		# main_copy2=mainFrame.copy()


		# thresFrame0 = rb.doThresHold(cp0, binValueT,7,1) 
		# cv2.imshow("thres0: ", thresFrame0)
		# cv2.waitKey(5)
		# contours0 = rb.findContours(thresFrame0) 
		# cowRectangles0,_ = rb.getGoodSquares(contours0,thresFrame0,main_copy2) 
		# findEquals(allSquares,cowRectangles0,15)

		# thresFrame1 = rb.doThresHold(cp1, binValueT,3,3) # Thresholds the image and erodes it
		# contours1 = rb.findContours(thresFrame1) # Finds all the contours inside the image
		# cowRectangles1,_ = rb.getGoodSquares(contours1,thresFrame1,main_copy2) # From contours, extract possile cow squares
		# findEquals(allSquares,cowRectangles1,15)

		# thresFrame2 = rb.doThresHold(cp2, binValueT,5,2) 
		# contours2 = rb.findContours(thresFrame2) 
		# cowRectangles2,_ = rb.getGoodSquares(contours2,thresFrame2,main_copy2) 
		# findEquals(allSquares,cowRectangles2,15)

		# del cp0
		# del cp1
		# del cp2
	

	if len(allSquares) > minNumSquares:
		tempAllSquares = deepcopy(allSquares)
		maxLenT = rb.doTissue(tempAllSquares)
		for sqr in maxLenT:
			cv2.rectangle(main_copy2, (sqr.getTopLeftC()[0],sqr.getTopLeftC()[1]), (sqr.getBotRightC()[0],sqr.getBotRightC()[1]), (127,50,127), 2)
			cv2.imshow("squares of " + str(binValueT),main_copy2)
			cv2.waitKey(5)

		if len(maxLenT) > minNumSquares:
			return True,maxLenT,allSquares

	return False,[],[]

def takePicture():
	global mainFrame
	mainFrame=cv2.imread('test_photos/'+str(frameNumber)+'.jpg')
	mainFrame =  cv2.resize(mainFrame,None,fx=0.5,fy=0.5,interpolation=cv2.INTER_AREA)
	cv2.imshow("frame: " + str(frameNumber),mainFrame)
	cv2.waitKey(0)
	# for i in range(10):
	# 	goodFrm, mainFrame = cap.read()
	# print "I took a pic"

	return True #goodFrm

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
	cv2.line(mainFrame,(0,y),(640,y),(255,0,0),3)
	# cv2.putText(mainFrame,("diff L: " + str(left)),(30,20), font, 0.8,(0,0,255),1,cv2.LINE_AA)
	# cv2.putText(mainFrame,("diff R: " + str(640-right)),(30,50), font, 0.8,(0,0,255),1,cv2.LINE_AA)
	# cv2.putText(mainFrame,("diff Top: " + str(y)),(30,80), font, 0.8,(0,0,255),1,cv2.LINE_AA)


'''
    MAIN
'''
if __name__ == "__main__":
	validation=False
	for x in range(1,19):
		frameNumber=x
		validation,maxLenT,_ =isThereACow()
		print validation, len(maxLenT)
		print "center camera: ",(mainFrame.shape[1])/2
		if validation:
			print "center cow: ",getCowXcenter(maxLenT)
		cv2.imshow("limits",mainFrame)
		cv2.waitKey(0)
		cv2.destroyAllWindows()
    