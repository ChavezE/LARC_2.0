## This is version 3.0 of LARC 2017 Module of Mexican Team RoBorregos
## which stores functions used by the computer vision squad.
## All rights reserved by Instituo Tecnologico de Monterrey.

## AUTHORS:    Emilio Chavez Madero
##             Alejandro Garza Castro


#############################################
##----------------MAIN LIBS----------------##
#############################################
import cv2
import time
import math
import numpy as np
import random
from copy import deepcopy


#-------------------GLOBAL FOR CALIBRATION-------------------
#Cow square area  #si detecta muchos cuadros
maxSquareArea = 1000
minSquareArea = 100
#Thresh range for cow squares  #si no detecta suficientes cuadros
minThresh = 50
maxThresh = 175
steps = 5
#Tissue Parameters
eps = 30
eps2 = 30
#HAAR Cascade Sansitivity
cascadeSensitivity = 50

#----HAAR Cascade---
#importing the trained cascade of cow
cowCascade = cv2.CascadeClassifier('../Cascades/COW1.xml')
#using a black frame to filter
blackFrame = cv2.imread("../images/white.jpg",0)


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


#############################################
##------------- IMAGE FUNCTIONS------------##
#############################################
# To work over the imageS to get them ready, obatain
# general data and use it in further algorithms

# Does some guassian filtering to remove noise and converts image to gray scale
def clearImage(imgOriginal):

   imGray = cv2.cvtColor(imgOriginal, cv2.COLOR_BGR2GRAY)
   imGray = cv2.GaussianBlur(imGray, (3,3), 2)
   imGray = cv2.equalizeHist(imGray)
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
def getGoodSquares(contours,thres,mainC):

   # ----VARIABLES----
   cowSquares = []   # This list is the one that is going to being returned
   # -----------------
   final_contours=[]

   for cnt in contours:
      area = cv2.contourArea(cnt)
      rect = cv2.minAreaRect(cnt)
      w = int(rect[1][0])
      h = int(rect[1][1])
      rect_area = w * h


      # cv2.drawContours(mainC,[cnt],-1,(0,255,0),1)
      # print area
      # cv2.imshow("individual: " ,mainC)

      if(rect_area > 0): # sometimes this value is found
         extent = float(area / rect_area)
         if (extent >= 0.75 and area >= minSquareArea and area <= maxSquareArea):   # tolerance #previos range: 400-8500
            x,y,w,h = cv2.boundingRect(cnt)
            if thres[y + h*0.5,x + w*0.5] == 1.0 and w/h < 3 and h/w < 3:
               tempCowSquare = cowSquare(x,y,w,h,area)    # Create an objet from the 'cowSquare' class
               cowSquares.append(tempCowSquare) # Insert object 'cowSquare' into a list
               cv2.drawContours(mainC,[cnt],-1,(255,0,0),1)
               final_contours.append(cnt)

   return cowSquares,final_contours

# def getGoodSquares(contours,thres,mainC):

#    # ----VARIABLES----
#    cowSquares = []   # This list is the one that is going to being returned
#    # -----------------

#    for cnt in contours:
#       area = cv2.contourArea(cnt)
#       rect = cv2.minAreaRect(cnt)
#       w = int(rect[1][0])
#       h = int(rect[1][1])
#       rect_area = w * h
#       final_contours=[]

#       cv2.drawContours(mainC,[cnt],-1,(0,255,0),1)
#       # print area
#       cv2.imshow("individual: " ,mainC)
#       # cv2.waitKey(200)
#       if(rect_area > 0): # sometimes this value is found
#          extent = float(area / rect_area)
#          if (extent >= 0.75 and area >= minSquareArea and area <= maxSquareArea):   # tolerance #previos range: 400-8500
#             x,y,w,h = cv2.boundingRect(cnt)
#             if thres[y + h*0.5,x + w*0.5] == 1.0 and w/h < 1.5:
#                tempCowSquare = cowSquare(x,y,w,h,area)    # Create an objet from the 'cowSquare' class
#                cowSquares.append(tempCowSquare) # Insert object 'cowSquare' into a list
#                cv2.drawContours(mainC,[cnt],-1,(255,0,0),2)
#                final_contours.append(cnt)
#    return cowSquares,final_contours






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

# Returns distance between two point in the image.
def distance(x1,y1,x2,y2):
  return math.sqrt(pow(x2 - x1,2) + pow(y2 - y1,2))


#############################################
##------------ DESSISION MAKING------------##
#############################################

#tissue updates
def doTissue(goodSqrs):
   # setting constants #
   tissue = []
   biggestTissue = []

   print("buling form now on")
   goodSqrs = sorted(goodSqrs, key=lambda x:x.getY(), reverse = False)
   while( len(goodSqrs) > 0):
      tActSqr = goodSqrs.pop(0)
      tissue.append(tActSqr)
      makeTissue(tActSqr,goodSqrs,tissue,eps,0)

      if(len(tissue) > len(biggestTissue)):
         biggestTissue = deepcopy(tissue)
      tissue[:] = []

   print ("im done with the tissue")
   return biggestTissue

# build the tissue
def makeTissue(tActSqr,tAllSqrs,tissue,eps,lvl):

   found = False
   for tSq in (tAllSqrs):

      # UPPER
      if (distance( tActSqr.getTopLeftC()[0], tActSqr.getTopLeftC()[1] - 2*tActSqr.getH(), tSq.getTopLeftC()[0], tSq.getTopLeftC()[1]) < eps2):
         tSq.setLevel(lvl+2)
         tissue.append(tSq)
         tAllSqrs.pop(tAllSqrs.index(tSq))
         found = True
         makeTissue(tissue[len(tissue)-1],tAllSqrs,tissue,eps,lvl+2)

      # LOWER
      elif (distance( tActSqr.getTopLeftC()[0], tActSqr.getTopLeftC()[1] + 2*tActSqr.getH(), tSq.getTopLeftC()[0], tSq.getTopLeftC()[1]) < eps2):
         tSq.setLevel(lvl-2)
         tissue.append(tSq)
         tAllSqrs.pop(tAllSqrs.index(tSq))
         found = True
         makeTissue(tissue[len(tissue)-1],tAllSqrs,tissue,eps,lvl-2)

      # RIGHT
      elif (distance( tActSqr.getTopLeftC()[0] + 2*tActSqr.getW(), tActSqr.getTopLeftC()[1], tSq.getTopLeftC()[0], tSq.getTopLeftC()[1]) < eps2):
         tSq.setLevel(lvl)
         tissue.append(tSq)
         tAllSqrs.pop(tAllSqrs.index(tSq))
         found = True
         makeTissue(tissue[len(tissue)-1],tAllSqrs,tissue,eps,lvl)

      # LEFT
      elif (distance( tActSqr.getTopLeftC()[0] - 2*tActSqr.getW(), tActSqr.getTopLeftC()[1], tSq.getTopLeftC()[0], tSq.getTopLeftC()[1]) < eps2):
         tSq.setLevel(lvl)
         tissue.append(tSq)
         tAllSqrs.pop(tAllSqrs.index(tSq))
         found = True
         makeTissue(tissue[len(tissue)-1],tAllSqrs,tissue,eps,lvl)

      # UPPER RIGHT
      elif (distance( tActSqr.getTopLeftC()[0] + tActSqr.getW(), tActSqr.getTopLeftC()[1], tSq.getTopLeftC()[0], tSq.getTopLeftC()[1] + tSq.getH()) < eps):
         tSq.setLevel(lvl+1)
         tissue.append(tSq)
         tAllSqrs.pop(tAllSqrs.index(tSq))
         found = True
         makeTissue(tissue[len(tissue)-1],tAllSqrs,tissue,eps,lvl+1)

      # UPPER LEFT
      elif (distance( tActSqr.getTopLeftC()[0], tActSqr.getTopLeftC()[1], tSq.getTopLeftC()[0] + tSq.getW(), tSq.getTopLeftC()[1] + tSq.getH()) < eps):
         tSq.setLevel(lvl+1)
         tissue.append(tSq)
         tAllSqrs.pop(tAllSqrs.index(tSq))
         found = True
         makeTissue(tissue[len(tissue)-1],tAllSqrs,tissue,eps,lvl+1)

      # LOWER RIGHT
      elif (distance( tActSqr.getTopLeftC()[0]+tActSqr.getW(), tActSqr.getTopLeftC()[1] + tActSqr.getH(), tSq.getTopLeftC()[0] , tSq.getTopLeftC()[1] ) < eps):
         tSq.setLevel(lvl-1)
         tissue.append(tSq)
         tAllSqrs.pop(tAllSqrs.index(tSq))
         found = True
         makeTissue(tissue[len(tissue)-1],tAllSqrs,tissue,eps,lvl-1)

      # LOWER LEFT
      elif (distance( tActSqr.getTopLeftC()[0], tActSqr.getTopLeftC()[1]+ tActSqr.getH(), tSq.getTopLeftC()[0] + tSq.getW(), tSq.getTopLeftC()[1] ) < eps):
         tSq.setLevel(lvl-1)
         tissue.append(tSq)
         tAllSqrs.pop(tAllSqrs.index(tSq))
         found = True
         makeTissue(tissue[len(tissue)-1],tAllSqrs,tissue,eps,lvl-1)

   if found == False:
      tissue.pop(tissue.index(tActSqr))


# returns a Tissue compossed by cow squares if found, if not empty list
def isThereACow(mainFrame, equalizedFrame):
   maxLenT = [] # maximumLenghtTissue
   allSquares = [] # Store in each iteration of the binarization the squares found in the image
   minNumSquares = 4
   # iterate to get max squares from the image
   # best way so far to counter ligh strokes

   print "starting bin values"

   for binValueT in range(minThresh,maxThresh,steps):

      cp0 = cp1 = cp2 = deepcopy(equalizedFrame)
      # main_copy2=mainFrame.copy()

      thresFrame0 = doThresHold(cp0, binValueT,7,1)
      #cv2.imshow("thres0: ", thresFrame0)
      #cv2.waitKey(150)
      contours0 = findContours(thresFrame0)
      cowRectangles0,_ = getGoodSquares(contours0,thresFrame0,mainFrame)
      findEquals(allSquares,cowRectangles0,15)

      thresFrame1 = doThresHold(cp1, binValueT,3,3) # Thresholds the image and erodes it
      contours1 = findContours(thresFrame1) # Finds all the contours inside the image
      cowRectangles1,_ = getGoodSquares(contours1,thresFrame1,mainFrame) # From contours, extract possile cow squares
      findEquals(allSquares,cowRectangles1,15)

      thresFrame2 = doThresHold(cp2, binValueT,5,2)
      contours2 = findContours(thresFrame2)
      cowRectangles2,_ = getGoodSquares(contours2,thresFrame2,mainFrame)
      findEquals(allSquares,cowRectangles2,15)

      del cp0
      del cp1
      del cp2

   print "donde with bin values"
   if len(allSquares) > minNumSquares:
      tempAllSquares = deepcopy(allSquares)
      maxLenT = doTissue(tempAllSquares)
      for sqr in maxLenT:
         cv2.rectangle(mainFrame, (sqr.getTopLeftC()[0],sqr.getTopLeftC()[1]), (sqr.getBotRightC()[0],sqr.getBotRightC()[1]), (0,255,0), 2)
      #    cv2.imshow("squares of " + str(binValueT),main_copy2)
      #    cv2.waitKey(5)

      if len(maxLenT) > minNumSquares:
         return True,maxLenT,allSquares

   return False,[],[]

#Implementation of the trained cascade classifier,
#trained to detect the COW pattern
def filterForCow(img):
   blackTemp = blackFrame.copy()

   cows = cowCascade.detectMultiScale(img, 1.3, cascadeSensitivity)

   xc,yc,hc,wc = 0,0,0,0
   individualCow = []

   cowDetected = False

   if cows is ():
      print "Nada detectado"
      return cowDetected, img

   for (x,y,w,h) in cows:

      relation = float(h)/w
      area = (float(w)*h)

     #ampliation is calibratable,

     #determines how much the
     #detected area expands
      ampliation = (2 * (area/10000))

     #This condition is also calibratable, by determining
     #a relationship between h/w
      if relation < 0.77 and relation > 0.74 and area > 11000 and w > 120:
         #cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,255),2)

         #Expanding the detected area
         xc = x - int(.5 * ampliation)
         yc = y - int(1 * ampliation)
         hc = h + int((3 * ampliation))
         wc = w + int((1 * ampliation))
         cowDetected = True

         cv2.rectangle(img,(xc,yc),(xc+wc,yc+hc),(0,255,0),1)
         #cv2.imshow("img",img)


   individualCow = img[yc:yc+hc,xc:xc+wc]
   print individualCow.shape
   blackTemp[yc:yc+hc,xc:xc+wc] = individualCow
   print blackTemp.shape

   cv2.imshow("with black", blackTemp)
   return cowDetected, blackTemp



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
   return distance(x1,y1,x2,y2)

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
   # theta,A,B = ajusteDeCurvas(listMaxLevel)
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


   # IN: Frame is grayscale image, n is num of corners, quality is 0-1

def getTissueTopLevel(tissue):

   tissue = sorted(tissue, key=lambda x:x.getLevel(), reverse = True)
   minLength = 3
   validLevel = False
   levelTissue = []

   currLevel = tissue[0].getLevel()
   levelTissue.append(tissue[0])
   for x in range(len(tissue)):
      if currLevel == tissue[x].getLevel():
         levelTissue.append(tissue[x])
      else:
         if len(levelTissue) >= minLength:
            break
         currLevel = tissue[x].getLevel()
         levelTissue = []
         levelTissue.append(tissue[x])


   return levelTissue


#############################################
##-----------SECONDARY FUNCTIONS-----------##
#############################################

# This functions sorts a multivaraible list depending on the index specified
def sortList(index,l):

   l = sorted(l, key=lambda x:x[index], reverse=False)

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
