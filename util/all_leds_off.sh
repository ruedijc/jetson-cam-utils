#!/bin/bash

# turn off all leds

/home/labuser/development/jetson-cam-utils/util/led1_off.sh
echo 0 > /var/hsi/led1_state

/home/labuser/development/jetson-cam-utils/util/led2_off.sh
echo 0 > /var/hsi/led2_state

/home/labuser/development/jetson-cam-utils/util/led3_off.sh
echo 0 > /var/hsi/led3_state

/home/labuser/development/jetson-cam-utils/util/led4_off.sh
echo 0 > /var/hsi/led4_state



