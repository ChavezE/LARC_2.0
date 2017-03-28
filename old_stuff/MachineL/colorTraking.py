import cv2
import numpy as np
import copy as deepcopy
import LARC2 as rb

cap = cv2.VideoCapture(0)

while(1):

    # Take each frame
    _, frame = cap.read()
    frame = cv2.resize(frame,None,fx=0.50,fy=0.50,interpolation=cv2.INTER_AREA)

    # Convert BGR to HSV
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # define range of orange color in HSV
    lower_blue = np.array([30,110,110])
    upper_blue = np.array([255,130,130])

    

    ORANGE_MIN = np.array([1, 186, 130],np.uint8)
    ORANGE_MAX = np.array([180, 255, 255],np.uint8)


    # Threshold the HSV image to get only orange
    mask = cv2.inRange(hsv, ORANGE_MIN, ORANGE_MAX)
    
    mask2 = cv2.inRange(hsv, ORANGE_MIN, ORANGE_MAX)

    mask = cv2.erode(mask,np.ones((7,7),np.uint8), iterations=1)

    contours = rb.findContours(mask)
    bigest = 1000
    bigestCont = contours[0]

    for cnt in contours:
        # epsilon = 0.1*cv2.arcLength(cnt,True)
        # approx = cv2.approxPolyDP(cnt,epsilon,True)
        if cv2.contourArea(cnt) > bigest:
            # cv2.drawContours(frame,[cnt],0,(255,0,0),3)
            bigestCont = cnt

    cv2.drawContours(frame,[bigestCont],0,(0,255,0),5)

    # Bitwise-AND mask and original image
    #res = cv2.bitwise_and(frame,frame, mask= mask)

    cv2.imshow('frame',frame)
    #cv2.imshow('mask',mask2)
    #cv2.imshow('res',res)
    k = cv2.waitKey(300) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()