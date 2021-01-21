#/bin/bash
#
# videocapture using nvgstcapture

nvgstcapture-1.0 -A -C 100 \
--capture-auto \
--orientation=ls -2 \
--video-res=8 \
--capture-time=10 \
--mode=2 \
--quit-after=10 \
--file-name='vid2.mp4'

