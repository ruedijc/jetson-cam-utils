#!/bin/bash

# turn on all leds

/home/labuser/development/jetson-cam-utils/util/led1_on.sh
echo 1 > /var/hsi/led1_state

/home/labuser/development/jetson-cam-utils/util/led2_on.sh
echo 1 > /var/hsi/led2_state

/home/labuser/development/jetson-cam-utils/util/led3_on.sh
echo 1 > /var/hsi/led3_state

/home/labuser/development/jetson-cam-utils/util/led4_on.sh
echo 1 > /var/hsi/led4_state



