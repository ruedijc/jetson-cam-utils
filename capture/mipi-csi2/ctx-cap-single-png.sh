#/bin/bash
#
# capture a single mipi-csi2 image to jpg
#

TIME=$(date '+%Y-%m-%d.%H-%M-%S')
LOCATION='/media/msata/img/'
FILE=$LOCATION$TIME'_ctx.png'
echo $FILE


gst-launch-1.0 nvarguscamerasrc sensor-id=0  num-buffers=1 !\
'video/x-raw(memory:NVMM), width=3840, height=2160, format=NV12' !\
nvvidconv flip-method=2 !\
pngenc  compression-level=6 snapshot=true !\
filesink location=$FILE -e

#nvjpegenc  quality=85  idct-method=float !\

#queue  min-threshold-time=10000000000 leaky=downstream !\


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

#gst-launch-1.0 filesrc location=$FILE !\
#pngdec !\
#imagefreeze !\
#xvimagesink -e



