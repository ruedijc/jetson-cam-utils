#!/bin/bash

# Set Spacely LED4 (P13-8) to 'on' state

#set the direction registers - 
#low byte (all outputs)
i2cset -y -f 0x00 0x77 0x07 0x00
#high byte (ss1, ss2 inputs (bit5,6) , rest are outputs) 0b00110000 is 0x30
i2cset -y -f 0x00 0x77 0x06 0x30

# LED3 and LED4 are on high byte, LED1 and LED2 are on low byte

#get the existing output register contents
OUTREG=$(i2cget -y -f 0x00 0x77 0x03)
printf 'existing output register high byte is : 0x%X \n' ${OUTREG}


#OR the value of the pin with the existing register values
OUTREG=$(( ${OUTREG} | 2#00000010 ))

# output bew register (should be 64 if current reg is 0)
printf 'new output register will be : 0x%X \n' ${OUTREG}

i2cset -y -f 0x00 0x77 0x03 ${OUTREG}

#get the existing output register contents to verify
NEWREG=$(i2cget -y -f 0x00 0x77 0x03)  
printf 'new output register is  : 0x%X \n' ${NEWREG}

