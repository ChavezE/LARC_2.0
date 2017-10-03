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
import sys
sys.path.insert(0, '../lib/')
import Larc_vision_2017 as rb



######### GOBAL VARIABLES #############
mainFrame = []	# frame


def takePicture(frameNumber):
	global mainFrame
	# mainFrame=cv2.imread('../TestPhotos/'+str(frameNumber)+'.jpg')
	mainFrame=cv2.imread('../TestPhotos/480-640/'+str(frameNumber)+'.jpg')
	# mainFrame=cv2.pyrDown(mainFrame)
	# mainFrame =  cv2.resize(mainFrame,None,fx=0.25,fy=0.25,interpolation=cv2.INTER_AREA)
	cv2.imshow("frame: " + str(frameNumber),mainFrame)
	cv2.waitKey(0)
	# for i in range(10):
	# 	goodFrm, mainFrame = cap.read()
	# print "I took a pic"

	return True #goodFrm


'''
    MAIN
'''
if __name__ == "__main__":
	validation=False
	for x in range(39,48):
		frameNumber=x
		this_time = time.time()
		takePicture(frameNumber)
		clearedFrame = rb.clearImage(mainFrame)
		validation2, filtered = rb.detectCow(clearedFrame)
		validation,maxLenT,_ = rb.isThereACow(mainFrame,filtered)
		#If implementing filterForCow(), the second argument must be "filtered",
		#else the secund argument must be "clearedFrame"

		# print validation, len(maxLenT)
		# print "center camera: ",(mainFrame.shape[1])/2
		if validation:
			print "COW FOUND"

			tLevel = rb.getTissueTopLevel(maxLenT)
			# rb.drawCowSquares(mainFrame,100,100,100,tLevel)
			A,B,theta = rb.ajusteDeCurvas(tLevel)
			rb.drawSlope(mainFrame,A,B)
			left,right,up=rb.calcCowLimits(maxLenT)
			rb.drawLimits(mainFrame,left,right,up)

			# showing level of tissue
			# for sq in maxLenT:
			# 	x = sq.getTopLeftC()[0]
			# 	y = sq.getTopLeftC()[1]
			# 	cv2.putText(mainFrame,str(sq.getLevel()),(x,y), cv2.FONT_HERSHEY_SIMPLEX, 0.5,(0,0,255),1,1)

			cowCenter = rb.getCowXCenter(maxLenT)
			frameCenter = rb.getXCenterFrame(mainFrame)


		else:
			print "COW NOT FOUND"
		print ("TOTAL TIME: ",time.time() - this_time)
		cv2.imshow("limits",mainFrame)
		print mainFrame.shape
		k = cv2.waitKey(0)
		if k ==27:
			break
		cv2.destroyAllWindows()
