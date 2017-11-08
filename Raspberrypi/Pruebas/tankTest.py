import numpy as np
import cv2
import sys
sys.path.insert(0, '../lib/')
import Larc_vision_2017 as rb

tankCascade = cv2.CascadeClassifier('Cascades/TANK.xml')
cascadeTSensitivity = 100
debugger = True
whiteFrame = np.ones((480,640), np.uint8)
whiteFrame[:] = 255

cap = cv2.VideoCapture(0)



# def detectTank(img):
#    white = whiteFrame.copy()#whiteFrame.copy()#blackFrame.copy()

#    tanks = tankCascade.detectMultiScale(img, 1.1, cascadeTSensitivity)

#    xc,yc,hc,wc = 0,0,0,0
#    individualTank = []

#    tankDetected = False

#    if tanks is ():
#       print "Nada detectado"
#       return tankDetected, img

#    for (x,y,w,h) in tanks:

    
#       area = (float(w)*h)

#      #ampliation is calibratable,

#      #determines how much the
#      #detected area expands


#      #This condition is also calibratable, by determining
#      #a relationship between h/w
     
#       if y > 240 : 
#          #cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,255),2)

#          #Expanding the detected area
#          # xc = x - int(.5 * ampliation)
#          # yc = y - int(.5 * ampliation)
#          # hc = h + int((1 * ampliation))
#          # wc = w + int((3 * ampliation)) #1.5

#         xc = x 
#         yc = y 
#         hc = h 
#         wc = w 
           
#         tankDetected = True

#       cv2.rectangle(img,(x,y),(x+w,y+h),(0,255,0),1) # cv2.rectangle(img,(xc,yc),(xc+wc,yc+hc),(0,255,0),1)
#          #cv2.imshow("img",img)


#    individualTank = img[yc:yc+hc,xc:xc+wc]
#    print individualTank.shape
#    white[yc:yc+hc,xc:xc+wc] = individualTank
#    print white.shape

#    if debugger:
#       cv2.imshow("white", white)
#    return tankDetected, white

def doThresHold(filteredImage,tVal,k,i):
   _, thres1 = cv2.threshold(filteredImage,tVal,255,cv2.THRESH_BINARY)
   thres1 = cv2.erode(thres1,np.ones((k,k),np.uint8), iterations=i)

   return thres1

def isThereATank(img):
  # squares = []
  # thresh = doThresHold(img,50,1,3)
  # cv2.imshow("thresh",thresh)

  # contours = rb.findContours(filtered)

  # squares = rb.getGoodSquares(contours,thresh,[])

  # print len(squares)
  gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
  found,filtered,left,right,top = rb.detectTank(gray)
  if found:
    rb.drawLimits(img, left, right, top)
  cv2.imshow("img", img)





  



while 1:
    ret, img = cap.read()
    
    
    
    isThereATank(img)
    # rb.drawCowSquares(img,0,100,0,sqrs)
    k = cv2.waitKey(0) & 0xff
    if k == 13:
      break

# import numpy as np
# import cv2


# tankCascade = cv2.CascadeClassifier('Cascades/TANK.xml')
# whiteFrame = np.ones((480,640), np.uint8)
# whiteFrame[:] = 255

# cap = cv2.VideoCapture(0)


# def detect(gray):
#   tanks = tankCascade.detectMultiScale(gray, 1.1, 150)
#     # add this
#   individualTank = []


#   if tanks is ():
#     print "Nope"
#   else:
#     print "tank!"
#   for (x,y,w,h) in tanks:
     
#     relation = float(h)/w
#     if relation < 0.77 and relation > 0.55 and (w*h) > 11000 and w > 120:#(w*h) > 11000 and w > 120:
#         cv2.rectangle(img,(x,y),(x+w,y+h),(0,0,255),2)
#     else:
#         cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)

#     cv2.putText(img,str(w*h),(x,y),cv2.FONT_HERSHEY_SIMPLEX,1,(0,255,255))
#     cv2.putText(img,str(h),(x,y+(h/2)),cv2.FONT_HERSHEY_SIMPLEX,1,(0,255,0))
#     cv2.putText(img,str(w),(x,y+h),cv2.FONT_HERSHEY_SIMPLEX,1,(255,255,0))

#   cv2.imshow('img',img)

#   return 


# while 1:
#     ret, img = cap.read()
#     gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
#     # add this
#     # image, reject levels level weights.

#     detect(gray)
   
#     k = cv2.waitKey(30) & 0xff
#     if k == 13:
#         break

# cap.release()
# cv2.destroyAllWindows()
