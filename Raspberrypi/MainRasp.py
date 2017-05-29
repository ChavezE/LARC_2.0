import cv2
import numpy as np

import serial
import time

import Larc_vision_2016 as rb
from copy import deepcopy

'''
VARIABLES GLOBALES
'''
#Inicializar el Serial
time.sleep(5);
try:
    arduino= serial.Serial('/dev/ttyUSB0', 9600, timeout=1);
except serial.SerialException:
    print "Desconecta y reconecta el arduino";
    while(True):
        pass;
    
time.sleep(10);

terrinesZone = "c" #Terrines Position; c = unknown, r = right, l = left
cowPos = 0  #Position where we find the Cow; 0 = unknown, 1 = right, 2 = center, 3 = left
cowAngle = 360 #Angle where we find the Cow; 360 = unknown, any other number is the angle
tankPos = 0  #Position where we find the Tank; 0 = unknown, 1 = right, 2 = center, 3 = left
tankAngle = 360 #Angle where we find the Tank; 360 = unknown, any other number is the angle

mainFrame = []

cap = cv2.VideoCapture(0)
'''
    METODOS
'''
###Movimientos

#Turn right the angles in the parameters
def turnRight(angle):
    arduino.write("r") #Send Turn Right angle
    time.sleep(1);
    parte1= (angle>>8)&255;#Upper
    parte2= angle&255;#down
    arduino.write(chr(parte1))
    time.sleep(1);
    arduino.write(chr(parte2))
    time.sleep(1);
    while(arduino.inWaiting() <= 0):
        pass;
    if(arduino.read() == "1"):
        return True;

#Turn left the angles in the parameters
def turnLeft(angle):
    arduino.write("i") #Send Turn Right angle
    time.sleep(1);
    parte1= (angle>>8)&255;#Upper
    parte2= angle&255;#down
    arduino.write(chr(parte1))
    time.sleep(1);
    arduino.write(chr(parte2))
    time.sleep(1);
    while(arduino.inWaiting() <= 0):
        pass;
    if(arduino.read() == "1"):
        return True;

#Turn to an angle
def turnTo(angle):
    arduino.write("g") #Send Turn Right angle
    time.sleep(1);
    parte1= (angle>>8)&255;#Upper
    parte2= angle&255;#down
    arduino.write(chr(parte1))
    time.sleep(1);
    arduino.write(chr(parte2))
    time.sleep(1);
    while(arduino.inWaiting() <= 0):
        pass;
    if(arduino.read() == "1"):
        return True;


#Move Forward till it detect a wall in front
def ForwardTillWall(separation):
    arduino.write("w") #Send Move Forward until Separation
    time.sleep(1);
    arduino.write(chr(separation))
    time.sleep(1);
    while(arduino.inWaiting() <= 0):
        pass;
    if(arduino.read() == "1"):
        return True;

#Move Forward till it detect a wall in front
def BackwardTillWall(separation):
    arduino.write("s") #Send Move Forward until Separation
    time.sleep(1);
    arduino.write(chr(separation))
    time.sleep(1);
    while(arduino.inWaiting() <= 0):
        pass;
    if(arduino.read() == "1"):
        return True;
    

#Move Forward the cms said
def ForwardCms(cant):
    arduino.write("a") #Send Move Forward until Separation
    time.sleep(1);
    arduino.write(chr(cant))
    time.sleep(1);
    while(arduino.inWaiting() <= 0):
        pass;
    if(arduino.read() == "1"):
        return True;
    

#Move Backward the cms said
def BackwardCms(cant):
    arduino.write("b") #Send Move Backward until Separation
    time.sleep(1);
    arduino.write(chr(cant))
    time.sleep(1);
    while(arduino.inWaiting() <= 0):
        pass;
    if(arduino.read() == "1"):
        return True;

#Move foward until ther is not left wall
def fowardUntilNotLeft():
    arduino.write("h");
    time.sleep(1);
    while(arduino.inWaiting()<=0):
        pass;

#Move Backward untile there is no left wall
def BackwardUntilNotLeft():
    arduino.write("y");
    time.sleep(1);
    while(arduino.inWaiting()<=0):
        pass;


#Move foward until ther is not right wall
def fowardUntilNotRight():
    arduino.write("x");
    time.sleep(1);
    while(arduino.inWaiting()<=0):
        pass;

#Move Backward untile there is no right wall
def BackwardUntilNoRight():
    arduino.write("z");
    time.sleep(1);
    while(arduino.inWaiting()<=0):
        pass;

def grabTerrine():
    arduino.write("-");
    time.sleep(1);
    while(arduino.inWaiting() <= 0):
        pass;
    if(arduino.read() == "1"):
        return True;


#####Rutinas
#Go to terrines zone
def goToTerrines():
    turnLeft(90)
    result="n"
    global terrinesZone;
    if(terrinesZone == "c" or terrinesZone == "r"):
        arduino.write("v") 
        time.sleep(1);
        while(True):
            if(arduino.inWaiting()>0):
                result= arduino.read()
                break
        
        #Si agarramos vaso pues nos vamos al centro a buscar vaca, sino pues para ir al otro lado a buscar vasos
        fowardUntilNotLeft();

        if(result=="1"):
            terrinesZone= "r";
            return;            
            

    #No hubo en la derecha o ya sabiamos que es izq
    arduino.write("k") 
    time.sleep(1);
    while(arduino.inWaiting()<=0):
        pass;
    terrinesZone= "l"

    #Nos vamos al centro a buscar vacas siempre
	#BackwardUntilNotLeft();
    

	
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

def walkingDetecting():
	global terrinesZone
	foundCow=False
	missingAngles=0
	startedLeft=0
	pseudoterrines='l'

	#if terrinesZone=='l':
	if pseudoterrines=='l':
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



'''
    MAIN
'''
if __name__ == "__main__":

    goToTerrines();
    walkingDetecting()
    
    

    
    
