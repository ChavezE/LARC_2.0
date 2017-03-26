
import cv2
import time
import math
import numpy as np
import LARC1 as rb
import random
import serial
import statistics
from copy import deepcopy



# Simple class to manage individual squares in the image
# ATRIBUTES:
# x = y = w = h = area = level = 0
# topLeftC = topRightC = botLeftC = botRightC = []
class cowSquare:
   ##-----------ATRIBUTES-----------##
   x = y = w = h = area = level = 0
   topLeftC = topRightC = botLeftC = botRightC = []
   ##-------------------------------##

   ##----------METHODS-----------##
   def __init__(self,xT,yT,wT,hT,areaT):
      self.x = xT
      self.y = yT
      self.w = wT
      self.h = hT
      self.area = areaT
      self.topLeftC = [xT,yT]
      self.topRightC = [xT+wT,yT]
      self.botLeftC = [xT,yT+hT]
      self.botRightC = [xT+wT,yT+hT]
   ##----------------------------##

   ##-----GETs-----##
   def getX(self):
      return self.x
   def getY(self):
      return self.y
   def getW(self):
      return self.w
   def getH(self):
      return self.h
   def getArea(self):
      return self.area
   def getTopLeftC(self):
      return self.topLeftC
   def getTopRightC(self):
      return self.topRightC
   def getBotLeftC(self):
      return self.botLeftC
   def getBotRightC(self):
      return self.botRightC
   def getLevel(self):
      return self.level
   ##--------------##

   ##-----SETs-----##
   def setX(self,xT):
      self.x = xT
   def setY(self,yT):
      self.y = yT
   def setW(self,wT):
      self.w = wT
   def setH(self,hT):
      self.h = hT
   def setArea(self,areaT):
      self.area = areaT
   def setLevel(self,lev):
      self.level = lev
   ##--------------##

   ##-----END of Class 'cowSquare'-----

# Returns distance between two point in the image.
def distance(x1,y1,x2,y2 ):
  return math.sqrt(pow(x2 - x1,2) + pow(y2 - y1,2))

# call for tissue
def doTissue(goodSqrs):
	# setting constants #
	tissue = []
	biggestTissue = []
	eps = 50
	# ------------------ #
	for i in range(len(goodSqrs)):
		print(goodSqrs[i].getTopLeftC())
	print("buling form now on")

	while( len(goodSqrs) > 0):
		print ("here i am")
		print (goodSqrs[0].getTopLeftC())
		tActSqr = goodSqrs.pop(0)
		tissue.append(tActSqr)
		makeTissue(tActSqr,goodSqrs,tissue,eps)

		if(len(tissue) > len(biggestTissue)):
			biggestTissue = deepcopy(tissue)
		tissue[:] = []

	print ("im done with the tissue")
	return biggestTissue

# build the tissue
def makeTissue(tActSqr,tAllSqrs,tissue,eps):
		
	found = False
	for tSq in (tAllSqrs):

		# UPPER 
		if (distance( tActSqr.getTopLeftC()[0], tActSqr.getTopLeftC()[1] - 2*tActSqr.getH(), tSq.getTopLeftC()[0], tSq.getTopLeftC()[1]) < eps):
			tissue.append(tSq)
			tAllSqrs.pop(tAllSqrs.index(tSq))
			found = True
			makeTissue(tissue[len(tissue)-1],tAllSqrs,tissue,eps)

		# LOWER 
		elif (distance( tActSqr.getTopLeftC()[0], tActSqr.getTopLeftC()[1] + 2*tActSqr.getH(), tSq.getTopLeftC()[0], tSq.getTopLeftC()[1]) < eps):
			tissue.append(tSq)
			tAllSqrs.pop(tAllSqrs.index(tSq))
			found = True
			makeTissue(tissue[len(tissue)-1],tAllSqrs,tissue,eps)

		# RIGHT
		elif (distance( tActSqr.getTopLeftC()[0] + 2*tActSqr.getW(), tActSqr.getTopLeftC()[1], tSq.getTopLeftC()[0], tSq.getTopLeftC()[1]) < eps):
			tissue.append(tSq)
			tAllSqrs.pop(tAllSqrs.index(tSq))
			found = True
			makeTissue(tissue[len(tissue)-1],tAllSqrs,tissue,eps)

		# LEFT
		elif (distance( tActSqr.getTopLeftC()[0] - 2*tActSqr.getW(), tActSqr.getTopLeftC()[1], tSq.getTopLeftC()[0], tSq.getTopLeftC()[1]) < eps):
			tissue.append(tSq)
			tAllSqrs.pop(tAllSqrs.index(tSq))
			found = True
			makeTissue(tissue[len(tissue)-1],tAllSqrs,tissue,eps)

		# UPPER RIGHT
		elif (distance( tActSqr.getTopLeftC()[0] + tActSqr.getW(), tActSqr.getTopLeftC()[1], tSq.getTopLeftC()[0], tSq.getTopLeftC()[1] + tSq.getH()) < eps):
			tissue.append(tSq)
			tAllSqrs.pop(tAllSqrs.index(tSq))
			found = True
			makeTissue(tissue[len(tissue)-1],tAllSqrs,tissue,eps)

		# UPPER LEFT
		elif (distance( tActSqr.getTopLeftC()[0], tActSqr.getTopLeftC()[1], tSq.getTopLeftC()[0] + tSq.getW(), tSq.getTopLeftC()[1] + tSq.getH()) < eps):
			tissue.append(tSq)
			tAllSqrs.pop(tAllSqrs.index(tSq))
			found = True
			makeTissue(tissue[len(tissue)-1],tAllSqrs,tissue,eps)

		# LOWER RIGHT
		elif (distance( tActSqr.getTopLeftC()[0]+tActSqr.getW(), tActSqr.getTopLeftC()[1] + tActSqr.getH(), tSq.getTopLeftC()[0] , tSq.getTopLeftC()[1] ) < eps):
			tissue.append(tSq)
			tAllSqrs.pop(tAllSqrs.index(tSq))
			found = True
			makeTissue(tissue[len(tissue)-1],tAllSqrs,tissue,eps)

		# LOWER LEFT
		elif (distance( tActSqr.getTopLeftC()[0], tActSqr.getTopLeftC()[1]+ tActSqr.getH(), tSq.getTopLeftC()[0] + tSq.getW(), tSq.getTopLeftC()[1] ) < eps):
			tissue.append(tSq)
			tAllSqrs.pop(tAllSqrs.index(tSq))
			found = True
			makeTissue(tissue[len(tissue)-1],tAllSqrs,tissue,eps)

	if found == False:
		tissue.pop(tissue.index(tActSqr))


# IN:	Frame is grayscale image, n is num of corners, quality is 0-1	
# OUT: list of (x,y) coordenates with >= n strongest corners
def getCorners(frame,n,quality):
	corners = cv2.goodFeaturesToTrack(frame,n,quality,10)
	corners = np.int0(corners)
	return corners

def cornersMatch(cnt,corners,minC,eps):
	x,y,w,h = cv2.boundingRect(cnt)
	matches = 0
	for i in corners:
		xc,yc = i.ravel()
		# upper left
		if(abs(x - xc) < eps and abs(y - yc) < eps):	
			matches = matches + 1
		# upper right
		if(abs((x+w) - xc) < eps and abs(y - yc) < eps):
			matches = matches + 1
		# lower left
		if(abs(x + xc) < eps and abs((y+h) - yc) < eps):
			matches = matches + 1
		# lower right
		if(abs((x+w) + xc) < eps and abs((y+h) - yc) < eps):
			matches = matches + 1


	return matches >= minC


	


def getGoodSquares(binFrame,contours,corners):
	# setting constants
	minArea = 400
	maxArea = 8000

	# variable to store the good squares
	goodSqrs = []

	for cnt in contours:
		area = cv2.contourArea(cnt)		# area of the contour itself
		rect = cv2.minAreaRect(cnt)		# rotated rectangle
		w = int(rect[1][0])
		h = int(rect[1][1])
		rect_area = w * h

		if(area > 0):
			extent = float(area / rect_area)
			if(area >= minArea and area <= minArea):
				x,y,w,h = cv2.boundingRect(cnt)

				if(thres[y + h*0.5,x + w*0.5] == 1 and w/h < 3):	# is a black rectangle ans sqr
					m = cornersMatch(cnt,corners,2,10)
					if(extent >= 0.88):								# very simetric square
						tempSqr = cowSquare(x,y,w,h,area)			# ADD TO LIST
						goodSqrs.append(tempSqr)
					elif(extent >= 0.5 and m == True):
						tempSqr = cowSquare(x,y,w,h,area)
						goodSqrs.append(tempSqr)
					else:
						pass

	return goodSqrs





#############################################
##--------------  PRINTS ------------------##
#############################################

def printCorners(frame, corners):
	for i in corners:
		x,y = i.ravel()
		cv2.circle(frame,(x,y),3,255,-1)
	return frame
