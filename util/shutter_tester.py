#!/bin/env python3
''' shutter testing program


pin assignments

Ain 1  = P13-1
Ain 2  = P13-2
Sleep  = P13-3

'''
import os
import time

''' 
turn Ain_1  on
os.system('/home/labuser/development/jetson-cam-utils/util/p13-1_on.sh')

turn Ain_1 off
os.system('/home/labuser/development/jetson-cam-utils/util/p13-1_off.sh')

turn Ain_2  on
os.system('/home/labuser/development/jetson-cam-utils/util/p13-2_on.sh')

turn Ain_2 off
os.system('/home/labuser/development/jetson-cam-utils/util/p13-2_off.sh')
'''
def open_shutter():
    os.system('/home/labuser/development/jetson-cam-utils/util/p13-3_on.sh') 	# Turn nsleep HI
    for i in range(open_pulses):
        os.system('/home/labuser/development/jetson-cam-utils/util/p13-1_on.sh') 	# Pulse HI
        time.sleep(hi_pw)								# Wait 
        os.system('/home/labuser/development/jetson-cam-utils/util/p13-1_off.sh')	# Pulse LOW
        time.sleep(low_pw)								# Wait
    os.system('/home/labuser/development/jetson-cam-utils/util/p13-3_off.sh') 	# Turn nsleep LOW

def close_shutter():
    os.system('/home/labuser/development/jetson-cam-utils/util/p13-3_on.sh') 	# Turn nsleep HI
    for i in range(close_pulses):
        os.system('/home/labuser/development/jetson-cam-utils/util/p13-2_on.sh') 	# Pulse HI
        time.sleep(hi_pw)								# Wait 
        os.system('/home/labuser/development/jetson-cam-utils/util/p13-2_off.sh')	# Pulse LOW
        time.sleep(low_pw)								# Wait
    os.system('/home/labuser/development/jetson-cam-utils/util/p13-3_off.sh') 	# Turn nsleep LOW

hi_pw = 0.75  	# 750 ms pulse width in hi state
low_pw = 0.25 	# 250 ms pulse width in low state
open_pulses = 2
close_pulses = 1
for j in range(10):
    open_shutter()
    time.sleep(5)
    close_shutter()
    time.sleep(5)
