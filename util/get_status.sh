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
echo "-filesystem--------------------------------------"
df -h
echo "-external storage--------------------------------"
du -h --max-depth=1 /media/msata
du -h --max-depth=1 /media/sd 
echo "----EOF _----------------------------------------"
