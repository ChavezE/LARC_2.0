
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
clearedMainFrame = []
maxLenTissue = []

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
    global clearedMainFrame
    # clear internal buffer
    for i in range(4):
        cap.grab()
    # get new image
    goodFrm, mainFrame = cap.read()
    print "I took a pic"
    if goodFrm:
        #cv2.imshow('main', mainFrame)
        clearedMainFrame = rb.clearImage(mainFrame)
        cv2.waitKey(30)
        #cv2.destroyAllWindows()

    else:
        print ("There is an error with the camera")
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

# Checks if the cascade detects a cow, updates maxLenTissue and returns boolean wheather found or not
# Robot must be facing EAST
def checkingTurningR():
    global maxLenTissue

    turnRight(135)

    for x in range(0,95,5) :
        turnLeft(5)
        takePicture()
        found, filtered = rb.detectCow(clearedMainFrame)
        #first validation, haar cascade
        if found:
            print "HAAR FOUND COW"
            # second validation, tissue algorithm
            foundCow,maxLenTissue,_ = rb.isThereACow(mainFrame,filtered)
            if foundCow:
                print "TISSUE FOUND COW"
                # uncomment to show the frame #
                # cv2.imshow("cow",mainFrame)
                # cv2.waitKey(0)
                # cv2.destroyAllWindows()
                return True

            print "TISSUE DIT NOT FIND COW"
    print "HAAR DID NOT FOUND COW"
    turnLeft(45)
    return False

# Checks if the cascade detects a cow, updates maxLenTissue and returns boolean wheather found or not
# Robot must be facing WEST
def checkingTurningL():
    global maxLenTissue

    turnLeft(135)

    for x in range(0,95,5) :
        turnRight(5)
        takePicture()
        found, filtered = rb.detectCow(clearedMainFrame)
        #first validation, haar cascade
        if found:
            # second validation, tissue algorithm
            foundCow,maxLenTissue,_ = rb.isThereACow(mainFrame,filtered)
            if foundCow:
                print "TISSUE FOUND COW"
                # uncomment to show the frame #
                # cv2.imshow("cow",mainFrame)
                # cv2.waitKey(0)
                # cv2.destroyAllWindows()
                return True

            print "TISSUE DIT NOT FIND COW"
    print "HAAR DID NOT FOUND COW"
    turnRight(45)
    return False


def walkingDetecting():
    global terrinesZone

    stepping = 75
    corner = "WEST"
    foundCow = False
    while foundCow == False:
            if(starting == "WEST"):
                for x in range(3):
                    foundCow=checkingTurningR()
                    if foundCow:
                        break
                    ForwardCms(stepping)
                corner = "EAST"
            else:
                for x in range(3):
                    foundCow=checkingTurningR()
                    if foundCow:
                        break
                    ForwardCms(stepping)
                corner = "WEST"

# def walkingDetecting():
#     global terrinesZone
#     foundCow=False
#     missingAngles=0
#     startedLeft=0
#     pseudoterrines='l'
#
#     if pseudoterrines=='l':
#         startedLeft=True
#         BackwardCms(75)
#     else:
#         ForwardCms(75)
#         turnRight(180)
#
#     while foundCow == False:
#         if startedLeft == True:
#
#             foundCow=checkingTurningR()
#             if foundCow:
#                 break
#
#             ForwardCms(75)
#             foundCow=checkingTurningL()
#             if foundCow:
#                 break
#
#
#             BackwardCms(75)
#             foundCow=checkingTurningR()
#             if foundCow:
#                 break
#
#
#         foundCow=checkingTurningL()
#         if foundCow:
#             break
#
#         ForwardCms(75)
#         foundCow=checkingTurningR()
#         if foundCow:
#             break
#
#         BackwardCms(75)
#         foundCow=checkingTurningL()
#         startedLeft=True

def alignWithCow():
    centerFrame = rb.getXCenterFrame(mainFrame)
    cowCenter = rb.getCowXCenter(maxLenTissue)
    pixelDif = centerFrame - cowCenter
    degree = abs(pixelDif)/12
    #Constant obtained throught calibration

    if (pixelDif < -2 ):
        turnRight(degree)

    elif(pixelDif > 2 ):
        turnLeft(degree)



'''
    MAIN
'''
if __name__ == "__main__":

    # Robot always STARTS facing NORTH, check field in 'information' folder #

    # goToTerrines()
    # getTerrines()

    # STARTING EXPLORTION HERE
    turnLeft(90)
    walkingDetecting()
    alignWithCow()
    com.forwardNCm(100)

    # print found
    # if (found):
    #     print "ALINEANDOSE"
    #     alignWithCow()
    #     print "ALINEADO TERMINADO"
    #     com.forwardNCm(100)
    # cv2.destroyAllWindows()
