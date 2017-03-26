import numpy as np
import cv2
from matplotlib import pyplot as plt

img = cv2.imread('cow1.jpg')
img = cv2.resize(img,None,fx=0.20,fy=0.20,interpolation=cv2.INTER_AREA)
gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

corners = cv2.goodFeaturesToTrack(gray,50,0.01,10)
corners = np.int0(corners)
print (corners)

for i in corners:
    x,y = i.ravel()
    cv2.circle(img,(x,y),3,255,-1)

cv2.imshow('i',img)
cv2.waitKey(0)