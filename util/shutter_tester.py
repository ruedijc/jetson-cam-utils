#!/bin/env python3
''' shutter testing program


pin assignments

Ain 1  = P13-1
Ain 2  = P13-2
Sleep  = P13-6

Bin 1  = P13-3
Bin 2  = P13-4

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
def close_shutter():
    print('Close shutter')
#    for i in range(close_pulses):
#    print('Pulse: ',i)
    os.system('/home/labuser/development/jetson-cam-utils/util/p13-3_on.sh') 	# Pulse HI
    time.sleep(2)								# Wait 
    os.system('/home/labuser/development/jetson-cam-utils/util/p13-3_off.sh')	# Pulse LOW
#    time.sleep(low_pw)								# Wait
    

def open_shutter():
    print('Open Shutter')
    #for i in range(open_pulses):
#    print('Pulse: ',i)
    os.system('/home/labuser/development/jetson-cam-utils/util/p13-4_on.sh') 	# Pulse HI
    time.sleep(2)								# Wait 
    os.system('/home/labuser/development/jetson-cam-utils/util/p13-4_off.sh')	# Pulse LOW
        #time.sleep(low_pw)								# Wait

hi_pw = 0.75  	# 750 ms pulse width in hi state
low_pw = 0.25 	# 250 ms pulse width in low state
open_pulses = 5
close_pulses = 5

os.system('/home/labuser/development/jetson-cam-utils/util/p13-6_on.sh') 	# Turn nsleep HI
for j in range(10):
    open_shutter()
    time.sleep(1)
    close_shutter()
    time.sleep(1)

os.system('/home/labuser/development/jetson-cam-utils/util/p13-6_off.sh') 	# Turn nsleep LOW
