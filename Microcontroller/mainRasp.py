#Standard libraries
import cv2
import numpy as numpy
import serial
import time
from copy import deepcopy

import communication as com

#Global Variables
time.sleep(5);
try:
        arduino = serial.Serial('/dev/ttyUSB0', 9600, timeout = 1);
except serial.SerialException:
        print("Desconecta y reconecta el arduino");
        while(True):
                pass;

time.sleep(10);

########################################
#This should be in communication file

#Stop the robot
def brake():
        print("Brake")
        #Tell the arduino to run brake()
        arduino.write("a")
        time.sleep(1);
        #Wait for something to happen
        while(arduino.inWaiting() <= 0):
                pass;

        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

        print("No Brake")

#Go Forward, the parameters are the velocity of the motors
def forward(left, right):
        print("forward")
        #Send the case for forward
        arduino.write("b")
        time.sleep(1)
        #Send velocity for left motor
        arduino.write(chr(left))
        time.sleep(1)
        #send velocity for right motor
        arduino.write(chr(right))
        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

#Go backward, the parameters are the velocity of the motors
def backward(left, right):
        print("forward")
        #Send the case for forward
        arduino.write("c")
        time.sleep(1)
        #Send velocity for left motor
        arduino.write(chr(left))
        time.sleep(1)
        #send velocity for right motor
        arduino.write(chr(right))
        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False


#Turn right to an exact angle
def turnRight(angle):
        print("forward")
        #Send the case for forward
        arduino.write("d")
        time.sleep(1)
        #Send velocity for left motor
        arduino.write(chr(angle))
        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

#Turn left to an exact angle
def turnLeft(angle):
        print("forward")
        #Send the case for forward
        arduino.write("e")
        time.sleep(1)
        #Send velocity for left motor
        arduino.write(chr(angle))
        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

#Turn to an exact angle
def turn(angle):
        print("forward")
        #Send the case for forward
        arduino.write("f")
        time.sleep(1)
        #Send velocity for left motor
        arduino.write(chr(angle))
        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True
        else:
                print("False")
                return False

#Go Forward an x amount of cm
def forwardNCm(cm):
        print("forward")
        #Send the case for forward
        arduino.write("g")
        time.sleep(1)
        #Send velocity for left motor
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

#Go backward an x amount of cm
def backwardNCm(cm):
        print("forward")
        #Send the case for forward
        arduino.write("h")
        time.sleep(1)
        #Send velocity for left motor
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

#Go Forward until it found a wall at a certain distance
def forwardUntilWall(cm):
        print("forward")
        #Send the case for forward
        arduino.write("i")
        time.sleep(1)
        #Send velocity for left motor
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

#Go backward until it found a wall at a certain distance
def backwardUntilWall(cm):
        print("forward")
        #Send the case for forward
        arduino.write("j")
        time.sleep(1)
        #Send velocity for left motor
        arduino.write(chr(cm))
        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True


#Turn to an specific angle
def turnToDegree(angle):
        print("forward")
        #Send the case for forward
        arduino.write("k")
        time.sleep(1)
        #Send velocity for left motor
        arduino.write(chr(angle))
        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True


#Turn right a certain amount of degrees
def turnRightNDegrees(angle):
        print("forward")
        #Send the case for forward
        arduino.write("l")
        time.sleep(1)
        #Send velocity for left motor
        arduino.write(chr(angle))
        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True


#Turn left a certain amount of degrees
def turnLeftNDegrees(angle):
        print("forward")
        #Send the case for forward
        arduino.write("m")
        time.sleep(1)
        #Send velocity for left motor
        arduino.write(chr(angle))
        #Wait for arduino response
        while(arduino.inWaiting() <= 0):
                pass;
        if(arduino.read() == "1"):
                print("True")
                return True


#Move Platafform In
def platIn():
        print("platIn")
        #Tell the arduino to run brake()
        arduino.write("n")
        time.sleep(1);
        #Wait for something to happen
        while(arduino.inWaiting() <= 0):
                pass;

        if(arduino.read() == "1"):
                print("True")
                return True

        else:
                print("False")
                return False

#Move Platafform Out
def platOut():
        print("platOut")
        #Tell the arduino to run brake()
        arduino.write("o")
        time.sleep(1);
        #Wait for something to happen
        while(arduino.inWaiting() <= 0):
                pass;

        if(arduino.read() == "1"):
                print("True")
                return True

        else:
                print("False")
                return False


#Open Claw
def openClaw():
        print("openClaw")
        #Tell the arduino to run brake()
        arduino.write("p")
        time.sleep(1);
        #Wait for something to happen
        while(arduino.inWaiting() <= 0):
                pass;

        if(arduino.read() == "1"):
                print("True")
                return True

        else:
                print("False")
                return False

#Close Claw
def closeClaw():
        print("closeClaw")
        #Tell the arduino to run brake()
        arduino.write("q")
        time.sleep(1);
        #Wait for something to happen
        while(arduino.inWaiting() <= 0):
                pass;

        if(arduino.read() == "1"):
                print("True")
                return True

        else:
                print("False")
                return False

'''Aun falta hacer que el arduino regrese la inf de un sensor a la rasp
#Move Platafform In
def closeClaw():
        print("closeClaw")
        #Tell the arduino to run brake()
        arduino.write("r")
        time.sleep(1);
        #Wait for something to happen
        while(arduino.inWaiting() <= 0):
                pass;

        if(arduino.read() == "1"):
                print("True")
                return True

        print("No Brake")


#Move Platafform In
def closeClaw():
        print("closeClaw")
        #Tell the arduino to run brake()
        arduino.write("s")
        time.sleep(1);
        #Wait for something to happen
        while(arduino.inWaiting() <= 0):
                pass;

        if(arduino.read() == "1"):
                print("True")
                return True

        print("No Brake")
'''

##################################
if __name__ == "__main__":
        forward(70, 70);
        time.sleep(10);
        #brake();
        #platOut();
        #platIn();
        #closeClaw();
        #openClaw();
