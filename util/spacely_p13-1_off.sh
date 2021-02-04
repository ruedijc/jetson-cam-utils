#!/bin/bash
#
# this sets P13-1  on CTI Spacely carrier , and turns P13-1/ sysfs 486 / GPIO 14 to 'off' state


# NOTE: carrier P13-1 = P16 on ti i2c expander, which is controlled by 8-15 registers

# carrier pin : P13-1
# tca9539 pin : P16
# sysfs number: 486
# i2c address : 0x77
# dir register: 0x07
# in register : 0x01
# out register: 0x03


# using i2c access to 0x77 the GPIO breakout chip:
# i2cset -y -f <bus> <addr> <regiser> <data>
# regsiters for 0x77:
#   0x00 input register, 0-7
#   0x01 input register,  8-15
#   0x02 output register, 0-7
#   0x03 output resister, 8-15
#   0x06 io direction, 0-7
#   0x07 io direction, 8-15

i2cset -y -f 0x00 0x77 0x07 0x00  #set direction register P13-1 to P13-8 as outputs


i2cset -y -f 0x00 0x77 0x03 0x00  #set output register all off for P13-1 to P13-8



 

