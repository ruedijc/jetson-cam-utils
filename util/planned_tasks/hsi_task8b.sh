#/bin/bash
#
# capture a single mipi-csi2 image to jpg
#
#
# notes -  quality levels- h264 
# at full 3840x2160 bitrate >=80000000
# at 1920x1080 ,    bitrate >=8000000
#

DURATION=600 #recording duration in seconds
TIME=$(date '+%Y-%m-%d.%H-%M-%S')
LOCATION='/media/msata/queue/'
FILE=$LOCATION$TIME'_4k_vid.h265'
echo $FILE

gst-launch-1.0 nvarguscamerasrc sensor-id=0  timeout=$DURATION !\
'video/x-raw(memory:NVMM), width=3840, height=2160, format=NV12, framerate=30/1' !\
nvvidconv flip-method=2 !\
'video/x-raw(memory:NVMM), format=I420' !\
omxh265enc peak-bitrate=80000000 !\
'video/x-h265, stream-format=byte-stream' !\
h265parse !\
qtmux !\
filesink location=$FILE -e

# show size
echo "==============================================="
du -h $FILE
# show vid folder size
du -h $LOCATION
echo "==============================================="


#for development, now open/playback the video
# take out for sapce use -

#gst-launch-1.0 filesrc location=$FILE !\
#qtdemux name=demux demux.video_0 !\
#queue !\
#h265parse ! omxh265dec !\
#nveglglessink -e


