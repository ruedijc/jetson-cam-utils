#!/bin/bash
#
# MIPI-CSI2 camera view from CTI - 
# https://connecttech.com/resource-center/kdb364-using-mipi-csi-2-cameras-connect-tech-asg006-spacely/
#
gst-launch-1.0 nvarguscamerasrc sensor-id=0 !\
"video/x-raw(memory:NVMM), width=3864, height=2196, format=NV12, framerate=60/1" !\
nvvidconv flip-method=2 !\
"video/x-raw, width=800, height=454" !\
nvvidconv !\
nvegltransform !\
nveglglessink -e

#xvimagesink -ev &
