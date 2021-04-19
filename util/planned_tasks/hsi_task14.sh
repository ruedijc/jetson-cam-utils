#!/bin/bash
# task14 - prepare for powerloss
echo "prepare for powerloss"

# close shutter
/home/labuser/development/jetson-cam-utils/util/shutter_close.sh

# turn off all LEDs
/home/labuser/development/jetson-cam-utils/util/all_leds_off.sh

#system halt
sudo halt
