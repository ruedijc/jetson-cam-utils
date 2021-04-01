#!/bin/bash
#
# cycle through LEDs
#
echo "Cycling LEDs..."
#for (( c=1; c<=10; c++ ))
for (( ; ; ))
do
    ./led1_on.sh
    sleep 5
    ./led1_off.sh
    ./led2_on.sh
    sleep 5
    ./led2_off.sh
    ./led3_on.sh
    sleep 5
    ./led3_off.sh
    ./led4_on.sh
    sleep 5
    ./led4_off.sh
done
echo "LED test complete."
