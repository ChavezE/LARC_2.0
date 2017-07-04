
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
# let camara calibrate light
for i in range(10):
		cap.read()

'''
	METODOS
'''
##### Camara ######

# updates mainFrame
def takePicture():
	global mainFrame
	# clear internal buffer
	for i in range(4):
		cap.grab()
	# get new image
	goodFrm, mainFrame = cap.read()
	print "I took a pic"
	cv2.imshow('main', mainFrame)
	cv2.waitKey(0)
	cv2.destroyAllWindows()
	return goodFrm


##### Rutinas ######

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

def getTerrines():
	com.goGrabTerrine()

##### Movimientos ######

def turnRight(degrees):
	com.turnNDegrees(degrees,0)

def turnLeft(degrees):
	com.turnNDegrees(degrees,1)

def checkingTurningR():
	turnRight(50)
	for x in range(1,3):
		turnRight(15)
		missingAngles=((3-x)*15)+75
		takePicture()


		clearedFrame = rb.clearImage(mainFrame)
		# validation2, filtered = rb.filterForCow(clearedFrame)
		validation,maxLenT,_ = rb.isThereACow(mainFrame,clearedFrame)
		if validation:
			print "COW FOUND"
			tLevel = rb.getTissueTopLevel(maxLenT)
			rb.drawCowSquares(mainFrame,100,100,100,tLevel)
			A,B,theta = rb.ajusteDeCurvas(tLevel)
			rb.drawSlope(mainFrame,A,B)
			left,right,up=getLimits(maxLenT)

			cowCenter = getCowXcenter(left,right)
			frameCenter = getXCenterFrame()

			dg = abs(cowCenter - frameCenter) / 12
			print "degrees phase :", dg
			print "centering the cow..."
			if cowCenter > frameCenter:
				# cow is at right
				com.turnNDegrees(dg,0)
			else:
				# cow is at left
				com.turnNDegrees(dg,1)
			print "centered"
			com.getInCow()

		if validation:
			print "lo encontro"
			break

	#turnRight(missingAngles)
	return validation

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
	getTerrines()
	com.turnNDegrees(90,0)
	res = checkingTurningR()
	print res
