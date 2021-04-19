#/bin/bash
#
# capture a single mipi-csi2 image to jpg
#
#
# notes -  quality levels- 
# at full 3840x2160 bitrate >=80000000
# at 1920x1080 ,    bitrate >=8000000
#
#
# nm-buffers sets the number of frames to store before exiting
# min ~100
#
gst-launch-1.0 nvarguscamerasrc sensor-id=0  num-buffers=300 !\
'video/x-raw(memory:NVMM), width=1920, height=1080, format=NV12, framerate=30/1' !\
nvvidconv flip-method=2 !\
omxh264enc bitrate=8000000 !\
'video/x-h264, stream-format=byte-stream' !\
h264parse !\
qtmux !\
filesink location='./vid.mp4' -e


