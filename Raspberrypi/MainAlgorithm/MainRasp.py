
# standar libs
import cv2
import numpy as np
import serial
import time
from copy import deepcopy

# Roborregos libs
import Larc_vision_2017 as rb
import communication as com

'''
VARIABLES GLOBALES
'''

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
	centerFrame=getXCenterFrame()
	
	cowCenter = getCowXCenter(maxLenTissue)
	#degrDif= abs(cowCenter - centerFrame)

	if (cowCenter < centerFrame - 2):
		#gira a la izquierda x grados
		pass
	elif(cowCenter > centerFrame + 2):
		#gira a la derecha x grados
		pass



'''
    MAIN
'''
if __name__ == "__main__":
	pass