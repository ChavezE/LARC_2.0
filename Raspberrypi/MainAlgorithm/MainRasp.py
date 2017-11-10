
# standar libs
import cv2
import numpy as np
import math
import RPi.GPIO as GPIO
import sys
import serial
import time
from copy import deepcopy
import thread

# Roborregos libs
import sys
sys.path.insert(0, '../lib/')
import Larc_vision_2017 as rb
import communication as com






##### Rutinas ######

def goAndGrabTerrine():
    com.goGrabTerrine()

##### Movimientos ######

def turnRight(degrees):
    com.turnNDegrees(degrees,0)

def turnLeft(degrees):
    com.turnNDegrees(degrees,1)

# Checks if the cascade detects a cow, updates maxLenTissue and returns boolean wheather found or not
# Robot must be facing EAST

def searchTank():
    while(1):
        for x in range(0,4):
            com.turnWest()
            for x in range(0,180,45) :
                turnLeft(45)
                time.sleep(2) 
                takePicture()
                found, filtered,left,right,top = rb.detectTank(clearedMainFrame)
                #first validation, haar cascade
                if found:
                    foundTank = rb.isThereATank(filtered)
                    if foundTank:
                        if debugger:
                            cv2.imshow("tank.haar",filtered)
                            cv2.waitKey(0)
                        alignWithTank(left,right)
                        com.goToTank()
                        return
                    else:
                        print "Searching Tank"
            com.turnSouth()
            com.forwardNCm(15)
        print "HAAR DID NOT FOUND TANK"
        
        backwardNCm(15)
        for x in range(0,3):
            com.turnWest()
            for x in range(0,180,45) :
                turnLeft(45)
                time.sleep(2) 
                takePicture()
                found, filtered,left,right,top = rb.detectTank(clearedMainFrame)
                #first validation, haar cascade
                if found:
                    foundTank = rb.isThereATank(filtered)
                    if foundTank:
                        if debugger:
                            cv2.imshow("tank.haar",filtered)
                            cv2.waitKey(0)
                        alignWithTank(left,right)
                        com.goToTank()
                        return
                    else:
                        print "Searching Tank"
            com.turnSouth()
            com.backwardNCm(15)
        print "HAAR DID NOT FOUND TANK"


def checkingTurningR():
    global maxLenTissue

    turnRight(135)

    for x in range(0,95,15) :
        turnLeft(15)
        takePicture()
        found, filtered = rb.detectCow(clearedMainFrame)
        #first validation, haar cascade
        if found:
            print "HAAR FOUND COW"
            # second validation, tissue algorithm
            foundCow,maxLenTissue,_ = rb.isThereACow(mainFrame,filtered)
            if foundCow:
                print "TISSUE FOUND COW"
                L,R,T = rb.calcCowLimits(maxLenTissue)
                dis = rb.getDistanceFromTop(T)
                print "DISTANCE O COW", dis
                # uncomment to show the frame #
                # cv2.imshow("cow",mainFrame)
                # cv2.waitKey(0)
                # cv2.destroyAllWindows()


                success = triangleToGetInCow()
                
                if success:
                        return True

            print "TISSUE DIT NOT FIND COW"
    print "HAAR DID NOT FOUND COW"
    com.turnWest() # WEST
    return False

# Checks if the cascade detects a cow, updates maxLenTissue and returns boolean wheather found or not
# Robot must be facing WEST
def checkingTurningL():
    global maxLenTissue

    turnLeft(135)

    for x in range(0,95,15) :
        turnRight(15)
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


                success = triangleToGetInCow()
                
                if success:
                        return True

            print "TISSUE DIT NOT FIND COW"
    print "HAAR DID NOT FOUND COW"
    com.turnEast()  # EAST
    return False


def walkingDetecting():
    global terrinesZone

    stepping = 30#20
    corner = "EAST"
    foundCow = False
    while foundCow == False:
            if(corner == "EAST"):
                com.turnWest()
                for x in range(3):
                    com.forwardNCm(stepping)
                    foundCow=checkingTurningR()
                    if foundCow:
                        break
                corner = "WEST"
            else:
                com.turnEast()
                for x in range(3):
                    com.forwardNCm(stepping)
                    foundCow=checkingTurningL()
                    if foundCow:
                        break
                corner = "EAST"


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

def alignWithTank(left,right):
    centerFrame = rb.getXCenterFrame(mainFrame)
    
    pixelDif = centerFrame - ((right + left)/2)
    degree = int((abs(pixelDif)/12))
    #Constant obtained throught calibration

    if (pixelDif < -2 ):
        turnRight(degree)

    elif(pixelDif > 2 ):
        turnLeft(degree)

def paralelism():
    tLevel = rb.getTissueTopLevel(maxLenTissue)
    top = rb.getTissueTopLevel(maxLenTissue)
    A,B,theta = rb.ajusteDeCurvas(top)
    rb.drawSlope(mainFrame,A,B)
    if debugger:
        cv2.imshow("slope",mainFrame)
        cv2.waitKey(0)
    degrees = int(abs(theta))

    if theta < -8:
        finalDeg = 90 - (4*degrees)
        if finalDeg < 0:
                finalDeg = 11 
        turnLeft(finalDeg)
        turnedLeft = True
        print finalDeg
    elif theta > 8:
        finalDeg = 90 - (5*degrees)
        if finalDeg < 0:
                finalDeg = 11 
        turnRight(finalDeg)
        turnedLeft = False
        print finalDeg
    else:
        alignWithCow()
        return 0, False

    return finalDeg, turnedLeft

def triangleToGetInCow():
    global maxLenTissue
    L,R,Top = rb.calcCowLimits(maxLenTissue)
    adyacent = rb.getDistanceFromTop(Top)
    print "ADYACENT"
    print adyacent
    if debugger:
        cv2.waitKey(0)
    print "PARALLEL"
    degs, turnedLeft = paralelism()
    print degs
    print "TRIANGLE"
    ninetyDegs = 90 + 7#degs || 15
    ####################################################################################
    ####################################################################################
    ##########################THIS IS VERY IMPORTANT####################################
    ####################################################################################
    # SINCE THE TRIANGLE DOES NOT WORK AS EXPECTED, WE WILL TRY TO JUST ALLIGN WITH,
    # THE COW, IN ORDER TO DO THAT, THE VALUE OF DEGS WILL BE PUT CONSTANT

    degs = 3   #   <-----  THIS IS WHAT I AM TALKING ABOUT


    if degs > 10:
        print "ACTION"
        hypotenuse = (1/math.cos(math.radians(degs))) * adyacent
        print hypotenuse
        if hypotenuse < 0:
                hypotenuse = hypotenuse * -1
                print "HYPOTENUSE CORRECTION"

        if hypotenuse > 100:
                hypotenuse = 100

        
        com.forwardNCm(int(hypotenuse))#com.forwardNCm(int(hypotenuse/2))



        if turnedLeft :
            turnRight(ninetyDegs)
        else:
            turnLeft(ninetyDegs)

        #LETS CONFIRM AGAIN IF THERE IS A COW, 
        #AND THEN ALLIGN TO IT, ELSE LETS RETURN

        com.backwardNCm(30)

        time.sleep(1)

        takePicture()
        if debugger:
            cv2.imshow("second try",mainFrame)
            cv2.waitKey(0)
        found, filtered = rb.detectCow(clearedMainFrame)
        #first validation, haar cascade
        if found:
            # second validation, tissue algorithm
            if debugger:
                cv2.imshow("second try",filtered)
                cv2.waitKey(0)
            foundCow,maxLenTissue,_ = rb.isThereACow(mainFrame,filtered)
            if foundCow:
                if debugger:
                    cv2.imshow("second try",mainFrame)
                    cv2.waitKey(0)
                alignWithCow()

                L,R,Top = rb.calcCowLimits(maxLenTissue)
                distanceFrwrd = rb.getDistanceFromTop(Top)
                com.forwardNCm(int(distanceFrwrd) - 15)
                return True #success
        if debugger:
            cv2.imshow("second try",mainFrame)
            cv2.waitKey(0)
        #If found nothing, lets return
        com.forwardNCm(30)

        if turnedLeft :
            turnLeft(ninetyDegs)
        else:
            turnRight(ninetyDegs)

        com.backwardNCm(int(hypotenuse))

        if turnedLeft :
            turnRight(degs)
        else:
            turnLeft(degs)

        return False

    #IF THE COW IS IN FRONT, LETS JUST MOVE FORWARD ITS DISTANCE
    com.forwardNCm(int(adyacent) - 15)
    return True #success




# Robot is facing south rigth after crossing the to the graval zone
# This function gets the robot in position to drop terrine's milk
def getToTank():
    global mainFrame

    tankContour = rb.getTankCenter(mainFrame)
    x,y,w,h = cv2.boundingRect(tankContour)
    # CENTER TO THE TANK AND THEN GO FOR
    alignWithTank((x+ (x+w))/2)




def control():
    com.controlRobot()








#################################


'''
VARIABLES GLOBALES
'''


#For debugging
debugger = False

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


############### BOTONS INTERRUPT SETUP ####################

START_PIN = 20
END_PIN = 4
LED_PIN = 21

GPIO.setmode(GPIO.BCM)
GPIO.setup(START_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(END_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(LED_PIN, GPIO.OUT)


# setting up interrupt on pin 3 to exit the abort the program
def my_callback(channel):
    print "exiting the program..."
    GPIO.cleanup()
    sys.exit()
# this line waits for the interrupt and calles the function above
GPIO.add_event_detect(END_PIN, GPIO.FALLING, callback=my_callback, bouncetime=1000)

# loop in this function until begin button is pressed
def waitToBegin():
    print "waiting for input"
    GPIO.output(LED_PIN,1)
    GPIO.wait_for_edge(START_PIN, GPIO.FALLING)
    GPIO.output(LED_PIN,0)
    print "begining program"
########################################################


# Global variable for camera
cap = cv2.VideoCapture(0)
# let camara calibrate light
for i in range(10):
        cap.read()

'''
    METODOS
'''
##### Camara ######

# updates mainFrame
def takePicture(inColor = False):
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
        mainFrame = cv2.resize(mainFrame,(640, 480), interpolation = cv2.INTER_CUBIC)
        if(not inColor):
            print "hello"
            clearedMainFrame = rb.clearImage(mainFrame)
        cv2.waitKey(30)
        #cv2.destroyAllWindows()

    else:
        print ("There is an error with the camera")
    return goodFrm




def tLook():
    i = 0
    while True:
        for x in range(0,360,10) :
            takePicture()
            found, filtered = rb.detectCow(clearedMainFrame)
            #first validation, haar cascade
            if found:            
                print "HAAR FOUND COW ON RESTART"
                #turnRight(35)
                i = x
                return i
            else:
                print "HAAR DIT NOT FIND COW ON RESTART"
            turnLeft(10)
#################################
#           RESTART             #
#################################

def turnLooking4cow():
    # turn until you find a cow
    pseudoOriented = ' '

    i = tLook()/10
    # check probabilistic orientation configurations

    # NORTH ?
    if (i >= 0 and i <= 3):
        print "P(pseudoOriented) -> NORTH"
        turnRight(93)
        pass   
    # EAST ?
    elif (i > 3 and i <= 8):
        print "P(pseudoOriented) -> EAST"
        turnRight(55)
        pass
    # SOUTH ?
    elif (i > 8 and i <= 18):
        print "P(pseudoOriented) -> SOUTH"
        turnRight(60)
        pass
    # WEST ? 
    elif (i > 18):
        print "P(pseudoOriented) -> WEST"
        turnRight(60)
        pass

    com.goToRestar()



# READING TRIAL PARAMETERS
tFile = open("../calibration/restart.txt", "r+")
trial = (tFile.readline().strip(" \n"))
print trial
try: 
    numberOfTrial = int(trial[len(trial)-1])
except ValueError:
    print "Error en el archivo de restart"
    while True:
        pass


# update trial
linesToWrite = [" "+str(numberOfTrial + 1)]
tFile.writelines(linesToWrite)
tFile.close()

restarted = False
if (numberOfTrial > 0):
    restarted = True
    print "Restart, looking for a cow"
    waitToBegin()
    turnLooking4cow()

'''
    MAIN
'''

if __name__ == "__main__":

    if (not restarted):
        waitToBegin()
    # Robot always STARTS facing NORTH, check field in 'information' folder #

    #goAndGrabTerrine()

    # STARTING EXPLORTION HERE #
    #com.turnWest()
    #turnRight(90)
    #walkingDetecting()
    #com.getInCow()
    #cv2.waitKey(0)
    #com.milk()
    #com.goToStart()

    #searchTank()



    #NO MORE TESTING!! THIS IS COMPETITION
    goAndGrabTerrine()
    walkingDetecting()
    com.getInCow()
    com.milk()
    com.goToStart()
    searchTank()

    
    
    while True:
        print "code"
        pass



    # takePicture()
    # getToTank()
    # com.forwardNCm(200)


