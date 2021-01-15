#!/bin/bash
# simplest mipi-csi-viewer
#
# Take a picture and save to disk
#CSI camera
#
#Runtime command line option
#
#    nvgstcapture-1.0
#
#    Press 'j' to Capture one image.
#    Press 'q' to exit

#nvgstcapture-1.0

# camsrc=1 is csi default
nvgstcapture-1.0 --camsrc=1
