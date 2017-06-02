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

        print("No Brake")

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

        print("No Brake")

#Move Platafform In
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

        print("No Brake")

#Move Platafform In
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

        print("No Brake")

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

        print("No Brake")

#Go Forward
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

##################################
if __name__ == "__main__":
        forward(70, 70);
        time.sleep(10);
        #brake();
        #platOut();
        #platIn();
        #closeClaw();
        #openClaw();
