#/bin/bash
#
#
#
gst-launch-1.0 filesrc location='./vid.mp4' !\
qtdemux name=demux demux.video_0 !\
queue !\
h264parse ! omxh264dec !\
nveglglessink -e
