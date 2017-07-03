import numpy as np
import cv2


cow = cv2.CascadeClassifier('../Cascades/COW1.xml')
black = cv2.imread("../images/black.jpg",0)

def detector(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    blackTemp = black.copy()

    cows = cow.detectMultiScale(gray, 1.3, 100)

    xc,yc,hc,wc = 0,0,0,0

    if cows is ():
        print "Nope"
    else:
        print "Cow!"
    for (x,y,w,h) in cows:
       
        relation = float(h)/w
        area = (float(w)*h)
        ampliation = (6 * (area/10000))
        if relation < 0.77 and relation > 0.74 and area > 11000 and w > 120:#(w*h) > 11000 and w > 120:
            cv2.rectangle(img,(x,y),(x+w,y+h),(0,0,255),2)

            xc = x - int(ampliation)
            yc = y - int(.6 * ampliation)
            hc = h + int((3 * ampliation))
            wc = w + int((2 * ampliation))

            # xc = x if xc < 0 else xc
            # xc = x if xc > 640 else xc
            # yc = y if yc < 0 else yc
            # yc = y if yc > 480 else yc


            cv2.rectangle(img,(xc,yc),(xc+wc,yc+hc),(0,255,0),1)
        else:
            cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)

        cv2.putText(img,str(w*h),(x,y),cv2.FONT_HERSHEY_SIMPLEX,1,(0,255,255))
        cv2.putText(img,str(h),(x,y+(h/2)),cv2.FONT_HERSHEY_SIMPLEX,1,(0,255,0))
        cv2.putText(img,str(w),(x,y+h),cv2.FONT_HERSHEY_SIMPLEX,1,(255,255,0))
        cv2.putText(img,str(x) + "," + str(y),(x-50,y),cv2.FONT_HERSHEY_SIMPLEX,0.5,(255,0,0))

        individualCow = gray[yc:yc+hc,xc:xc+wc]
    # black=np.zeros(img.shape[:2])
    #x,y,z = img.shape[:3]
    
        blackTemp[yc:yc+hc,xc:xc+wc] = individualCow
   
    cv2.imshow("black", blackTemp)

    return img


cap = cv2.VideoCapture(0)

while 1:
    ret, img = cap.read()

    cv2.imshow('img',detector(img))
    k = cv2.waitKey(30) & 0xff
    if k == 13:
        break

cap.release()
cv2.destroyAllWindows()