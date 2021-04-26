#!/bin/bash
echo "starting hsi_failsafe_cap.timer..."
sudo systemctl start hsi_failsafe_cap.timer
sleep 7m # waits 7 minutes for cap to finish so these tasks are offset
echo "starting hsi_failsafe_cal.timer..."
sudo systemctl start hsi_failsafe_cal.timer
