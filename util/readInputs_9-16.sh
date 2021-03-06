#!/bin/bash

# Set Spacely P13-9 to 'read in' state

#set the direction register P13-1-16 as inputs
i2cset -y -f 0x00 0x77 0x07 0x01

#get the existing output register contents
OUTREG=$(i2cget -y -f 0x00 0x77 0x00)
printf 'existing output register is : 0x%X \n' ${OUTREG}

#AND the value of the pin with the existing register values
#HIREG=$(( ${OUTREG} | 2#01000000 ))

# output bew register (should be 64 if current reg is 0)
#printf 'new output register will be : 0x%X \n' ${HIREG}

#i2cset -y -f 0x00 0x77 0x03 ${HIREG}

#get the existing output register contents
#NEWREG=$(i2cget -y -f 0x00 0x77 0x03)  
#printf 'new output register is  : 0x%X \n' ${NEWREG}

