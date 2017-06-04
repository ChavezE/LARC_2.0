#Standard libraries
#import cv2
#import numpy as numpy
import serial
import time
from copy import deepcopy

import communication as com

#Global Variables
#time.sleep(5);
#time.sleep(10);


if __name__ == "__main__":
	com.getDistance("pinSB")