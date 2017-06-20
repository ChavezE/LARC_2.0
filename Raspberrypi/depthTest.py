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
otherIm = cv2.imread('hleft.jpg',0)
stereo = cv2.StereoBM(0,16, 71)

disparity = stereo.compute(imgL, imgR).astype(np.float32) / 16
disparity = abs(disparity)

print disparity.shape
print otherIm.shape


print ("TOTAL TIME: ",time.time() - this_time)

threshold = cv2.threshold(disparity, 10, 255, cv2.THRESH_BINARY)[1]
t2 = cv2.threshold(otherIm,100,255,cv2.THRESH_BINARY)[1]

mask_inv = cv2.bitwise_not(threshold)
result = cv2.bitwise_and(otherIm,otherIm,mask = thr)

cv2.imshow('p',result)
# cv2.imshow('p',imgR)

cv2.waitKey(0)
cv2.destroyAllWindows()
plt.imshow(threshold,'gray')
plt.show()
