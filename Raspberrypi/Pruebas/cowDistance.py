import cv2
import numpy as np
# imporging Roborregos libs
import sys
sys.path.insert(0, '../lib/')
import Larc_vision_2017 as rb

focalLength = 0
cowHeightCm = 17

def live():
	cap = cv2.VideoCapture(1)
	 
	while(1):
		ret, mainFrame = cap.read()
		if ret:
			clearedFrame = rb.clearImage(mainFrame)
			validation2, filtered = rb.detectCow(clearedFrame)
			validation,maxLenT,_ = rb.isThereACow(mainFrame,filtered)
			
			if validation:
				left,right,up,bottom=rb.calcCowLimits2(maxLenT)
				rb.drawLimits2(mainFrame,left,right,up,bottom)
				cowHeightPx = abs(up-bottom)
				print cowHeightPx

				if cowHeightPx > 0 :
					distance = (cowHeightCm*focalLength)/cowHeightPx
					cv2.putText(mainFrame,str(distance), (left,up), cv2.FONT_HERSHEY_SIMPLEX, 1, (255,0,0), 3)

			cv2.imshow("vaquita", mainFrame)
			k = cv2.waitKey(0)

			if k == 27:
				break



def picture():
	global focalLength

	mainFrame = cv2.imread("C:/Users/VIAO2/Desktop/Codigo/Larc/images/0.jpg")
	clearedFrame = rb.clearImage(mainFrame)
	validation2, filtered = rb.detectCow(clearedFrame)
	validation,maxLenT,_ = rb.isThereACow(mainFrame,filtered)
	
	if validation:
		left,right,up,bottom=rb.calcCowLimits2(maxLenT)
		rb.drawLimits2(mainFrame,left,right,up,bottom)

		cowHeightPx = abs(up-bottom)
		# focalLength = (cowHeightPx * 70)/cowHeightCm
		focalLength = 620

		print cowHeightPx
		print focalLength
	
	cv2.imshow("vaquita", mainFrame)
	cv2.waitKey(0)





if __name__ == "__main__":
	picture()
	live()
cv2.destroyAllWindows()