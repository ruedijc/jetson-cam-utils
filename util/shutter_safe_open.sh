#!/bin/bash
#
# open shutter if no sun is detected
# CAUTION: only checks for sun at opening. User is required to check if task is long
# CAUTION: user is responsible for closing shutter after use
#


#read two sensor GPIO inputs
# 0 == no sun, 1 == sun
SUNSENSOR_2VAL=$(cat /sys/class/gpio/gpio220/value) 
SUNSENSOR_1VAL=$(cat /sys/class/gpio/gpio221/value) 

echo $SUNSENSOR_2VAL
echo $SUNSENSOR_1VAL

if [[ "$SUNSENSOR_1VAL" -eq "1" ]]
then
    echo 1 > /var/hsi/sunsensor_state 
    echo "ss1 detects sun"
    /home/labuser/development/jetson-cam-utils/util/shutter_close.sh  > /dev/null


elif [[ "$SUNSENSOR_2VAL" -eq "1" ]]
then 
    echo 2 > /var/hsi/sunsensor_state 
    echo "ss2 detects sun"
    # close the shutter
    /home/labuser/development/jetson-cam-utils/util/shutter_close.sh  > /dev/null

else
    echo 0 > /var/hsi/sunsensor_state 
    echo "sun not detected. opening shutter"
    # open the shutter
    /home/labuser/development/jetson-cam-utils/util/shutter_open.sh  > /dev/null

fi









