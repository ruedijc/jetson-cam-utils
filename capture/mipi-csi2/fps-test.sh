#!/bin/bash
#
# test max fps
#

# test with fake videosrc -
#gst-launch-1.0 videotestsrc ! fpsdisplaysink

# test with LI-IMX274-M12 mipi cam
#gst-launch-1.0 nvarguscamerasrc ! fpsdisplaysink

#gst-launch-1.0 nvarguscamerasrc sensor-id=0 !\
#'video/x-raw(memory:NVMM), width=(int)3840, height=(int)2160' !\
#nvvidconv flip-method=2 !\
#fpsdisplaysink

#GST_DEBUG="GST_TRACER:7" GST_TRACERS="framerate" gst-launch-1.0 videotestsrc !\
#videorate max-rate=15 ! fakesink

gst-launch-1.0 nvarguscamerasrc sensor-id=0 !\
"video/x-raw(memory:NVMM), width=3840, height=2160, format=NV12, framerate=30/1" !\
nvvidconv flip-method=2 !\
xvimagesink -e

