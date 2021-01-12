#!/bin/bash
#
# test of gstreamer capture pipeline for webcam (vs mipi-csi cam)

# see: developer.nvidia.com/embedded/learn/tutorials/first-picture-csi-usb-camera
####
# Nvidia has their own gstcapture function - 
# nvgstcapture-1.0

#capture preview and display on screen (mipi)
#nvgstcapture-1.0

#display preview with usb camera
# --this works with logitech webcams into jetson nano with a  /dev/video0 device
nvgstcapture-1.0 --camsrc=0 --cap-dev-node=0

#capture with usb camera
# 1 : start recording video
# 0 : stop recording video
# q : exit
nvgstcapture-1.0 --mode=2 --camsrc=0 --cap-dev-node=0


