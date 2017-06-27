
import numpy as np
import cv2


cow = cv2.CascadeClassifier('../Cascades/COW1.xml')


for i in range(1, 35):
    img=cv2.imread("../TestPhotos/" + str(i) + ".jpg")
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # add this
    # image, reject levels level weights.
    cows = cow.detectMultiScale(gray, 1.3, 65)
    # add this

    if cows is ():
        print "Nope"
    else:
        print "Cow!"

    for (x,y,w,h) in cows:
        #print "salio la h wey"
        cv2.rectangle(img,(x,y),(x+w,y+h),(0,0,255),2)

    cv2.imshow('img',img)
    cv2.waitKey(0)

cap.release()
cv2.destroyAllWindows()
