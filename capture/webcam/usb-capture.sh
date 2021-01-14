#!/bin/bash

#capture image with USB camera
# 1 : start recording video
# 0 : stop recording video
# q : exit

# V4L2 USB camera (where <N> is the /dev/videoN node)
nvgstcapture-1.0 --mode=2 --camsrc=0 --cap-dev-node=0

