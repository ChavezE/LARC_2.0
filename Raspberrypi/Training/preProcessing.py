import cv2
import numpy as np
import glob

drawing = False
nx,ny = -1, -1
imag,img,square = [],[],[]

# mouse callback function
def drawRectangle(event,x,y,flags,param):
	global nx, ny, drawing, img, square
	
	if event == cv2.EVENT_LBUTTONDOWN:

		if drawing:
			if nx < x and ny < y:
				cv2.rectangle(img,(nx,ny),(x,y),(255,255,0),1)
				
				square = cv2.cvtColor(img[ny+1:y,nx+1:x],cv2.COLOR_BGR2GRAY)
				
				cv2.imshow('Selected Area',cv2.pyrUp(cv2.pyrUp(square.copy())))

		else:
			img = imag.copy()
			cv2.destroyWindow('Selected Area')
			nx, ny = x, y
			#cv2.circle(img,(x,y),5,(0,0,255),-1)
			cv2.line(img,(x,y),(x+10,y),(0,0,255),1)
			cv2.line(img,(x,y),(x,y+10),(0,0,255),1)


		drawing = not drawing	



cv2.namedWindow('image')
cv2.setMouseCallback('image',drawRectangle)

savePath = "C:/Users/VIAO2/Desktop/Codigo/Vision/Training/CornersTraining/images"

cont = 0


for path in glob.glob("../CowTraining/images/*.jpg"):
	imag = cv2.imread(path)
	img = imag.copy()
	#square = cv2.cvtColor(img.copy(),cv2.COLOR_BGR2GRAY)
	while 1:
		
		cv2.imshow('image',img)

		k = cv2.waitKey(33)
		#Letter "S" on the keyboard, to save
		if k == 115:
			cv2.destroyWindow('Selected Area')
			cv2.imwrite(str(savePath)+"/"+str(cont)+".jpg",square)
			cont = cont + 1

		#Letter "N" on the keyboard, for next
		if k == 110:
			break
		#"ESC" to exit, keep pressed
		if k == 27:
			break
			
	if k == 27:
		break		
cv2.destroyAllWindows()