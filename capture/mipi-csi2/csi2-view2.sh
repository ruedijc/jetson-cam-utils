#!/bin/bash
#
# MIPI-CSI2 camera view from CTI - 
# https://connecttech.com/resource-center/kdb364-using-mipi-csi-2-cameras-connect-tech-asg006-spacely/
#
gst-launch-1.0 nvarguscamerasrc sensor-id=0 !\
"video/x-raw(memory:NVMM),height=1080, width=1920, format=NV12, framerate=60/1" !\
nvvidconv !\
xvimagesink -ev &
