# standar libs
import cv2
import numpy as np
import time
import math
import glob
from copy import deepcopy
import subprocess

# Roborregos libs
from calibrateDistance import runCalib
import sys
sys.path.insert(0, '../lib/')
import Larc_vision_2017 as rb



mainFrame = []
clearedMainFrame = []

cap = cv2.VideoCapture(0)
# let camara calibrate light
for i in range(10):
        cap.read()

# updates mainFrame
def takePicture():
	global mainFrame
	global clearedMainFrame
	# clear internal buffer
	for i in range(4):
		cap.grab()
	# get new image
	goodFrm, mainFrame = cap.read()
	print "I took a pic"
	if goodFrm:
		clearedMainFrame = rb.clearImage(mainFrame)

	else:
		print ("There is an error with the camera")
	return goodFrm

def getPhotos():
	global mainFrame
	print "Iniciating distance calib \n \n "

	ans = ""
	while (ans != "q"):
		_ = raw_input("Press any key to take photo ")
		takePicture()
		cv2.imshow('frame',mainFrame)
		cv2.waitKey(500)
		k = raw_input("press any key to continue or \'r\' to retake")
		cv2.destroyWindow('frame')
		if k == 'r':
			continue

		distance = raw_input("type the distance in cm of the current photo:  ")
		distance += "_cm.jpg"
		path = "/distance_data/" + distance
		cv2.imwrite("distance_data/" +distance,mainFrame)

		ans = raw_input("press \'q\' to finish or any key to keep adding photos")

	print "Generating linear regresion..."
	runCalib()
	print "...Success!"


def emptyFolder():
	bashCommand = "rm distance_data/*.jpg"
	output = subprocess.check_output(['bash','-c', bashCommand])


#emptyFolder()
getPhotos()
