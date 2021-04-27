#!/bin/bash
#
# placeholder for "disk maintainer service"
#
# purpose: if folders or disk gets too full for some reason, go delete oldest files until x is only 70% full)
# run frequency: 10 minutes



# Optionally, set default value (integer values only!)
#size_limit_GB="1"
size_limit_task3_folder=100 #size in GB
size_limit_task4_folder=100 #size in GB
size_limit_vid_folder=300 #size in GB
size_limit_task9_folder=100 #size in GB
size_limit_task10_folder=100 #size in GB
size_limit_queue_folder=1 #size in GB
# load config values
source /etc/hsi-config/folder_size_limits

#size_limit_queue_folder=0.5 #size in GB


echo "size_limit_task3_folder is: ${size_limit_task3_folder}"
echo "size_limit_task4_folder is: ${size_limit_task4_folder}"          
echo "size_limit_vid_folder is: ${size_limit_vid_folder}"
echo "size_limit_task9_folder is: ${size_limit_task9_folder}"
echo "size_limit_task10_folder is: ${size_limit_task10_folder}"
echo "size_limit_queue_folder is: ${size_limit_queue_folder}"          


#   1.  change to directory in question
#   2.  list files, newest first
#   3.  Run awk on output, check that it is a regular file, add file size to counter, remove file if cumulative size over 5 gigs

# example - final "ls" lists the files to be removed. change final "ls" to "rm" to actually delete the files.
# cd /media/msata/task3 && ls -ltc | awk '{ if (!system("test -f " $9)) { size += $5; if (size > $size_limit_task3_folder*2^30 ) system("rm " $9) } }'

echo "clearing files in task3...> ${size_limit_task3_folder}GB"
cd /media/msata/task3 && ls -ltc | awk -v var="$size_limit_task3_folder" '{ if (!system("test -f " $9)) { size += $5; if (size > var*2^30 ) system("rm " $9) } }'
echo "clearing files in task4... > ${size_limit_task4_folder}GB"
cd /media/msata/task4 && ls -ltc | awk -v var="$size_limit_task4_folder" '{ if (!system("test -f " $9)) { size += $5; if (size > var*2^30 ) system("rm " $9) } }'
echo "clearing files in vid... > ${size_limit_vid_folder}GB"
cd /media/msata/vid && ls -ltc | awk -v var="$size_limit_vid_folder" '{ if (!system("test -f " $9)) { size += $5; if (size > var*2^30 ) system("rm " $9) } }'
echo "clearing files in task9... > ${size_limit_task9_folder}GB"
cd /media/msata/task9 && ls -ltc | awk -v var="$size_limit_task9_folder" '{ if (!system("test -f " $9)) { size += $5; if (size > var*2^30 ) system("rm " $9) } }'
echo "clearing files in task10... > ${size_limit_task10_folder}GB"
cd /media/msata/task10 && ls -ltc | awk -v var="$size_limit_task10_folder" '{ if (!system("test -f " $9)) { size += $5; if (size > var*2^30 ) system("rm " $9) } }'
echo "clearing files in queue... > ${size_limit_queue_folder}GB"   
cd /media/msata/queue && ls -ltc | awk -v var="$size_limit_queue_folder" '{ if (!system("test -f " $9)) { size += $5; if (size > var*2^30 ) system("rm " $9) } }'


#cd /media/msata/task_a && ls -ltc | awk '{ if (!system("test -f " $9)) { size += $5; if (size > 8*2^30 ) system("rm " $9) } }'
#cd /media/msata/task_z && ls -ltc | awk '{ if (!system("test -f " $9)) { size += $5; if (size > 8*2^30 ) system("ls " $9) } }'
#cd /media/msata/queue && ls -ltc | awk '{ if (!system("test -f " $9)) { size += $5; if (size > 1*2^30 ) system("ls " $9) } }'

