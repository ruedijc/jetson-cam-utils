#!/bin/bash
# task13 - stop all tasks
echo "stop all tasks"
systemctl stop hsi*.service

# close shutter
/home/labuser/jetson-cam-utils/util/shutter_close.sh

# turn off all LEDs
/home/labuser/jetson-cam-utils/util/all_leds_off.sh
