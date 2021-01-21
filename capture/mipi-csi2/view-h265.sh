#/bin/bash
#
#
#
gst-launch-1.0 filesrc location='./vid.h265' !\
qtdemux name=demux demux.video_0 !\
queue !\
h265parse ! omxh265dec !\
nveglglessink -e
