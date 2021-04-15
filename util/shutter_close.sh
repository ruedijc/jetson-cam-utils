#!/bin/bash
#
# close shutter
#
# see sysfs assignments in Spacely GPIO breakout map

# set NSLEEP high to enable shutter control
#/home/labuser/development/jetson-cam-utils/util/p13-6_on.sh
echo 1 > /sys/class/gpio/gpio227/value

# set BIN1 high to drive shutter closed
#/home/labuser/development/jetson-cam-utils/util/p13-4_on.sh
echo 1 > /sys/class/gpio/gpio229/value

# remain high for 1 seconds
sleep 1

# set BIN1 low to stop driving shutter 
#/home/labuser/development/jetson-cam-utils/util/p13-4_off.sh
echo 0 > /sys/class/gpio/gpio229/value


# set shutter state indicator to 1 (closed)
echo 1 > /var/hsi/shutter_state


# set NSLEEP low to disable shutter control
#/home/labuser/development/jetson-cam-utils/util/p13-6_off.sh
echo 0 > /sys/class/gpio/gpio227/value


echo "Shutter closed"


