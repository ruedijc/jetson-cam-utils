#!/bin/bash

#
# on computer startup, set everything as an output, and put low
# set 0-7 as output, set 0-7 low
i2cset -y -f 0x00 0x77 0x06 0x00
i2cset -y -f 0x00 0x77 0x02 0x00
# set 8-15 as output, set 8-15 low
i2cset -y -f 0x00 0x77 0x07 0x00
i2cset -y -f 0x00 0x77 0x03 0x00 
# end startup
