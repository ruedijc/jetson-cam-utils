#!/bin/bash

#
# on computer startup, set everything as an output, and put low
# set 0-7 as output, set 0-7 low
#i2cset -y -f 0x00 0x77 0x06 0x00
#i2cset -y -f 0x00 0x77 0x02 0x00
# set 8-15 as output, set 8-15 low
#i2cset -y -f 0x00 0x77 0x07 0x00
#i2cset -y -f 0x00 0x77 0x03 0x00 
# end startup

if [ `whoami` != root ]; then
    echo "Please run this script as root or using sudo"
    exit
fi

echo "Using the system driver for gpio, export pins"
echo 230 > /sys/class/gpio/export
echo 231 > /sys/class/gpio/export
echo 228 > /sys/class/gpio/export
echo 229 > /sys/class/gpio/export
echo 226 > /sys/class/gpio/export
echo 227 > /sys/class/gpio/export
echo 224 > /sys/class/gpio/export
echo 225 > /sys/class/gpio/export
echo 222 > /sys/class/gpio/export
echo 223 > /sys/class/gpio/export
echo 220 > /sys/class/gpio/export
echo 221 > /sys/class/gpio/export

echo "Set pin directions"
echo  out > /sys/class/gpio/gpio230/direction
echo  out > /sys/class/gpio/gpio231/direction
echo  out > /sys/class/gpio/gpio228/direction
echo  out > /sys/class/gpio/gpio229/direction
echo  out > /sys/class/gpio/gpio226/direction
echo  out > /sys/class/gpio/gpio227/direction
echo  out > /sys/class/gpio/gpio224/direction
echo  out > /sys/class/gpio/gpio225/direction
echo  out > /sys/class/gpio/gpio222/direction
echo  out > /sys/class/gpio/gpio223/direction
echo  in > /sys/class/gpio/gpio220/direction
echo  in > /sys/class/gpio/gpio221/direction

echo "Set initial default values"
echo 0 > /sys/class/gpio/gpio230/value
echo 0 > /sys/class/gpio/gpio231/value
echo 0 > /sys/class/gpio/gpio228/value
echo 0 > /sys/class/gpio/gpio229/value
echo 0 > /sys/class/gpio/gpio226/value
echo 0 > /sys/class/gpio/gpio227/value
echo 0 > /sys/class/gpio/gpio224/value
echo 0 > /sys/class/gpio/gpio225/value
echo 0 > /sys/class/gpio/gpio222/value
echo 0 > /sys/class/gpio/gpio223/value


