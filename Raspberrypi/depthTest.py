import numpy as np
import cv2
import time
from matplotlib import pyplot as plt

#
# cap1 = cv2.VideoCapture(0)
# cap2 = cv2.VideoCapture(1)
# Left=[]
# Right = []
#
# imgR = cv2.imread('b.jpg',0)
# imgL = cv2.imread('a.jpg',0)
#
# # def takePicture():
# # 	global Right,Left
# #
# # 	_,frame1= cap1.read()
# # 	_,frame2= cap2.read()
# # 	frame1 = cv2.cvtColor(frame1, cv2.COLOR_BGR2GRAY)
# # 	frame2 = cv2.cvtColor(frame2, cv2.COLOR_BGR2GRAY)
# # 	Left = cv2.resize(frame1,(600, 800), interpolation = cv2.INTER_CUBIC)
# # 	Right = cv2.resize(frame2,(600, 800), interpolation = cv2.INTER_CUBIC)
# # 	print frame1.shape
# # 	print frame2.shape
# # 	cv2.imshow('i',Left)
# # 	cv2.imshow('d',Right)
# # 	cv2.waitKey(0)
# # 	cv2.destroyAllWindows()
#
#
#
# if __name__ == '__main__':
#
# 	# takePicture()
# 	stereo = cv2.StereoBM(0, ndisparities=16, SADWindowSize=15)
# 	disparity = stereo.compute(imgL,imgR)
# 	plt.imshow(disparity,'gray')
# 	plt.show()

# imgR = cv2.imread('Yeuna9x.png',0)
# imgL = cv2.imread('SuXT483.png',0)
this_time = time.time()
imgR = cv2.imread('rigth.jpg',0)
imgL = cv2.imread('mleft.jpg',0)
stereo = cv2.StereoBM(0,16, 71)

disparity = stereo.compute(imgL, imgR)
print ("TOTAL TIME: ",time.time() - this_time)

h,w= imgR.shape
thes = 180
eps = 10
for i in range(h):
    for j in range (w):
        if abs(disparity[i,j] - thes) > eps:
            imgR[i,j] = 255

cv2.imshow('p',disparity)
cv2.imshow('p',imgR)
print disparity
cv2.waitKey(0)
cv2.destroyAllWindows()
plt.imshow(disparity,'gray')
plt.show()
