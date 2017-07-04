import numpy as np
import cv2


H_cascade = cv2.CascadeClassifier('../Cascades/COW3.xml')

cap = cv2.VideoCapture(0)

while 1:
    ret, img = cap.read()
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # add this
    # image, reject levels level weights.
    H = H_cascade.detectMultiScale(gray, 1.3, 75)
    # add this

    if H is ():
        print "Nope"
    else:
        print "Cow!"
    for (x,y,w,h) in H:
       
        relation = float(h)/w
        if relation < 0.77 and relation > 0.74 and (w*h) > 11000 and w > 120:#(w*h) > 11000 and w > 120:
            cv2.rectangle(img,(x,y),(x+w,y+h),(0,0,255),2)
        else:
            cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)

        cv2.putText(img,str(w*h),(x,y),cv2.FONT_HERSHEY_SIMPLEX,1,(0,255,255))
        cv2.putText(img,str(h),(x,y+(h/2)),cv2.FONT_HERSHEY_SIMPLEX,1,(0,255,0))
        cv2.putText(img,str(w),(x,y+h),cv2.FONT_HERSHEY_SIMPLEX,1,(255,255,0))

    cv2.imshow('img',img)
    k = cv2.waitKey(30) & 0xff
    if k == 13:
        break

cap.release()
cv2.destroyAllWindows()