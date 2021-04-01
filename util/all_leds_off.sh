#!/bin/bash

# turn off all leds

echo 0 > /sys/class/gpio/gpio222/value
echo 0 > /var/hsi/led1_state

echo 0 > /sys/class/gpio/gpio223/value
echo 0 > /var/hsi/led2_state

echo 0 > /sys/class/gpio/gpio224/value
echo 0 > /var/hsi/led3_state

echo 0 > /sys/class/gpio/gpio225/value
echo 0 > /var/hsi/led4_state



