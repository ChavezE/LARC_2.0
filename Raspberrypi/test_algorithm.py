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
import Larc_vision_2017 as rb


######### GOBAL VARIABLES #############
mainFrame = []	# frame


def takePicture(frameNumber):
	global mainFrame
	mainFrame=cv2.imread('test_photos/'+str(frameNumber)+'.jpg')
	mainFrame=cv2.pyrDown(mainFrame)
	# mainFrame =  cv2.resize(mainFrame,None,fx=0.25,fy=0.25,interpolation=cv2.INTER_AREA)
	# cv2.imshow("frame: " + str(frameNumber),mainFrame)
	# cv2.waitKey(0)
	# for i in range(10):
	# 	goodFrm, mainFrame = cap.read()
	# print "I took a pic"

	return True #goodFrm

def getCowXcenter(maxLenT):
	left,right,up=rb.calcCowLimits(maxLenT)
	# print "cow x dist: ", right - left
	# print "y: ", up
	drawLimits(left,right,up)
	return (left+right)/2

def drawLimits(left,right,y):
	global mainFrame
	font = cv2.FONT_HERSHEY_SIMPLEX
	cv2.line(mainFrame,(left,0),(left,480),(0,0,255),2)
	cv2.line(mainFrame,(right,0),(right,480),(0,0,255),2)
	cv2.line(mainFrame,(0,y),(640,y),(0,0,255),2)
	# cv2.putText(mainFrame,("diff L: " + str(left)),(30,20), font, 0.8,(0,0,255),1,cv2.LINE_AA)
	# cv2.putText(mainFrame,("diff R: " + str(640-right)),(30,50), font, 0.8,(0,0,255),1,cv2.LINE_AA)
	# cv2.putText(mainFrame,("diff Top: " + str(y)),(30,80), font, 0.8,(0,0,255),1,cv2.LINE_AA)


'''
    MAIN
'''
if __name__ == "__main__":
	validation=False
	for x in range(1,9):
		frameNumber=x
		this_time = time.time()
		takePicture(frameNumber)
		validation,maxLenT,_ = rb.isThereACow(mainFrame)

		# print validation, len(maxLenT)
		# print "center camera: ",(mainFrame.shape[1])/2
		if validation:
			print "COW FOUND"
			tLevel = rb.getTissueTopLevel(maxLenT)
			# rb.drawCowSquares(mainFrame,100,100,100,tLevel)
			A,B,theta = rb.ajusteDeCurvas(tLevel)
			# rb.drawSlope(mainFrame,A,B)
			print "center cow: ",getCowXcenter(maxLenT)
			# showing level of tissue
			# for sq in maxLenT:
			# 	x = sq.getTopLeftC()[0]
			# 	y = sq.getTopLeftC()[1]
			# 	cv2.putText(mainFrame,str(sq.getLevel()),(x,y), cv2.FONT_HERSHEY_SIMPLEX, 0.5,(0,0,255),1,1)
		else:
			print "COW NOT FOUND"
		print ("TOTAL TIME: ",time.time() - this_time)
		cv2.imshow("limits",mainFrame)
		print( mainFrame.shape)
		k = cv2.waitKey(0)
		if k ==27:
			break
		cv2.destroyAllWindows()
