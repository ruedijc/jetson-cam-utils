#!/bin/bash
#
#
# since thetx2i cannot be permanently shutdown without removing power,
# its PMIC will automatically reboot after a shutdown event.
#
# to prepare for the hard-removal of power, this script sets the
# system to run level 0 (system halt) which is not quite shudown.
#
# once this is run, the system power must be removed and reapplied to start again
#
#
halt

