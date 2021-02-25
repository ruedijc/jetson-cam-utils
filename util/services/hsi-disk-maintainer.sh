#!/bin/bash
#
# placeholder for "disk maintainer service"
#
# purpose: if folders or disk gets too full for some reason, go delete oldest files until x is only 70% full)
# run frequency: 10 minutes



# Optionally, set default value
#size_limit_GB="1.2"
size_limit_exp_a_folder="5.0" #size in GB
size_limit_exp_z_folder="5.0" #size in GB
size_limit_queue_folder="1.0" #size in GB
# load config values
source /etc/hsi-config/folder_size_limits


size_limit_exp_a_folder="9"

#beef="boop"
echo "size_limit_exp_a_folder is: ${size_limit_exp_a_folder}"
echo "size_limit_exp_z_folder is: ${size_limit_exp_z_folder}"          
echo "size_limit_queue_folder is: ${size_limit_queue_folder}"          

#echo "hey now is ${beef}"

sz_a="8.0"


#   1.  change to directory in question
#   2.  list files, newest first
#   3.  Run awk on output, check that it is a regular file, add file size to counter, remove file if cumulative size over 5 gigs

#cd /media/msata/task_a && ls -ltc | awk '{ if (!system("test -f " $9)) { size += $5; if (size > $size_limit_GB*2^30 ) system("ls " $9) } }'

cd /media/msata/task_a && ls -ltc | awk '{ if (!system("test -f " $9)) { size += $5; if (size > 8*2^30 ) system("rm " $9) } }'

cd /media/msata/task_z && ls -ltc | awk '{ if (!system("test -f " $9)) { size += $5; if (size > 8*2^30 ) system("ls " $9) } }'

cd /media/msata/queue && ls -ltc | awk '{ if (!system("test -f " $9)) { size += $5; if (size > 1*2^30 ) system("ls " $9) } }'

