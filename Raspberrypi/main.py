import serial #Serial for communicate with arduino
import time

arduino = serial.Serial('/dev/ttyACM0',9600, timeout = 1) #Serial communication with arduino at 9600 bauds
terrinesZone = c #Terrines Position; c = unknown, r = right, l = left
cowPos = 0  #Position where we find the Cow; 0 = unknown, 1 = right, 2 = center, 3 = left
cowAngle = 360 #Angle where we find the Cow; 360 = unknown, any other number is the angle
tankPos = 0  #Position where we find the Tank; 0 = unknown, 1 = right, 2 = center, 3 = left
tankAngle = 360 #Angle where we find the Tank; 360 = unknown, any other number is the angle

#Check availibility of the Arduino
def checkForArduino():
	# Send a 'b' to tell the Arduino the RaspberryPi is alive
	# print "Conecting with arduino"
	time.sleep(3)
	arduino.write("b")
	ans = wait4ArduAnswer()
	# print "Time to boot arduino: ", (time.time() - start)
	print "Arduino is alive",ans
	return True

#Turn right the angles in the parameters
def turnRight(angle):
    arduino.write("r {}".format(angle)) #Send Turn Right angle
    while(True):
        if(arduino.read() == "0"):
            break

#Turn left the angles in the parameters
def turnLeft(angle):
    arduino.write("l {}".format(angle)) #Send Turn Left angle
    while(True):
        if(arduino.read() == "0"):
            break

#Move Forward till it detect a wall in front
def ForwardTillWall(separation):
    arduino.write("r {}".format(separation)) #Send Move Forward until Separation
    while(True):
        if(arduino.read() == "0"):
            break

#Return True if the robot is in the terrines zone, False if not
def confirmTerrineZone():

    arduino.write("z {}".format(separation)) #Send confirmTerrineZone
    response = ""

    while(True):
        response = arduino.read() #Save Response
        if(response != ""):
            break

    if(response == "0"): #The arduino tell us we are not in Terrine Zone
        return False
    if(response == "1"): #The arduino tell us we are in Terrine Zone
        return True

#Go to terrines zone
def goToTerrines():
    turnRight(90)
    ForwardTillWall()
    if(confirmTerrineZone()):
        terrinesZone = r #Save that the Terrine Zone is at right
        return True
    else:
        turnRight(180)
        ForwardTillWall()
        if(confirmTerrineZone()):
            terrinesZone = l #Save that the Terrine Zone is at left
            return True

    return False

#Grab the terrine
def grabTerrine():
    arduino.write("t") #Send grab terrine
    while(True):
        if(arduino.read() == "0"):
            break

#Look for cow and fill cowPos and cowAngle variables
def lookForCow():
    return

#Get next to cow
def goToCow():
    return

#Get inside the cow
def getInCow():
    return

#milk the cow
def milk():
    return

#Get in Gravel zone
def goToGravel():
    return

#Look for Tank and fill tankPos and tankAngle variables
def lookForTank():
    return

#getNext to the Tank
def goToTank():
    return

#Throw Terrine inside the Tank
def throwTerrine():
    return

#Get to initial position
def getToInit():
    return

if __name__ == "__main__":
    while(!checkForArduino()):
        time.sleep(1)

    while(True):
        completed = goToTerrines()
        if(!completed):
            pass
            break
        completed = grabTerrine()
        if(!completed):
            pass
            break
        completed = lookForCow()
        if(!completed):
            pass
            break
        completed = goToCow()
        if(!completed):
            pass
            break
        completed = getInCow()
        if(!completed):
            pass
            break
        completed = milk()
        if(!completed):
            pass
            break
        completed = goToGravel()
        if(!completed):
            pass
            break
        completed = lookForTank()
        if(!completed):
            pass
            break
        completed = goToTank()
        if(!completed):
            pass
            break
        completed = throwTerrine()
        if(!completed):
            pass
            break
        completed = getToInit()
        if(!completed):
            pass
            break
