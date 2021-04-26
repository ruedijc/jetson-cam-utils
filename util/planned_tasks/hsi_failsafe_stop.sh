#!/bin/bash
echo "stopping hsi_failsafe_cap.timer..."
sudo systemctl stop hsi_failsafe_cap.timer
echo "stopping hsi_failsafe_cal.timer..."
sudo systemctl stop hsi_failsafe_cal.timer
