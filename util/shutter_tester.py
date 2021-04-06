#!/usr/bin/env python3
''' shutter testing program
    open/close the shutter 10 times
'''
import os
import time

def close_shutter():
    print('Close shutter')
    os.system('/home/labuser/development/jetson-cam-utils/util/shutter_close.sh') 	# Pulse HI

def open_shutter():
    print('Open Shutter')
    os.system('/home/labuser/development/jetson-cam-utils/util/shutter_open.sh') 	# Pulse HI


for j in range(10):
    open_shutter()
    time.sleep(1)
    close_shutter()
    time.sleep(1)
