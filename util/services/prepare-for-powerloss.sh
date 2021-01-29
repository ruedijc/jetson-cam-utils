#!/bin/bash
#
# put this system in a safe state that can accept external loss of power
# 
# The TX2i will auto-reboot when power is provided so normal 'shutdown now' will result in a reboot
# Halting the system will stop disk writes and sleep the system so it should be safe to remote external power.
# When power is restored the system should reboot normally.
#

sudo shutdown -h now

