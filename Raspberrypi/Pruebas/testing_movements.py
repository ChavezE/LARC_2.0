

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

'''
    MAIN
'''
if __name__ == "__main__":

	while True:

		analyse = raw_input("process photo? ")
		if analyse == '1':
			takePicture()
			cv2.imshow('im',mainFrame)
			this_time = time.time()
			validation,maxLenT,_ = rb.isThereACow(mainFrame)
			# print validation, len(maxLenT)
			# print "center camera: ",(mainFrame.shape[1])/2

			if validation:
				print "COW FOUND"
				print "center cow: ",getCowXcenter(maxLenT)
				tLevel = rb.getTissueTopLevel(maxLenT)
				rb.drawCowSquares(mainFrame,100,100,100,tLevel)
				A,B,theta = rb.ajusteDeCurvas(tLevel)
				rb.drawSlope(mainFrame,A,B)
				left,right,up=getLimits(maxLenT)

				cowCenter = getCowXcenter(left,right)
				frameCenter = getXCenterFrame()
				# showing level of tissue
				for sq in maxLenT:
					x = sq.getTopLeftC()[0]
					y = sq.getTopLeftC()[1]
					cv2.putText(mainFrame,str(sq.getLevel()),(x,y), cv2.FONT_HERSHEY_SIMPLEX, 0.5,(0,0,255),1,1)

			else:
				print "COW NOT FOUND"


			print ("TOTAL TIME: ",time.time() - this_time)
			cv2.imshow("limits",mainFrame)
			print( mainFrame.shape)
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
