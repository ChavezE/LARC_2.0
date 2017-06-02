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

        print("No Brake")

if __name__ == "__main__":
        brake()
