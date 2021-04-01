#!/bin/bash

#
# read SS1 which is GPIO-5 which is P13-12, sysfs 221
# read SS2 which is GPIO-4 which is P13-11, sysfs 220


SS1VAL=$( cat /sys/class/gpio/gpio221/value )
printf 'SS1 value is : %X \n' ${SS1VAL}

SS2VAL=$( cat /sys/class/gpio/gpio220/value )
printf 'SS2 value is : %X \n' ${SS2VAL}   



