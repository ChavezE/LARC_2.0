
# LIBS
import cv2
import numpy as np
import LARC2 as rb
from copy import deepcopy
# ------------------


tVal = 70
cap = cv2.VideoCapture(0)

############# QUERY IMAGES #####################
queryTerrine = cv2.imread('terrines.png',0)     
queryTank = cv2.imread('tank.png',0)   
queryExchange = cv2.imread('exchange.png',0)       



def getMarkerBase(contours,i,epsilon):

    
    baseMarkers = []

    for cnt in contours:
        area = cv2.contourArea(cnt)
        rect = cv2.minAreaRect(cnt)      # rotated rectangle
        w = int(rect[1][0])
        h = int(rect[1][1])
        rect_area = w * h
        if(rect_area > epsilon): 
            extent = float(area / rect_area)
            if (extent >= 0.8):   # tolerance
                baseMarkers.append((cnt,area))


    baseMarkers = rb.sortList(1,baseMarkers)
    finalMarkers=[]

    for x in range(0,i):
        if( x < len(baseMarkers)):
            if(baseMarkers[x][1]> epsilon):
                finalMarkers.append(baseMarkers[x])

    return finalMarkers



def getMatchesNum(query, subFrame):
    # Initiate SIFT detector
    sift = cv2.SIFT()
    # find the keypoints and descriptors with SIFT
    kp1, des1 = sift.detectAndCompute(query,None)
    kp2, des2 = sift.detectAndCompute(subFrame,None)

    if des2 != None:
        # create BFMatcher object
        # bf = cv2.BFMatcher()
        # bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)

        # BFMatcher with default params
        bf = cv2.BFMatcher()
        matches = bf.knnMatch(des1,des2, k=2)

        # Apply ratio test
        good = []
        for m,n in matches:
            if m.distance < 0.75*n.distance:
                good.append([m])



        return len(good)
        # # Match descriptors.
        # matches = bf.match(des1,des2)
        # return len(matches)
        #print "numero de matches  :", len(matches)
    
    else:
        return -1
            

ret = 1
def goLive():
  
    global queryTank
    global queryTerrine
    global queryExchange
    ret = 1

    while (1 and ret ):
        
        ret , frame = cap.read()
        frame = cv2.resize(frame,None,fx=0.5,fy=0.5,interpolation=cv2.INTER_AREA)
        filteredImage = rb.clearImage(frame)
        thres1 = rb.doThresHold(filteredImage,tVal,5,1)
        contours = rb.findContours(thres1)

        for cnt in contours:
            #epsilon = 0.1*cv2.arcLength(cnt,True)
            #approx = cv2.approxPolyDP(cnt,epsilon,True)
            cv2.drawContours(frame,[cnt],0,(0,0,255),2)


        markerBase = getMarkerBase(contours,2,epsilon=5000)
        if len(markerBase) > 0:
            font = cv2.FONT_HERSHEY_SIMPLEX
            for i in range (len(markerBase)):
                # print "MARKER",i
                x,y,w,h = cv2.boundingRect(markerBase[i][0])

                subFrame = frame[y:y+h,x:x+h]
                # cv2.imshow('sub',subFrame)
                res1 = getMatchesNum(queryTerrine,subFrame)
                # print "terrines", res1
                if(res1 > 15):
                    cv2.putText(frame,"TERRINES",(x,y), font, 0.7,(0,255,0),3)
                    print "TERRINES ZONE"

                res2 = getMatchesNum(queryExchange,subFrame)
                # print "exchange", res2
                if(res2 > 8):
                    cv2.putText(frame,"EXCHANGE",(x,y), font, 0.7,(0,255,0),3)
                    print "EXCHANGE ZONE"
                
                res3 = getMatchesNum(queryTank,subFrame)
                # print "tank", res3
                if(res3 > 12):
                    cv2.putText(frame,"TANK",(x,y), font, 0.7,(0,255,0),3)
                    print "TANK ZONE"

                
                
                cv2.drawContours(frame,[markerBase[i][0]],0,(255,0,0),5)



        else:
            print "NO MARKERS"


        

        
        # cv2.imshow('sub',subFrame)
        # cv2.drawContours(frame,[markerBase],0,(255,0,0),5)
        cv2.imshow('img',frame)
        # # cv2.imshow('i',img1)



        k = cv2.waitKey(10) & 0xFF
        if k == 27:
            break
        cv2.waitKey(100)

goLive()


cv2.destroyAllWindows()