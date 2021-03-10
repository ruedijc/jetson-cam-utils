#!/bin/bash
#
# run i2c detect to show devices, using the SMBus "read byte" commands (shown as supported by i2cdetect -F)
#
echo "i2c bus 0x00 :"
i2cdetect -y -r 0
echo "i2c bus 0x01 :"
i2cdetect -y -r 1
echo"--------------"
echo"gpio bus expander status at 77 :"
i2cdetect -y -r 0 0x77 0x77

