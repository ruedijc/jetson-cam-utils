#!/bin/bash
#
# this sets up the 16 GPIO on CTI Spacely carrier for a mix of output and input pins
# ASG006 spacely with tx2

#first export for use
# P00 thru P07
echo 216 > /sys/class/gpio/export
echo 217 > /sys/class/gpio/export
echo 218 > /sys/class/gpio/export
echo 219 > /sys/class/gpio/export
echo 220 > /sys/class/gpio/export
echo 221 > /sys/class/gpio/export
echo 222 > /sys/class/gpio/export
echo 223 > /sys/class/gpio/export
# P010 thru P17
echo 224 > /sys/class/gpio/export
echo 225 > /sys/class/gpio/export
echo 226 > /sys/class/gpio/export
echo 227 > /sys/class/gpio/export
echo 228 > /sys/class/gpio/export
echo 229 > /sys/class/gpio/export
echo 230 > /sys/class/gpio/export
echo 231 > /sys/class/gpio/export

# Now set as read or write
# setting P00 thru P07 as outputs
echo out > /sys/class/gpio/gpio216/direction
echo out > /sys/class/gpio/gpio217/direction
echo out > /sys/class/gpio/gpio218/direction
echo out > /sys/class/gpio/gpio219/direction
echo out > /sys/class/gpio/gpio220/direction
echo out > /sys/class/gpio/gpio221/direction
echo out > /sys/class/gpio/gpio222/direction
echo out > /sys/class/gpio/gpio223/direction
# setting P10 thru P17 to inputs
echo in > /sys/class/gpio/gpio224/direction
echo in > /sys/class/gpio/gpio225/direction
echo in > /sys/class/gpio/gpio226/direction
echo in > /sys/class/gpio/gpio227/direction
echo in > /sys/class/gpio/gpio228/direction
echo in > /sys/class/gpio/gpio229/direction
echo in > /sys/class/gpio/gpio230/direction
echo in > /sys/class/gpio/gpio231/direction

# Example: write a value -
# echo 1> /sys/class/gpio/gpio216/value

# Example read a value - 
# cat /sys/class/gpio/gpio216/value

 

