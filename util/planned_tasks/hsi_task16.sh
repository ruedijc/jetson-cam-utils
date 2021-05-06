#/bin/bash

# hsi_task16.sh - backup to sdcard

# save in /home/labuser/tasks and chmod +x
# ensure labuser has ownership of sdcard
sudo chown labuser:labuser /media/sd/
chmod +xr /media/sd/
# make dirs for storage if not there
mkdir /media/sd/task3
mkdir /media/sd/task4
mkdir /media/sd/task9
mkdir /media/sd/task10
#if they are there, delete old stuff
rm -r /media/sd/task3/*
rm -r /media/sd/task4/*
rm -r /media/sd/task9/*
rm -r /media/sd/task10/*
# now copy msata into sdcard (should be max of 400GB)
cp -r /media/msata/task3/* /media/sd/task3/
cp -r /media/msata/task4/* /media/sd/task4/
cp -r /media/msata/task9/* /media/sd/task9/
cp -r /media/msata/task10/* /media/sd/task10/
