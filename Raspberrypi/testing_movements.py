

# standar libs
import cv2
import numpy as np
import serial
import time
from copy import deepcopy

# Roborregos libs
import Larc_vision_2017 as rb
import communication as com






'''
    MAIN
'''
if __name__ == "__main__":
	com.forwardNCm(40)
	com.backwardNCm(40)
	com.turnNDegrees(90,0)
	com.turnNDegrees(90,1)
	

