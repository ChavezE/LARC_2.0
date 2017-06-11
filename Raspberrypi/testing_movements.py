

# standar libs
import cv2
import numpy as np
import serial
import time
from copy import deepcopy

# Roborregos libs
import Larc_vision_2017 as rb
import communication as com

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


def getCowXcenter(maxLenT):
	left,right,up=rb.calcCowLimits(maxLenT)
	# print "cow x dist: ", right - left
	# print "y: ", up
	drawLimits(left,right,up)
	return (left+right)/2

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
			else:
				print "COW NOT FOUND"
			print ("TOTAL TIME: ",time.time() - this_time)
			cv2.imshow("limits",mainFrame)
			print( mainFrame.shape)
			k = cv2.waitKey(0)
			if k ==27:
				break
			cv2.destroyAllWindows()
			c = cv2.waitKey(0)
			if c == 27:
				break

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


	







