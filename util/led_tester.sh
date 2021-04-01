#!/bin/bash
#
# cycle through LEDs
#
echo "Cycling LEDs..."
for (( c=1; c<=10; c++ ))
#for (( ; ; ))
do
    echo 1 > LED1
    sleep 0.3
    echo 0 > LED1
    echo 1 > LED2
    sleep 0.3
    echo 0 > LED2
    echo 1 > LED3
    sleep 0.3
    echo 0 > LED3
    echo 1 > LED4
    sleep 0.3
    echo 0 > LED4
done
echo 0 > LED1
echo 0 > LED2
echo 0 > LED3
echo 0 > LED4
echo "LED test complete."
