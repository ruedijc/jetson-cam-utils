#!/bin/bash
#
# more verbose gstreamer pipeline for mipi-csi2 cameras. 
#  NOTE: script are sensitive to pipline + line continuation '!\' being together for some reason
#
#  NOTE: base imx274 resolution is 3840x2160, display is 1920x1080
gst-launch-1.0 nvarguscamerasrc sensor-id=0 !\
'video/x-raw(memory:NVMM), width=(int)3840, height=(int)2160, format=(string)NV12, framerate=(fraction)30/1' !\
nvvidconv flip-method=2 !\
tee name=t t. ! queue !\
nvvidconv !\
'video/x-raw' !\
nvoverlaysink overlay-w=640 overlay-h=480 t. ! queue !\
nvvidconv flip-method=1 !\
nvoverlaysink overlay-w=640 overlay-h=480

