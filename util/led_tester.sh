#!/bin/bash
#
# cycle through LEDs
#
echo "Cycling LEDs..."
for (( c=1; c<=10; c++ ))
#for (( ; ; ))
do

    echo 1 > /sys/class/gpio/gpio222/value
    sleep 0.3
    echo 0 > /sys/class/gpio/gpio222/value
    echo 1 > /sys/class/gpio/gpio223/value
    sleep 0.3
    echo 0 > /sys/class/gpio/gpio223/value
    echo 1 > /sys/class/gpio/gpio224/value
    sleep 0.3
    echo 0 > /sys/class/gpio/gpio224/value
    echo 1 > /sys/class/gpio/gpio225/value
    sleep 0.3
    echo 0 > /sys/class/gpio/gpio225/value

done
echo 0 > /sys/class/gpio/gpio222/value
echo 0 > /sys/class/gpio/gpio223/value
echo 0 > /sys/class/gpio/gpio224/value
echo 0 > /sys/class/gpio/gpio225/value
echo "LED test complete."
