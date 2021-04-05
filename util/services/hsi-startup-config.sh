#!/bin/bash

#
# on computer startup, set everything as an output, and put low
# set low byte ss1, ss2 inputs (bit5,6) , rest are outputs - 0b00110000 is 0x30
# set low byte outputs to off
# (removing -f flag, could cause problems?)
i2cset -y  0x00 0x77 0x06 0x30
i2cset -y  0x00 0x77 0x02 0x00
# set 8-15 as output, set 8-15 low
i2cset -y  0x00 0x77 0x07 0x00
i2cset -y  0x00 0x77 0x03 0x00 
# end startup

