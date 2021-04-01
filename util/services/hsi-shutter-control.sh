#!/bin/bash

# read the low byte of the  input status registers. 
# this should be 0x00 if no sun or 2#00110000 if both sensors active
# SS2 active :  2#0001000 or 0x10 
# SS1 active :  2#0010000 or 0x20
# both active : 2#0011000 or 0x30

#note: for testing this automatically opens the shutter when there is no sun
#      for production, probably should not do this and only open the 
#      shutter when grabbing images.


for (( ; ; ))
do


    #ensure pin io modes are set correctly -
    #high byte (all outputs)
    i2cset -y -f 0x00 0x77 0x07 0x00
    #low byte (ss1, ss2 inputs (bit5,6) , rest are outputs) 0b00110000 is 0x30
    i2cset -y -f 0x00 0x77 0x06 0x30

    #read input register that contains the two sunsensor bits
    SUNSENSOR_REGVAL=$(i2cget -y -f 0x00 0x77 0x00) 
    
    echo $SUNSENSOR_REGVAL
    
    # get only the 2 bits of interest
    #printf '%X\n' "$(( $SUNSENSOR_REGVAL & 0x30 ))"
    SUNSENSOR_DEC="$(( $SUNSENSOR_REGVAL & 0x30 ))"
    echo $SUNSENSOR_DEC

    # 0x30 = 48 = SS1, SS2 active
    # 0x20 = 32 = SS1 active   
    # 0x10 = 16 = SS2 active
    #

    if [[ "$SUNSENSOR_DEC" -eq "0" ]]
    then
       echo 0 > /var/hsi/sunsensor_state 
       echo "no sun"
       
       # production: don't do anything. testing: open the shutter, if not already open
       #/home/labuser/development/jetson-cam-utils/util/shutter_open.sh > /dev/null

    elif [[ "$SUNSENSOR_DEC" -eq "16" ]]
    then 
       echo 2 > /var/hsi/sunsensor_state 
       echo "ss2 detects sun"
       # close the shutter
       /home/labuser/development/jetson-cam-utils/util/shutter_close.sh  > /dev/null

    elif [[ "$SUNSENSOR_DEC" -eq "32" ]] 
    then  
       echo 1 > /var/hsi/sunsensor_state 
       echo "ss1 detects sun"
       # close the shutter
       /home/labuser/development/jetson-cam-utils/util/shutter_close.sh > /dev/null

    elif [[ "$SUNSENSOR_DEC" -eq "48" ]] 
    then  
       echo 3 > /var/hsi/sunsensor_state 
       echo "ss1 and ss2 detect sun"
       # close the shutter
       /home/labuser/development/jetson-cam-utils/util/shutter_close.sh > /dev/null

    else
       echo -1 > /var/hsi/sunsensor_state 
       echo "invalid sunsensor response"
       # do nothing
    fi

    sleep 30

done

