import numpy as np
import cv2

'''
 This works on jetson nano, with jetpack 4.4.1 opencv installed, with logitech 920 webcam
 with the camera on /dev/video0
 -JCR
'''

cap = cv2.VideoCapture("/dev/video0")
while(True):
    # capture frame-by-frmae
    ret, frame = cap.read()
    
    # Our operations on the frame come here
    #    this example makes a grayscale version of the frame
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    

    # Display the resulting frame
    #cv2.imshow('frame',gray)
    cv2.imshow('frame',frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything is done, release the capture
cap.release()
cv2.destroyAllWindows()


