import cv2
import numpy as np
# Roborregos libs
import sys
sys.path.insert(0, '../lib/')
import Larc_vision_2017 as rb

cap = cv2.VideoCapture(0)
cv2.namedWindow('HSV',cv2.WINDOW_NORMAL)
cv2.namedWindow('BGR',cv2.WINDOW_NORMAL)
cv2.resizeWindow('BGR', 300,50)
cv2.resizeWindow('HSV', 300,50)

def nothing(x):
    pass
# create trackbars for color change
cv2.createTrackbar('H','HSV',0,179,nothing)
cv2.createTrackbar('S','HSV',0,255,nothing)
cv2.createTrackbar('V','HSV',0,255,nothing)

testColor = np.zeros((100,200,3), np.uint8)

# create trackbars for color change
cv2.createTrackbar('R','BGR',0,255,nothing)
cv2.createTrackbar('G','BGR',0,255,nothing)
cv2.createTrackbar('B','BGR',0,255,nothing)

while(1):

    # Take each frame
    _, frame = cap.read()
    frame = cv2.resize(frame,(640, 480), interpolation = cv2.INTER_CUBIC)


    h = cv2.getTrackbarPos('H','HSV')
    s = cv2.getTrackbarPos('S','HSV')
    v = cv2.getTrackbarPos('V','HSV')

    b = cv2.getTrackbarPos('B','BGR')
    g = cv2.getTrackbarPos('G','BGR')
    r = cv2.getTrackbarPos('R','BGR')

    testColor[:] = [b,g,r]
    colorToTrack = np.uint8([[[b,g,r ]]])
    hsv_color = cv2.cvtColor(colorToTrack,cv2.COLOR_BGR2HSV)
    print hsv_color

    # Convert BGR to HSV
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # define range of blue color in HSV
    lower_blue = np.array([h-20,100,100])
    upper_blue = np.array([h+20,s,v])

    # Threshold the HSV image to get only blue colors
    mask = cv2.inRange(hsv, lower_blue, upper_blue)

    # Bitwise-AND mask and original image
    res = cv2.bitwise_and(frame,frame, mask= mask)

    # cv2.imshow('frame',frame)
    # cv2.imshow('mask',mask)
    cv2.imshow('res',res)
    cv2.imshow('color',testColor)

    rb.getTankCenter(frame)
    break
    k = cv2.waitKey(100) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()


#
# import cv2
# import numpy as np
#
# def nothing(x):
#     pass
#
# # Create a black image, a window
# img = np.zeros((300,512,3), np.uint8)
# cv2.namedWindow('image')
#
# # create trackbars for color change
# cv2.createTrackbar('R','image',0,255,nothing)
# cv2.createTrackbar('G','image',0,255,nothing)
# cv2.createTrackbar('B','image',0,255,nothing)
#
# # create switch for ON/OFF functionality
# switch = '0 : OFF \n1 : ON'
# cv2.createTrackbar(switch, 'image',0,1,nothing)
#
# while(1):
#     cv2.imshow('image',img)
#     k = cv2.waitKey(100) & 0xFF
#     if k == 27:
#         break
#
#     # get current positions of four trackbars
#     r = cv2.getTrackbarPos('R','image')
#     g = cv2.getTrackbarPos('G','image')
#     b = cv2.getTrackbarPos('B','image')
#     s = cv2.getTrackbarPos(switch,'image')
#
#     if s == 0:
#         img[:] = 0
#     else:
#         img[:] = [b,g,r]
#
# cv2.destroyAllWindows()
