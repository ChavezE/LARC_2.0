# hi hi hi
## This is version 2.0 of LARC 2016 Module of Mexican Team RoBorregos
## which stores functions used by the computer vision squad. 
## All rights reserved by Instituo Tecnologico de Monterrey.

## AUTHORS:    Emilio Chavez Madero
##             Sebastian Rivera Gonzalez
##             Diego Garza Rodriguez

## Updated and commented on December 2016 by Emilio Chavez

#############################################
##----------------MAIN LIBS----------------##
#############################################
import cv2
import time
import math
import numpy as np
import LARC1 as rb
import random
import serial
import statistics
from copy import deepcopy

import updates as up



#############################################
##------------------CLASSES----------------##
#############################################

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

# clustering class NOT USED
class cluster:
   ##-----------ATRIBUTES-----------##
   center = []
   new_points = []
   old_points = []
   ##-------------------------------##

   ##----------METHODS-----------##
   def __init__(self, init_center):
      self.center = init_center
      self.new_points = []
      self.old_points = []

   def calculate_center(self):
      x_sum = 0
      y_sum = 0
      for i in range (len(self.new_points)):
         x_sum = x_sum + self.new_points[i][0]
         y_sum = y_sum + self.new_points[i][1]
      if(len(self.new_points) > 0):
         x_center = int(x_sum/len(self.new_points))
         y_center = int(y_sum/len(self.new_points))
         self.center = (x_center,y_center)

   def add_point(self,new_p):
      self.new_points.append(new_p)
  
   def set_center(self,point):
      self. center = point

   def distance(self,p):
      x = p[0]
      y = p[1]
      xDif = abs(x - self.center[0])
      yDif = abs(y - self.center[1])
      return math.sqrt((xDif*xDif) + (yDif*yDif))

   def update_points(self):
      self.old_points = list(self.new_points)
      self.new_points = []

   def get_center(self):
      return self.center

   def get_new_points(self):
      return list(self.new_points)

   def get_old_points(self):
      return list(self.old_points)
   ##----------------------------##
   ##-----END of Class 'Cluster'-----##


#############################################
##----------- IMAGE FUNCTIONS-----------##
#############################################
# To work over the imageS to get them ready, obatain 
# general data and use it in further algorithms

# Does some guassian filtering to remove noise and converts image to gray scale
def clearImage(imgOriginal):

   imGray = cv2.cvtColor(imgOriginal, cv2.COLOR_BGR2GRAY)
   imGray = cv2.GaussianBlur(imGray, (3,3), 2)
   # imGray = cv2.fastNlMeansDenoisingColored(imgOriginal,None,10,10,7,21)

   return imGray

# Binarized the image in respecto to tVal and erodes the image to sepate
# the squares from each other.
# k is kernel size, parameter 'i' is the iterations, both for the erosion
def doThresHold(filteredImage,tVal,k,i):
   _, thres1 = cv2.threshold(filteredImage,tVal,255,cv2.THRESH_BINARY_INV)
   thres1 = cv2.erode(thres1,np.ones((k,k),np.uint8), iterations=i)
   
   return thres1

# Finds contours in the image and returns them as a numpy list. 
# Recieves a simple image as a parameter.
def findContours(img):

   contours, hierarchy = cv2.findContours(img, cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
   return contours

# Processes the contours found in order to find squares in the image
# returns a list of objects 'cowsquare' class:
# cowSquare(x,y,w,h,area)
def getGoodSquares(contours,imgOriginal,thres):

   # ----VARIABLES----
   cowSquares = []   # This list is the one that is going to being returned
   # -----------------

   for cnt in contours:
      area = cv2.contourArea(cnt)
      rect = cv2.minAreaRect(cnt)      # rotated rectangle
      w = int(rect[1][0])
      h = int(rect[1][1])
      rect_area = w * h
      if(rect_area > 0): # sometimes this value is found
         extent = float(area / rect_area)
         if (extent >= 0.75 and area >= 400 and area <= 8500):   # tolerance
            x,y,w,h = cv2.boundingRect(cnt)
            if thres[y + h*0.5,x + w*0.5] == 1 and w/h < 3:
               tempCowSquare = cowSquare(x,y,w,h,area)    # Create an objet from the 'cowSquare' class
               cowSquares.append(tempCowSquare) # Insert object 'cowSquare' into a list  

   # Print elements in cowSqrs
   # b = int ( random.uniform(50,255))
   # g = int ( random.uniform(50,255))
   # r = int ( random.uniform(50,255))
   # printCowSquares(imgOriginal,b,g,r,cowSquares)
                                       
   return cowSquares

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
      if(abs(x - xc) < eps and abs((y+h) - yc) < eps):
         matches = matches + 1
      # lower right
      if(abs((x+w) - xc) < eps and abs((y+h) - yc) < eps):
         matches = matches + 1


   return matches >= minC

def getGoodSquares2(binFrame,contours,corners):
   # setting constants
   minArea = 10
   maxArea = 8500

   # variable to store the good squares
   goodSqrs = []

   for cnt in contours:
      area = cv2.contourArea(cnt)      # area of the contour itself
      rect = cv2.minAreaRect(cnt)      # rotated rectangle
      w = int(rect[1][0])
      h = int(rect[1][1])
      rect_area = w * h

      if(rect_area > 0):
         extent = float(area / rect_area)
         if(area >= minArea and area <= maxArea):

            x,y,w,h = cv2.boundingRect(cnt)
            if(binFrame[y + h*0.5,x + w*0.5] == 1 and w/h < 3):   # is a black rectangle ans sqr
               m = cornersMatch(cnt,corners,2,15)
               #print m
               if(extent >= 0.88):                       # very simetric square
                  tempSqr = cowSquare(x,y,w,h,area)        
                  goodSqrs.append(tempSqr)
                  
               elif(extent >= 0.5 and m == True):        # BEST CONSTANT
                  tempSqr = cowSquare(x,y,w,h,area)
                  goodSqrs.append(tempSqr)
                  #print "here"
               else:
                  pass

   return goodSqrs


# Returns distance between two point in the image.
def distance(x1,y1,x2,y2):
  return math.sqrt(pow(x2 - x1,2) + pow(y2 - y1,2))


#############################################
##------------ DESSISION MAKING------------##
#############################################
# To take the last steps over dessions in dataflow

# ONE OF THE MOST IMPORTANT FUNCTION
# Description of the parameters:
# tCowSquares -> Stores the remaining squares and it will decrease its size in 1 each time a new neighboor is found
# tissue -> The current tissue that is being formed and that will be compared with greatestTissue 
# epsilon -> is a parameter determined in the main module and is the margin of error when checking for neighboors
# checkWith -> could have a value of 1, 2, 3, or 4 when searching for neighboors and it determines which corner of the 
# square is being used and which is the one that need to be compared with. It could also be 0 but just when it is called
# from the main module
# corner -> is a coordinate that corresponds to a specific corner which is determined by the checkWith parameter
# curLevel -> Determines the level of the square to keep track of where it was found inside the tissue
def makeTissue(tCowSquares, tissue, epsilon, checkWith, corner, curLevel):

   if checkWith == 0:   # First call to the function
      greatestTissue = []
      tissue = []

      while len(tCowSquares) > 0:
         actSquare = tCowSquares.pop(0)
         actSquare.setLevel(0)
         tissue.append(actSquare)
         makeTissue(tCowSquares, tissue, epsilon, 1, actSquare.getTopLeftC(), 0)
         makeTissue(tCowSquares, tissue, epsilon, 2, actSquare.getTopRightC(), 0)
         makeTissue(tCowSquares, tissue, epsilon, 3, actSquare.getBotLeftC(), 0)
         makeTissue(tCowSquares, tissue, epsilon, 4, actSquare.getBotRightC(), 0)

         # Compare the tissue that was formed with the current 'greatestTissue'
         if len(tissue) > len(greatestTissue):
            greatestTissue = deepcopy(tissue)
         tissue[:] = []

      greatestTissue = sorted(greatestTissue, key=lambda x:x.getLevel(), reverse=True)
      iA = 0
      # Remove those squares who are unique at a specific level
      while iA < len(greatestTissue) and len(greatestTissue) > 1:
         if(iA == len(greatestTissue) - 1):
            if(greatestTissue[iA-1].getLevel() != greatestTissue[iA].getLevel()):
               greatestTissue.pop(iA)
            else:
               iA += 1
         elif(iA == 0):
            if(greatestTissue[iA+1].getLevel() != greatestTissue[iA].getLevel()):
               greatestTissue.pop(iA)
            else:
               iA += 1
         else:
            if(greatestTissue[iA-1].getLevel() != greatestTissue[iA].getLevel() and greatestTissue[iA+1].getLevel() != greatestTissue[iA].getLevel()):
               greatestTissue.pop(iA)
            else:
               iA += 1
         
      return greatestTissue

   elif checkWith == 1:   # corner => topLeft
      found = False     # Flag to check if the tissue found a neighboor
      # found2 = False
      for i in range(len(tCowSquares)):    # Go over the remaining elements in 'tCowSquares' from 0 to len(cowSquares)-1
         cowSquare= tCowSquares[i]        # temporal square
         compareCoord = cowSquare.getBotRightC()   # use the corner of interest depending on the attribute 'checkWith'
         # compareCoord2 = cowSquare.getTopRightC()
         # Find the distance between the two corners in order to find adjacent ones
         if(distance(corner[0],corner[1],compareCoord[0],compareCoord[1]) < epsilon and compareCoord[1] - 30 <= corner[1]):
            cowSquare.setLevel(curLevel + 1)   # Set the level of the square
            tissue.append(cowSquare)  # Add 'cowSquare' to the list 'tissue'
            found = True
            tCowSquares.pop(i)
            break      

      if found:
         tempElement = tissue[len(tissue)-1]
         makeTissue(tCowSquares, tissue, epsilon, 1, tempElement.getTopLeftC(), curLevel + 1)
         makeTissue(tCowSquares, tissue, epsilon, 2, tempElement.getTopRightC(), curLevel + 1)
         makeTissue(tCowSquares, tissue, epsilon, 3, tempElement.getBotLeftC(), curLevel + 1)

   elif checkWith == 2:   # corner => topRight
      found = False
      # found2 = False
      for i in range(len(tCowSquares)):
         cowSquare= tCowSquares[i]
         compareCoord = cowSquare.getBotLeftC()
         # compareCoord2 = cowSquare.getTopLeftC()

         # Find the distance between the two corners in order to find adjacent ones
         if(distance(corner[0],corner[1],compareCoord[0],compareCoord[1]) < epsilon and compareCoord[1] - 30<= corner[1]):
            cowSquare.setLevel(curLevel + 1)   # Set the level of the square
            tissue.append(cowSquare)  # Add 'cowSquare' to the list 'tissue'
            found = True
            tCowSquares.pop(i)
            break


      if found:
         tempElement = tissue[len(tissue)-1]
         makeTissue(tCowSquares, tissue, epsilon, 1, tempElement.getTopLeftC(), curLevel + 1)
         makeTissue(tCowSquares, tissue, epsilon, 2, tempElement.getTopRightC(), curLevel + 1)
         makeTissue(tCowSquares, tissue, epsilon, 4, tempElement.getBotRightC(), curLevel + 1)


   elif checkWith == 3:   # corner => botLeft
      found = False
      for i in range(len(tCowSquares)):
         cowSquare= tCowSquares[i]
         compareCoord = cowSquare.getTopRightC()

         # Find the distance between the two corners in order to find adjacent ones
         if(distance(corner[0],corner[1],compareCoord[0],compareCoord[1]) < epsilon and compareCoord[1] + 30 >= corner[1]):
            cowSquare.setLevel(curLevel - 1)   # Set the level of the square
            tissue.append(cowSquare)  # Add 'cowSquare' to the list 'tissue'
            found = True
            tCowSquares.pop(i)
            break
      if found:
         tempElement = tissue[len(tissue)-1]
         makeTissue(tCowSquares, tissue, epsilon, 1, tempElement.getTopLeftC(), curLevel - 1)
         makeTissue(tCowSquares, tissue, epsilon, 3, tempElement.getBotLeftC(), curLevel - 1)
         makeTissue(tCowSquares, tissue, epsilon, 4, tempElement.getBotRightC(), curLevel - 1)

   elif checkWith == 4:   # corner => botRight
      found = False
      for i in range(len(tCowSquares)):
         cowSquare= tCowSquares[i]
         compareCoord = cowSquare.getTopLeftC()

         # Find the distance between the two corners in order to find adjacent ones
         if(distance(corner[0],corner[1],compareCoord[0],compareCoord[1]) < epsilon and compareCoord[1] + 30 >= corner[1]):
            cowSquare.setLevel(curLevel - 1)   # Set the level of the square
            tissue.append(cowSquare)  # Add 'cowSquare' to the list 'tissue'
            found = True
            tCowSquares.pop(i)
            break
      if found:
         tempElement = tissue[len(tissue)-1]
         makeTissue(tCowSquares, tissue, epsilon, 2, tempElement.getTopRightC(), curLevel - 1)
         makeTissue(tCowSquares, tissue, epsilon, 3, tempElement.getBotLeftC(), curLevel - 1)
         makeTissue(tCowSquares, tissue, epsilon, 4, tempElement.getBotRightC(), curLevel - 1)

# cSquares is a multidimensional list: [[x1,y1],[x2,y2],...,[xN,yN]]
# These lists and variables are used to calculate A and B
# to get a polynomial of 1st grade: y = Ax + B
def ajusteDeCurvas(cSquares):

   theta = 0
   A = 0
   B = 0
   n = len(cSquares)
   if n > 1:
      Xi = [] # [x1,x2,...,xN]
      Yi = [] # [y1,y2,...,yN]
      XiYi = [] #[x1*y1,x2*y2,...,xN*yN]
      Xi2 = [] #[x1^2, x2^2,...,xN^2]

      # This cycle is to fill up all the lists in order to compute A and B
      for c in cSquares:
         Xi.append(c.getX())
         Yi.append(c.getY())
         XiYi.append(c.getX() * c.getY())
         Xi2.append(pow(c.getX(),2))

      # The reason to create lists is to get the sum of all its elements 
      # so it will be computed the sum of each of the lists
      sXi = sum(Xi)
      sYi = sum(Yi)
      sXiYi = sum(XiYi)
      sXi2 = sum(Xi2)
      
      # Now its time to compute A and B
      A = float((n*sXiYi)-(sXi*sYi))/((n*sXi2)-(pow(sXi,2)))
      B = float(((sXi2*sYi)-(sXiYi*sXi))/((n*sXi2)-(pow(sXi,2))))
      theta = float(math.atan(A))
      theta = float(theta*180/math.pi)
      
   return A,B,theta

# Calculates and returns the limits of the cowlimitLeft, limitRight, topY
# using maxLvl from Tissue
def calcCowLimits(maxLvl):
   maxLvl = sorted(maxLvl, key=lambda x:x.getX(), reverse = False)
   limitRight = maxLvl[len(maxLvl)-1].getTopRightC()[0]
   limitLeft = maxLvl[0].getX()
   maxLvl = sorted(maxLvl, key=lambda x:x.getY(), reverse = False)
   topY = maxLvl[0].getY()
   return limitLeft, limitRight, topY

# Return distance between the corners of the square object
def distanceBCorners(c1,c2):
   x1 = c1[0]
   x2 = c2[0]
   y1 = c1[1]
   y2 = c2[1]
   return rb.distance(x1,y1,x2,y2)

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

# Looks after a tissue in the frame and returns the tissue itself and allsquares
# returns a bolean and empty lists if the squares dont match for a cow 
def isThereACow(frame):
   
   maxLenT = [] # maximumLenghtTissue
   allSquares = [] # Store in each iteration of the binarization the squares found in the image
   minNumSquares = 4
     
   # iterate to get max squares from the image
   # best way so far to counter ligh strokes 
   for binValueT in range(5,131,3):
      cp0 = cp1 = cp2 = deepcopy(frame)

      
      corners = getCorners(cp0,70,0.1)
      #print (corners)
      thresFrame0 = doThresHold(cp0, binValueT,3,1)
      contours0 = findContours(thresFrame0)
      #cowRectangles0 = getGoodSquares(contours0,frame,thresFrame0) # From contours, extract possile cow squares
      cowRectangles0 = getGoodSquares2(thresFrame0,contours0,corners)
      findEquals(allSquares,cowRectangles0,15)

      # thresFrame1 = rb.doThresHold(cp1, binValueT,3,3) 
      # #cv2.imshow('1', thresFrame1)
      # contours1 = findContours(thresFrame1) 
      # cowRectangles1 = getGoodSquares(contours1,frame,thresFrame1) # From contours, extract possile cow squares
      # findEquals(allSquares,cowRectangles1,15)

      # thresFrame2 = doThresHold(cp2, binValueT,5,2) 
      # #cv2.imshow('2', thresFrame2)
      # contours2 = findContours(thresFrame2) 
      # cowRectangles2 = getGoodSquares(contours2,frame,thresFrame2) 
      # findEquals(allSquares,cowRectangles2,15)

      del cp0
      del cp1
      del cp2
      
      
    
      #cv2.waitKey(100)

   print "im done with thres"
   
   #for c in allSquares:
   #   cv2.rectangle(mainFrame,(c.getX(),c.getY()),(c.getX()+c.getW(),c.getY()+c.getH()),(255,255,255),4)
   
   # When there are more than 'minNumSquares', it can be found at least one tissue
   if len(allSquares) > minNumSquares:
      tempAllSquares = deepcopy(allSquares)
      tempAllSquares2 = deepcopy(allSquares)
      maxLenT = rb.makeTissue(tempAllSquares,[],50,0,[0,0],0)
      maxLenT2 = up.doTissue(tempAllSquares2)
      
      if len(maxLenT) > minNumSquares:
         return True,maxLenT,maxLenT2,allSquares
   
   return False,[],[]

# INPUT: maximunLengthTissue found in isThereACow
# OUTPUT : bool to go and milk the cow, limLeft, limRight, limTop
# if tissue[0].getLevel() != tissue[1].getLevel() and tissue[1].getLevel() == tissue[2].getLevel():
#  # To remove any noise that is higher than the cow
#  tissue.pop(0)
def isCowMilkeable(tissue,squares):

   if tissue[0].getLevel() == tissue[1].getLevel() and tissue[1].getLevel() == tissue[2].getLevel():
      # Tissue has the 3 squares on the top...
      if len(tissue) >= 6:
         if (tissue[3].getLevel() == tissue[4].getLevel() and tissue[4].getLevel() == tissue[5].getLevel()):
            # ... and also the tissue has the 3 squares from the second row...
            print "Found the 6 squares from cow"
            tempLevel = [tissue[0],tissue[1],tissue[2],tissue[3],tissue[4],tissue[5]]
            limLeft,limRight,limTop = calcCowLimits(tempLevel)
            return True,limLeft,limRight,limTop,0
      if len(tissue) >= 5:
         if tissue[3].getLevel() == tissue[4].getLevel():
            print "Found 5/6 squares 1.0"
            tempLevel = [tissue[0],tissue[1],tissue[2],tissue[3],tissue[4]]
            limLeft,limRight,limTop = calcCowLimits(tempLevel)
            coinLeft = 0
            coinRight = 0
            for s in squares:
               if limLeft != s.getX() and abs(limLeft - s.getX()) < 15:
                  coinLeft += 1
               elif limRight != s.getX()+s.getW() and abs(limRight - (s.getX()+s.getW())) < 15:
                  coinRight += 1
            if coinLeft != 0 or coinRight != 0:
               if coinLeft > coinRight:
                  limRight += tissue[0].getW()
               else:
                  limLeft -= tissue[0].getW()

               return True,limLeft,limRight,limTop,0

   elif tissue[0].getLevel() == tissue[1].getLevel() and tissue[1].getLevel() != tissue[2].getLevel():
      if len(tissue) >= 5:
         if tissue[2].getLevel() == tissue[3].getLevel() and tissue[3].getLevel() == tissue[4].getLevel():
            print "Found 5/6 squares 2.0"
            tempLevel = [tissue[0],tissue[1],tissue[2],tissue[3],tissue[4]]
            limLeft,limRight,limTop = calcCowLimits(tempLevel)
            coinLeft = 0
            coinRight = 0
            for s in squares:
               if limLeft != s.getX() and abs(limLeft - s.getX()) < 15:
                  coinLeft += 1
               elif limRight != s.getX()+s.getW() and abs(limRight - (s.getX()+s.getW())) < 15:
                  coinRight += 1
            if coinLeft != 0 or coinRight != 0:
               if coinLeft > coinRight:
                  limRight += tissue[0].getW()
               else:
                  limLeft -= tissue[0].getW()

               return True,limLeft,limRight,limTop,0

   
   return False,0,0,0,0
   # listMaxLevel = findMaxLevel(tissue)
   # theta,A,B = rb.ajusteDeCurvas(listMaxLevel)
   # limLeft,limRight,limTop = calcCowLimits(listMaxLevel,tissue)
   
   ##-------PRINTS-------
   # drawGreatestTissue(tissue)
   # drawSlope(A,B)
   # drawLimits(limLeft,limRight,limTop)
   # cv2.imshow('mF',mainFrame)
   # cv2.waitKey(0)
   # cv2.destroyAllWindows()
   ##--------------------
   # if (640 - limLeft - (640 - limRight)) < 200:
   #  # There is a cow but you can't milk it
   #  return False,limLeft,limRight,limTop,theta
   # else:
   #  # Go milk the cow
   #  return True,limLeft,limRight,limTop,theta

###########################################################################
# THE FOLLOWING WERE CONSIDERABLE APPROACH IDEAD BUT NOT USED IN LARC 2016
###########################################################################

# This function is to implement the clustering algorithm 
# PARAMETERS: number of clusters to search for, list of coordinates of the cow
# and number of iterations before returning the final clusters
# First, the list clusters is initialized with n number of Clusters
def findClusters(cowRectangles,iterations,coordClusters):

   clusters = []
   for iA in range(len(coordClusters)):
      cluster = Cluster(coordClusters[iA])
      clusters.append(cluster)
    
   for dummy_iterator in range(iterations):
      for rect in cowRectangles:
         dist = float('inf')
         index = 0
         for i in range(len(clusters)):
            #actClust = clusters[i]
            if(clusters[i].distance(rect) < dist):
               dist = clusters[i].distance(rect)
               index = i
         clusters[index].add_point(rect)
      for cluster in clusters:
         #cluster.update_points()
         cluster.calculate_center()
         cluster.update_points()
   
   return clusters

# This function returnsa list of coords with all those
# that have more than 2 neihbors in a specific radius
def neighboors(cowSquares):

   radius = 100
   neighboors = []
   for i in range (len(cowSquares)):
      xi = cowSquares[i][4]
      yi = cowSquares[i][5]
      count = 0
      for j in range (len(cowSquares)):
         if (i != j):
            xj = cowSquares[j][4]
            yj = cowSquares[j][5]
            if ((abs(xi-xj) < radius) and (abs(yi-yj) < radius)):
               count = count + 1
      if (count > 2):
         neigh.append([xi,yi])
   return neigh

# This function is to implement the clustering algorithm 
# PARAMETERS: number of clusters to search for, list of coordinates of the cow
# and number of iterations before returning the final clusters
# First, the list clusters is initialized with n number of Clusters
def findClusters(cowRectangles,iterations,coordClusters):

   clusters = []
   for iA in range(len(coordClusters)):
      cluster = Cluster(coordClusters[iA])
      clusters.append(cluster)
    
   for dummy_iterator in range(iterations):
      for rect in cowRectangles:
         dist = float('inf')
         index = 0
         for i in range(len(clusters)):
            #actClust = clusters[i]
            if(clusters[i].distance(rect) < dist):
               dist = clusters[i].distance(rect)
               index = i
         clusters[index].add_point(rect)
      for cluster in clusters:
         #cluster.update_points()
         cluster.calculate_center()
         cluster.update_points()
   
   return clusters

# This function is still on progress, hope to return well 
# ordered body of cow in a convenient way 
def getBody(cowRectangles,imgOriginal,totLines,epsilon):

   bodyLines = []
   singleLine = []
   yTest = cowRectangles[0][5]
   i = 0
   b = g = r = 0

   for count in range(0,totLines):
      if count == 0:
         b = 255
      elif count == 1:
         g = 255
      elif count == 2:
         r = 255
      while(abs(cowRectangles[i][5] - yTest) < epsilon):
         x = cowRectangles[i][4]
         y = cowRectangles[i][5]
         #cv2.circle(imgOriginal,(x,y),5,(b,g,r),-1)
         #bodyLines.insert(0,allRect[i])
         singleLine.append((x,y))
         i += 1
      bodyLines.append(singleLine)
      singleLine = []
      print count
      yTest = cowRectangles[i][5]

   return bodyLines

#############################################
##-----------SECONDARY FUNCTIONS-----------##
#############################################

# This functions sorts a multivaraible list depending on the index specified
def sortList(index,l): 

   l = sorted(l, key=lambda x:x[index], reverse=True) 
   
   return l

def findMedian(index,l):
   # The length of the list is odd
   if (len(l) % 2) != 0:     
      return l[len(l)/2][index]
   # The lenght of the list is even
   else:              
      return (l[len(l)/2][index] + l[len(l)/2 + 1][index])/2

# This method will sort countours respect to Y or X coord of bounding Rectangle
   # Critieria defines whether X o Y is used DEFAULT IS y
   # This is always from lef to right of from top to bottom --> Reverse = False
   # Most of this is from here : http://www.pyimagesearch.com/2015/04/20/sorting-contours-using-python-and-opencv/
def boundingRectSort(allRect,criteria):
   
   i = 1
   if criteria == 'X' or criteria ==  'x':
      i = 0
   boundingBoxes = [cv2.boundingRect(c) for c in allRect]
   (allRect, boundingBoxes) = zip(*sorted(zip(allRect, boundingBoxes),
      key=lambda b:b[1][i], reverse=False))
 
   return (allRect, boundingBoxes)



#############################################
##--------------  PRINTS ------------##
#############################################

def printContourCoords(cx,cy,x,y):
   print "Center: %d, %d   LeftTopCorner: %d, %d" % (cx,cy,x,y)

def drawCowSquares(frame,G,B,R,sqrs):
   for sqr in sqrs:
      x = sqr.getX()
      y = sqr.getY()
      w = sqr.getW()
      h = sqr.getH()
      cv2.rectangle(frame,(x,y),(x+w,y+h),(G,B,R),2)
   return frame

def drawSlope(frame,A,B):
   x1 = 0
   x2 = 600
   y1 = int(A*x1 + B)
   y2 = int(A*x2 + B)
   cv2.line(frame,(x1,y1),(x2,y2),(0,255,255),3)

   return frame

def drawGreatestTissue(frame,greatestTissue):
   
   font = cv2.FONT_HERSHEY_SIMPLEX
   areaT = 0
   for c in greatestTissue:
      areaT += c.getArea()
      b = int ( random.uniform(50,255))
      g = int ( random.uniform(50,255))
      r = int ( random.uniform(50,255))
      x = c.getX()
      y = c.getY()
      cv2.circle(frame,(x,y),5,(b,g,r),-1)
      cv2.putText(frame,(str(c.getLevel())),(c.getX(),c.getY()), font, 0.5,(0,0,255),1)
   # areaT /= len(greatestTissue)
   # cv2.putText(mainFrame,(str(areaT)),(50,50), font, 1,(0,0,255),1,cv2.LINE_AA)
   return frame

def drawLimits(frame,left,right,y):
   font = cv2.FONT_HERSHEY_SIMPLEX
   cv2.line(frame,(left,0),(left,480),(0,200,0),3)
   cv2.line(frame,(right,0),(right,480),(0,200,0),3)
   cv2.line(frame,(0,y),(640,y),(0,200,0),3)

   cv2.putText(frame,("diff L: " + str(left)),(30,20), font, 0.8,(0,0,255),1)
   cv2.putText(frame,("diff R: " + str(640-right)),(30,50), font, 0.8,(0,0,255),1)
   cv2.putText(frame,("diff Top: " + str(y)),(30,80), font, 0.8,(0,0,255),1)

   return frame



