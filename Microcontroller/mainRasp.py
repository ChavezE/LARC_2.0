#Standard libraries
import cv2
import numpy as numpy
import serial
import time
fromcopy import deepcopy

import Larc_vision_2017 as rb
import communication as

#Global Variables

time.sleep(5);
try:
	arduino = serial.Serial('/dev/ttyUSB0', 9600, timeout = 1);
except serialSerialException:
	print("Desconecta y reconecta el arduino");
	while(True):
		pass;

time.sleep(10);

#Terrines Position; c = unknown, r = right, l = left
terrinesZone = "c"

#Position where we find the Cow; 0 = unknown, 1 = right, 2 = center, 3 = left 
cowPos = 0 

#Angle where we find the Cow; 360 = unknown, any other number is the angle
cowAngle = 360

#Position where we find the Tank; 0 = unknown, 1 = right, 2 = center, 3 = left 
tankPos = 0  

#Angle where we find the Tank; 360 = unknown, any other number is the angle
tankAngle = 360 

mainFrame = []

cap = cv2.VideoCapture(0)

if __name__ == "__main__":

	arduino.write("q");
	time.sleep(1);
	while(arduino.inWaiting() < = 0):
		pass;
	print(arduino.read());