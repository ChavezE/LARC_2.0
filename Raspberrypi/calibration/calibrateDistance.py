
# standar libs
import cv2
import numpy as np
import time
import math
import glob
from copy import deepcopy

# Roborregos libs
import sys
sys.path.insert(0, '../lib/')
import Larc_vision_2017 as rb


# global main Frame
frame = []

# global points to store (x,y) coord for linear regresion
points = []
# A and B from linear regresion y = A*x + B
A = 0
B = 0

def analyseData():
    global frame
    global points
    # get all images in folder and sort them
    images = sorted(glob.glob("./distance_data/*.jpg"))

    for path in images:
        print "READING...  " + str(path)
        frame = cv2.imread(path)

        # storing distance
        stringPath = str(path)
        cmString = ""
        for i in range(len(stringPath)):
            # if is a number then it is the distance
            if(stringPath[i].isdigit()):
                cmString += stringPath[i]
        print "centimeters", cmString
        # Haar cascade detection
        clearedFrame = rb.clearImage(frame)
        found, Haarfiltered = rb.detectCow(clearedFrame)
        if found:
            # Tissue
            cowExists, tissue, allSquares = rb.isThereACow(frame,Haarfiltered)
            if cowExists:
                # this method gets the Left, Rigth and Top limits in pixels
                L, R, T = rb.calcCowLimits(tissue)
                # appending cm and pixels into all the points
                pTemp = rb.cowSquare(T,int(cmString),0,0,0)
                points.append(pTemp)

                print "TOP DISTANCE : ", T, " pxls"
                print "OK"
            else:
                print "COW NOT FOUND"
            print "DONE  ",(str(images.index(path) + 1)), "/", (str(len(images)))
            cv2.imshow(str(path), frame)
            cv2.waitKey(0)
            cv2.destroyWindow(str(path))
            print ""
        else:
            print "COW NOT FOUND"
            print ""

def calcY(x,A,B):
    y = x*A + B
    return y

def calibDst():
    global A
    global B
    A, B, theta = rb.ajusteDeCurvas(points)
    #########################
    # saving data in reference File
    ########################
    dfile = open("distParams","w")
    linesToWrite = [str(A)+"\n",str(B)]
    dfile.writelines(linesToWrite)
    dfile.close()

    print A, "  ", B

    # print cords
    for p in points:
        print p.getX(), p.getY()

    # test
    for c in points:
        dist = calcY(int(c.getX()),A,B)
        print "obtained dst : ", dist, " real distance : ", c.getY()



# driving the functions
analyseData()
calibDst()
