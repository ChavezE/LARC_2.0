import serial
import time
import cv2

try:
        arduino = serial.Serial('/dev/ttyACM0', 9600, timeout = 1)
	time.sleep(3)
except serial.SerialException:
        print("bad port, check labeling");
        while(True):
                pass;

def forwardNCm(cm):
        print("forwardNCm")
        arduino.write("a")
        #Send the speed to turn
        arduino.write(chr(cm))
        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

def backwardNCm(cm):
        print("backwardNCm")
        arduino.write("b")
        #Send the speed to turn
        arduino.write(chr(cm))
        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

def forwardUntilWallN(cm):
        print("forwardUntilWallN")
        arduino.write("c")
        #Send the speed to turn
        arduino.write(chr(cm))
        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

def backwardUntilWallN(cm):
        print("backwardUntilWallN")

        arduino.write("d")

        #Send the speed to turn
        arduino.write(chr(cm))

        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;

        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

def turnToObjective(angle):
        print("turnToObjective")

        #Tell arduino to run turn()
        arduino.write("e")

        part1 = (angle>>8)&255
        part2 = angle&255

        arduino.write(chr(part1))
        arduino.write(chr(part2))

        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

#direction = 0 right, 1 = left
def turnNDegrees(angle, direction):
        print("turnNDegrees")

        #Send the case for forward
        arduino.write("f")

        #Send velocity for left motor
        arduino.write(chr(angle))
        #send velocity for right motor
        arduino.write(chr(direction))

        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        #Completed
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

def forwardUntilNoRight():
        print("forwardUntilNoRight")
        #Tell the arduino to run brake()
        arduino.write("g")

        #Wait for something to happen
        while(arduino.inWaiting() <= 0):
                pass;
        #Completed
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

def forwardUntilNoLeft():
        print("forwardUntilNoLeft")
        #Tell the arduino to run brake()
        arduino.write("h")

        #Wait for something to happen
        while(arduino.inWaiting() <= 0):
                pass;
        #Completed
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

def backwardUntilNoRight():
        print("backwardUntilNoRight")
        #Tell the arduino to run brake()
        arduino.write("i")

        #Wait for something to happen
        while(arduino.inWaiting() <= 0):
                pass;
        #Completed
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

def backwardUntilNoLeft():
        print("backwardUntilNoLeft")
        #Tell the arduino to run brake()
        arduino.write("j")

        #Wait for something to happen
        while(arduino.inWaiting() <= 0):
                pass;
        #Completed
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

def getSharpDistance(num):
    print("getSharpDistance")
    arduino.write("k")
    arduino.write(num)
    # wait for arduino response
    while (arduino.inWaiting() <= 0):
        pass
    cm = arduino.readline()
    print (cm)

def goGrabTerrine():
        print("GoGrabTerrine")
        #Tell the arduino to run goGrabTerrines()
        arduino.write("l")
        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;

        #Completed
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

def getInCow():
        print("getInCow")
        #Tell the arduino to run goGrabTerrines()
        arduino.write("m")
        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;

        #Completed
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

#Turn to the North
def turnNorth():
    print("Turn North")
    #Tell the arduino to turn
    arduino.write("n")
    #Tell the arduino to turn to the north
    arduino.write("n")
    #Wait for arduino response
    while(arduino.inWaiting() <= 0):
            pass;
    #Completed
    if(arduino.read() == "1"):
            print("True")
            return True
    else:
            print("False")
            return False

#Turn to the East
def turnEast():
    print("Turn East")
    #Tell the arduino to turn
    arduino.write("n")
    #Tell the arduino to turn to the east
    arduino.write("e")
    #Wait for arduino response
    while(arduino.inWaiting() <= 0):
            pass;
    #Completed
    if(arduino.read() == "1"):
            print("True")
            return True
    else:
            print("False")
            return False

#Turn to the South
def turnSouth():
    print("Turn South")
    #Tell the arduino to turn
    arduino.write("n")
    #Tell the arduino to turn to the South
    arduino.write("s")
    #Wait for arduino response
    while(arduino.inWaiting() <= 0):
            pass;
    #Completed
    if(arduino.read() == "1"):
            print("True")
            return True
    else:
            print("False")
            return False

#Turn to the West
def turnWest():
    print("Turn West")
    #Tell the arduino to turn
    arduino.write("n")
    #Tell the arduino to turn to the West
    arduino.write("w")
    #Wait for arduino response
    while(arduino.inWaiting() <= 0):
            pass;
    #Completed
    if(arduino.read() == "1"):
            print("True")
            return True
    else:
            print("False")
            return False

#Control the robot with the keyboard
def controlRobot():
    print "Control Robot"
    while(True):
        pressKey = cv2.waitKey(250)
        cv2.imshow("img", cv2.imread("../images/black.jpg"))
        #w key
        if (w == 119):
            forwardNCm(1)
        #a key
        elif (a == 97):
            turnNDegrees(1 , 1)
        #s key
        elif (s == 115):
            backwardNCm(1)
        #d key
        elif(d == 100):
            turnNDegrees(1 , 0)
        #i key
        elif(i == 105):
            pass
        #j key
        elif(j == 106):
            pass
        #k key
        elif(k == 108):
            pass
        #l key
        elif(l == 107):
            pass
        #esc key
        elif(esc == 27):
            break