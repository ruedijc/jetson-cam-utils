#!/bin/bash
#
# get status of hsi services
#
echo "--- System Time ---------------------------------"
echo "-------------------------------------------------"
echo -n "time: "
date
echo -n "startup: "
systemd-analyze time
echo -n "uptime:" 
uptime
echo "----Status of HSI Services ----------------------"
echo "-------------------------------------------------"
systemctl list-unit-files --state=enabled | grep hsi 
systemctl list-unit-files --state=disabled | grep hsi 
echo "----HSI Device States ---------------------------"
echo "-------------------------------------------------"
echo "LED1 state	: " $(cat /var/hsi/led1_state)
echo "LED2 state	: " $(cat /var/hsi/led2_state)
echo "LED3 state	: " $(cat /var/hsi/led3_state)
echo "LED4 state  	: " $(cat /var/hsi/led4_state)
echo "shutter state  	: " $(cat /var/hsi/shutter_state)
echo "sunsensor state	: " $(cat /var/hsi/sunsensor_state)
echo "----Storage Status ------------------------------"
echo "-------------------------------------------------"
df -h
du -h /media/msata/queue/
du -h /media/msata/task_a/
du -h /media/msata/task_z/
du -h /media/msata/vid/ 
echo "----EOF _----------------------------------------"
