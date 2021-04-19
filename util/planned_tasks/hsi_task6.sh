#/bin/bash
#
# capture a single mipi-csi2 image to jpg
#
#
# notes -  quality levels- h264 
# at full 3840x2160 bitrate >=80000000
# at 1920x1080 ,    bitrate >=8000000
#

# set default value
CTX_VID_DURATION="30"
# Context camera video bitrate
# notes -  quality levels- h264 
# at full 4k / 3840x2160,  bitrate >=80000000
# at 1920x1080 ,           bitrate >=8000000
CTX_VID_BITRATE="8000000"
# Context camera, capture in 4k? if not, in HD
CTX_VID_4K="0"
# Save Path for image captures
SAVE_PATH="/media/msata/queue/"
# Maximum size of files on disk before dropping old files, in GB
TASK_MAX_DISK_GB="1"

# load config values
source /etc/hsi-config/hsi_task6.ini


#echo values
echo "vid duration is: ${CTX_VID_DURATION}"
echo "vid bitrate is: ${CTX_VID_BITRATE}"          
echo "4k? is: ${CTX_VID_4K}"
echo "save path is: ${SAVE_PATH}"
echo "max disk usage : ${TASK_MAX_DISK_GB}"


if [ "CTX_VID_4K" = "1" ]; then

    #DURATION=10 #recording duration in seconds
    TIME=$(date '+%Y-%m-%d.%H-%M-%S')
    #LOCATION='/media/msata/queue/'
    #FILE=$LOCATION$TIME'_hd_vid.h265'
    FILE=$SAVE_PATH$TIME'_4K_vid.h265'
    echo $FILE

    gst-launch-1.0 nvarguscamerasrc sensor-id=0  timeout=$CTX_VID_DURATION !\
    'video/x-raw(memory:NVMM), width=3840, height=2160, format=NV12, framerate=30/1' !\
    nvvidconv flip-method=2 !\
    'video/x-raw(memory:NVMM), format=I420' !\
    omxh265enc peak-bitrate=$CTX_VID_BITRATE !\
    'video/x-h265, stream-format=byte-stream' !\
    h265parse !\
    qtmux !\
    filesink location=$FILE -e

else

    #DURATION=10 #recording duration in seconds
    TIME=$(date '+%Y-%m-%d.%H-%M-%S')
    #LOCATION='/media/msata/queue/'
    #FILE=$LOCATION$TIME'_hd_vid.h265'
    FILE=$SAVE_PATH$TIME'_hd_vid.h265'
    echo $FILE

    gst-launch-1.0 nvarguscamerasrc sensor-id=0  timeout=$CTX_VID_DURATION !\
    'video/x-raw(memory:NVMM), width=1920, height=1080, format=NV12, framerate=30/1' !\
    nvvidconv flip-method=2 !\
    'video/x-raw(memory:NVMM), format=I420' !\
    omxh265enc peak-bitrate=8000000 !\
    'video/x-h265, stream-format=byte-stream' !\
    h265parse !\
    qtmux !\
    filesink location=$FILE -e

fi

# show size
echo "==============================================="
du -h $FILE
# show vid folder size
du -h $SAVE_PATH
echo "==============================================="


#for development, now open/playback the video
# take out for sapce use -

#gst-launch-1.0 filesrc location=$FILE !\
#qtdemux name=demux demux.video_0 !\
#queue !\
#h265parse ! omxh265dec !\
#nveglglessink -e


