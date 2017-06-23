# importing external libs
import cv2
import numpy as np
from copy import deepcopy
from matplotlib import pyplot as plt
import serial
import time
import math
import random
# imporging Roborregos libs
import Larc_vision_2017 as rb

imgR = cv2.imread('./depth_photos/b2_1.jpg',0)
imgL = cv2.imread('./depth_photos/base2.jpg',0)
mainFrame = cv2.imread('./depth_photos/base2.jpg')

cv2.imshow('derecha',imgR)
cv2.imshow('main',mainFrame)
cv2.imshow('izquierda',imgL)
cv2.waitKey(0)
cv2.destroyAllWindows()



def getCowXcenter(frame,maxLenT):
	left,right,up=rb.calcCowLimits(maxLenT)
	# print "cow x dist: ", right - left
	# print "y: ", up
	drawLimits(frame,left,right,up)
	return (left+right)/2

def drawLimits(frame,left,right,y):

	font = cv2.FONT_HERSHEY_SIMPLEX
	cv2.line(frame,(left,0),(left,480),(0,0,255),2)
	cv2.line(frame,(right,0),(right,480),(0,0,255),2)
	cv2.line(frame,(0,y),(640,y),(0,0,255),2)
	# cv2.putText(mainFrame,("diff L: " + str(left)),(30,20), font, 0.8,(0,0,255),1,cv2.LINE_AA)
	# cv2.putText(mainFrame,("diff R: " + str(640-right)),(30,50), font, 0.8,(0,0,255),1,cv2.LINE_AA)
	# cv2.putText(mainFrame,("diff Top: " + str(y)),(30,80), font, 0.8,(0,0,255),1,cv2.LINE_AA)



# calcultation time
this_time = time.time()

#stereo obj intializer
stereo = cv2.StereoBM(0,16,35)
disparity = abs(stereo.compute(imgL, imgR).astype(np.uint8))
# plt.imshow(disparity,'gray')
# plt.show()

# mask= cv2.threshold(disparity, 170, 255, cv2.THRESH_BINARY)[1]
mask1= cv2.inRange(disparity,80,110)
# closing mask erosion ad dilate
mask2 = cv2.erode(mask1,(11,11),iterations = 5)
mask2 = cv2.dilate(mask2,(11,11),iterations = 25)
# putting mask over image
maskedFrame = cv2.bitwise_and(mainFrame,mainFrame,mask = mask2)

validation,maxLenTMasked,_ = rb.isThereACow(maskedFrame)
if validation:
		tLevel = rb.getTissueTopLevel(maxLenTMasked)
		# rb.drawCowSquares(mainFrame,100,100,100,tLevel)
		A,B,theta = rb.ajusteDeCurvas(tLevel)
		# rb.drawSlope(mainFrame,A,B)
		print "center cow: ",getCowXcenter(maskedFrame,maxLenTMasked)

# validation,maxLenT,_ = rb.isThereACow(mainFrame)
# if validation:
# 		tLevel = rb.getTissueTopLevel(maxLenT)
# 		# rb.drawCowSquares(mainFrame,100,100,100,tLevel)
# 		A,B,theta = rb.ajusteDeCurvas(tLevel)
# 		# rb.drawSlope(mainFrame,A,B)
# 		print "center cow: ",getCowXcenter(mainFrame,maxLenT)
#

print ("TOTAL TIME: ",time.time() - this_time)

cv2.imshow('maskedFrame',maskedFrame)
# cv2.imshow('normalFrame',mainFrame)
cv2.imshow('thereshold closed',mask2)


cv2.waitKey(0)
cv2.destroyAllWindows()
plt.imshow(disparity,'gray')
plt.show()
