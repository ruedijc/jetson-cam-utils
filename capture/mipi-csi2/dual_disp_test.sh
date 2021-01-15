#!/bin/bash
#
# testing gstreamer tee
#gst-launch-1.0 videotestsrc ! tee name=t \
#t. ! queue ! xvimagesink \
#t. ! queue ! xvimagesink

#gst-launch-1.0 nvarguscamerasrc ! tee name=t \
#t. ! queue !\
#'video/x-raw(memory:NVMM), width=(int)800, height=(int)400, format=(string)NV12, framerate=(fraction)30/1' !\
#nvvidconv flip-method=2 !\
#xvimagesink \
#t. ! queue !\
#'video/x-raw(memory:NVMM), width=(int)800, height=(int)400, format=(string)NV12, framerate=(fraction)30/1' !\
#nvvidconv flip-method=2 !\
#xvimagesink

gst-launch-1.0 nvarguscamerasrc ! tee name=t \
t. ! queue !\
nvvidconv !\
'video/x-raw(memory:NVMM), width=(int)640, height=(int)363, format=(string)NV12' !\
nvvidconv flip-method=2 !\
xvimagesink \
t. ! queue !\
nvvidconv !\
'video/x-raw(memory:NVMM), width=(int)3840, height=(int)2160, format=(string)NV12' !\
nvvidconv flip-method=2 !\
xvimagesink 









