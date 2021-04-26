#!/bin/bash
#
# placeholder for "temperature logging"
#
# run frequency: every 3 minutes?
# purpose: find files of interest and put into the /media/msata/queue folder to be downloaded. 
#
# run frequency: 
# /etc/hsi-config/hsi_temp_log_sample_period  (int seconds)
#
# parameters:
# /etc/hsi-config/hsi_temp_log_config  (contents below)
#
# log save path:
# /etc/hsi-config/hsi_temp_log_path  (string e.g. '/media/msata/logs/temps')
#
SS1VAL=$( cat /sys/class/gpio/gpio221/value )
#printf 'SS1 value is : %d \n' ${SS1VAL}

SS2VAL=$( cat /sys/class/gpio/gpio220/value )
#printf 'SS2 value is : %d \n' ${SS2VAL}   


SHUTTER_STATE=$( cat /var/hsi/shutter_state )
#printf 'SHUTTER_STATE value is : %d \n' ${SHUTTER_STATE}

T0=$( cat  /sys/devices/virtual/thermal/thermal_zone0/temp )
#printf 'T0 value is : %d \n' ${T0}

T1=$( cat  /sys/devices/virtual/thermal/thermal_zone1/temp )
#printf 'T1 value is : %d \n' ${T1}

T2=$( cat  /sys/devices/virtual/thermal/thermal_zone2/temp )
#printf 'T2 value is : %d \n' ${T2}

T3=$( cat  /sys/devices/virtual/thermal/thermal_zone3/temp )
#printf 'T3 value is : %d \n' ${T3}

T4=$( cat  /sys/devices/virtual/thermal/thermal_zone4/temp )
#printf 'T4 value is : %d \n' ${T4}

T5=$( cat  /sys/devices/virtual/thermal/thermal_zone5/temp )
#printf 'T5 value is : %d \n' ${T5}

#T6=$( cat  /sys/devices/virtual/thermal/thermal_zone6/temp )
#printf 'T6 value is : %d \n' ${T6}

T7=$( cat  /sys/devices/virtual/thermal/thermal_zone7/temp )
#printf 'T7 value is : %d \n' ${T7}


TIME=$(date '+%Y-%m-%d.%H-%M-%S')

FILE="/media/msata/queue/temp_log.txt"

echo  "$TIME, sun: $SS1VAL,$SS2VAL,$SHUTTER_STATE, temps: $T0,$T1,$T2,$T3,$T4,$T5,$T7" >> $FILE

