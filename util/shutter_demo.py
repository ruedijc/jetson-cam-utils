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
import time, sched
import subprocess

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
    os.system('/home/labuser/development/jetson-cam-utils/util/p13-4_on.sh') 	# Pulse HI
    time.sleep(hi_pw)								# Wait 
    os.system('/home/labuser/development/jetson-cam-utils/util/p13-4_off.sh')	# Pulse LOW

def open_shutter():
    print('Open Shutter')
    os.system('/home/labuser/development/jetson-cam-utils/util/p13-3_on.sh') 	# Pulse HI
    time.sleep(hi_pw)								# Wait 
    os.system('/home/labuser/development/jetson-cam-utils/util/p13-3_off.sh')	# Pulse LOW

def check_sunsensor():
    SS1, SS2 = False, False
    p = subprocess.check_output('i2cget -y -f 0x00 0x77 0x00', shell = True) # Read inputs as Hex
    reg = bin(int(p,16)).zfill(8)    					     # Convert Hex to Binry
    SS1, SS2 = int(reg[3]), int(reg[2])				     # Set Sunsensor Bool
    print('Sunsensor: ',SS1, SS2, reg, bool('0'))
    #if SS1 or SS2:
     #   close_shutter()
    return SS1, SS2


hi_pw = 1.0  	# 750 ms pulse width in hi state
low_pw = 0.25 	# 250 ms pulse width in low state
open_pulses = 5
close_pulses = 5
sun = False
counts = 5




os.system('/home/labuser/development/jetson-cam-utils/util/p13-6_on.sh') 	# Turn nsleep HI

starttime = time.time()
total_time = 0.0
shutter_open = False
time_err = 0.01
open_interval = 10.0
close_interval = 10.0

while total_time < 600.0:
    SS1, SS2 = False, False
    total_time = (time.time() - starttime)
    print('Time Elapsed: ', total_time)
    SS1, SS2 = check_sunsensor()
    openTime = (total_time%open_interval <= time_err)
    SS_okay = (SS1 == 0) and (SS2 == 0)
    print(openTime,shutter_open, SS_okay)
    if (SS1 or SS2) and shutter_open:
        print('\n\n Sunsensor Closing Shutter!\n\n')
        close_shutter()
        shutter_open = False
    elif  openTime and (shutter_open == False) and SS_okay:
        print('\n\n Safe to Open Shutter!\n\n')
        open_shutter()
        shutter_open = True
    elif openTime and shutter_open:
        print('\n\n Closing the Shutter!\n\n')
        close_shutter()
        shutter_open = False


    time.sleep(0.5 - ((time.time() - starttime) % 0.5))


os.system('/home/labuser/development/jetson-cam-utils/util/p13-6_off.sh') 	# Turn nsleep LOW
