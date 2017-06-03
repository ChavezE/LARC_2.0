#Standard libraries
#import cv2
#import numpy as numpy
import serial
import time
from copy import deepcopy

import communication as com

#Global Variables
#time.sleep(5);
try:
        arduino = serial.Serial('/dev/ttyUSB0', 9600, timeout = 1);
except serial.SerialException:
        print("Desconecta y reconecta el arduino");
        while(True):
                pass;

#time.sleep(10);


if __name__ == "__main__":
        com.backward()
