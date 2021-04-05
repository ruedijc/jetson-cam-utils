#!/bin/bash

# turn on all leds

#led1
echo 1 > /sys/class/gpio/gpio222/value
echo 1 > /var/hsi/led1_state

#led2
echo 1 > /sys/class/gpio/gpio223/value
echo 1 > /var/hsi/led2_state

#led3
echo 1 > /sys/class/gpio/gpio224/value
echo 1 > /var/hsi/led3_state

#led4
echo 1 > /sys/class/gpio/gpio225/value
echo 1 > /var/hsi/led4_state

