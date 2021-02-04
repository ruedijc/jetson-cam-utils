#!/bin/bash
#
# this sets up the 16 GPIO on CTI Spacely carrier , and turns GPIO14 to 'off' state

#first export for use
# P14
echo 228 > /sys/class/gpio/export

# Now set as read or write
# setting P14 to output (P14 is GPIO 228)
echo out > /sys/class/gpio/gpio228/direction 

# WRite GPIO14 to off
echo 0 > /sys/class/gpio/gpio228/value

# Example read a value - 
# cat /sys/class/gpio/gpio216/value

 

