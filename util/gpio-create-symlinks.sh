
#!/bin/bash
#
# Create symlinks to GPIO values

ln -s  /sys/class/gpio/gpio230/value AIN2
ln -s  /sys/class/gpio/gpio231/value AIN1
ln -s  /sys/class/gpio/gpio228/value BIN2
ln -s  /sys/class/gpio/gpio229/value BIN1
ln -s  /sys/class/gpio/gpio226/value NFAULT
ln -s  /sys/class/gpio/gpio227/value NSLEEP
ln -s  /sys/class/gpio/gpio224/value LED3
ln -s  /sys/class/gpio/gpio225/value LED4
ln -s  /sys/class/gpio/gpio222/value LED1
ln -s  /sys/class/gpio/gpio223/value LED2
ln -s  /sys/class/gpio/gpio220/value SS2
ln -s  /sys/class/gpio/gpio221/value SS1
