#!/bin/bash
#
# close shutter
#

# set NSLEEP high to enable shutter control
/home/labuser/development/jetson-cam-utils/util/p13-6_on.sh
# set BIN2 high to drive shutter closed
/home/labuser/development/jetson-cam-utils/util/p13-4_on.sh
# remain high for 2 seconds
sleep 1
# set BIN2 low to stop driving shutter 
/home/labuser/development/jetson-cam-utils/util/p13-4_off.sh
# set shutter state indicator to 1 (closed)
echo 1 > /var/hsi/shutter_state
# set NSLEEP low to disable shutter control
/home/labuser/development/jetson-cam-utils/util/p13-6_off.sh

echo "Shutter closed"


