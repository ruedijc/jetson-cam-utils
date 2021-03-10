#!/bin/bash

#
# read SS1 which is GPIO-5 which is P13-12
# read SS2 which is GPIO-4 which is P13-11

#When this runs, set the direction registers -
#high byte (all outputs)
i2cset -y -f 0x00 0x77 0x07 0x00
#low byte (ss1, ss2 inputs (bit5,6) , rest are outputs) 0b00110000 is 0x30
i2cset -y -f 0x00 0x77 0x06 0x30


# Set Spacely P13-12,P13-11 to 'read in' state

#below are full steps to set io mode register ----

#get the existing io mode register
MODREG=$(i2cget -y -f 0x00 0x77 0x06)

printf 'existing io mode register is : 0x%X \n' ${MODREG}

#OR the intended mode register values with the existing register values
MODREG=$(( ${MODREG} | 2#00011000 ))

printf 'new io mode register is (0x30) : 0x%X \n' ${MODREG}

#read values on low byte input register
INPREG=$(i2cget -y -f 0x00 0x77 0x00)

SS1VAL=$(( ${INPREG} & 2#00100000 ))
printf 'SS1 value is : 0x%X \n' ${SS1VAL}

SS2VAL=$(( ${INPREG} & 2#00010000 ))
printf 'SS2 value is : 0x%X \n' ${SS2VAL}   

# output bew register (should be 64 if current reg is 0)
#printf 'new output register will be : 0x%X \n' ${HIREG}

#i2cset -y -f 0x00 0x77 0x03 ${HIREG}

#get the existing output register contents
#NEWREG=$(i2cget -y -f 0x00 0x77 0x03)  
#printf 'new output register is  : 0x%X \n' ${NEWREG}

