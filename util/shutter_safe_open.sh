#!/bin/bash
#
# open shutter if no sun is detected
#

# set NSLEEP high to enable shutter control
/home/labuser/development/jetson-cam-utils/util/p13-6_on.sh
# set BIN1 high to drive shutter open
/home/labuser/development/jetson-cam-utils/util/p13-3_on.sh
# remain high for 2 seconds
sleep 1
# set BIN1 low to stop driving shutter 
/home/labuser/development/jetson-cam-utils/util/p13-3_off.sh
# set shutter state indicator to 0 (open)
echo 0 > /var/hsi/shutter_state
# set NSLEEP high to disable shutter control
/home/labuser/development/jetson-cam-utils/util/p13-6_off.sh

echo "Shutter open"


