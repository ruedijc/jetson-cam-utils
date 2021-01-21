#/bin/bash
#
# capture a single mipi-csi2 image to jpg
#

TIME=$(date '+%Y-%m-%d.%H-%M-%S')
LOCATION='/media/msata/img/'
FILE=$LOCATION$TIME'_ctx.jpg'
echo $FILE


gst-launch-1.0 nvarguscamerasrc sensor-id=0  num-buffers=1 !\
'video/x-raw(memory:NVMM), width=3840, height=2160, format=NV12' !\
nvvidconv flip-method=2 !\
queue max-size-buffers=1 leaky=downstream !\
nvjpegenc  quality=85  idct-method=float !\
filesink location=$FILE -e

#for development, now open/playback the video
# take out for sapce use -

# show size
echo "=================================================="
du -h $FILE
# show vid folder size
du -h $LOCATION
echo "=================================================="


#exit 


#for development, now open/playback the video
# take out for sapce use -

gst-launch-1.0 filesrc location=$FILE !\
nvjpegdec !\
imagefreeze !\
xvimagesink -e



