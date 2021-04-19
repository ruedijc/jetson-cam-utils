#!/bin/bash
# task13 - stop all tasks
echo "stop all capturing tasks"
sudo systemctl stop hsi_task3.timer
sudo systemctl stop hsi_task4.timer
sudo systemctl stop hsi_task9.timer
sudo systemctl stop hsi_task10.timer

# close shutter
/home/labuser/development/jetson-cam-utils/util/shutter_close.sh

# turn off all LEDs
/home/labuser/development/jetson-cam-utils/util/all_leds_off.sh
