

Global control parameters
===================================

global error gathering file -
/etc/hsi-config/error_filepath
contains only the path and filename for writing out errors. any errors can concat to this file
/media/msata/logs/error_log


Eyes off - controls whether or not images are being captured
any capture mechansim should reference the value in this file before capturing/saving images
/etc/hsi-config/eyes_off
1 or 0, 1=no capturing, 0=capture enabled


functions
=====================================
prepare-for-powerloss.sh
since the TX2i will reboot automatically after a software shutdown, this script will put the device into a 'system halt' mode where disk writes stop and it's OK to remove external power. THis would be called when/if alphaspace needs to shut our system down. When power is eventually restored, the system should boot up normally (need to test & verify).



Services
==============


"hsi_task_a.service"

executes :  /home/labuser/development/jetson-cam-utils/util/services/grab_hsi_and_ctx_images.py

run frequency: 
/etc/hsi-config/hsi_task_a_frequency (int seconds)

parameters:
/etc/hsi-config/hsi_task_a.config  (contents below)
-----------------------------------------------------------------
#
# /etc/chimp/capture-config
# configuration file for nanohmics chip-scale hyperspectral imaging payload image capture
# for taking sync'ed hsi and ctx images
# 

# HSI min exposure, us 
HSI_EXPOSURE_MIN 5000

# HSI max exposure, us
HSI_EXPOSURE_MIN 5000  

# HSI set exposure to auto (1 = on 0=off)
HSI_EXPOSURE_AUTO 0  

# HSI number of exposure steps to perform from min to max ( set 1 = use AE between min and max exposure limits)
HSI_EXPOSURE_STEPS 5

# HSI number of images to capture at each exposure
HSI_SAMPLES 3

# Capture a single context camera image along with a batch of exposure captures ( 0 = don't capture Ctx image)
CTX_SYNC_CAPTURE 1

# Context camera, compressed image quality level, 10-99 save jpg, 100 = save as tif
CTX_IMG_QUALITY 95


# Save Path for image captures
HSI_SAVE_PATH 		"/media/msata/task_a/"

# Maximum size of files on disk before dropping old files, in GB
HSI_TASK_A_MAX_DISK_GB  200

#END----------------------------------------------------------------

When run, this task grabs HSI and CTX images and saves them to storage
By default, it grabs several HSI images, and a single CTX image

This task should be off when launched, and turned on once contact is established. Run for a few days, then turn off. 
Near end of mission, set hsi_save_path to a new folder, and run again


===============
task: hsi_disk_maintainer.service
executes: /home/labuser/development/jetson-cam-utils/util/services/disk_maintainer.sh
run frequency: 10 minutes
purpose: if folders or disk gets too full for some reason, go delete oldest files until x is only 70% full)

===============
task: hsi_queue_filler.service
executes: /home/labuser/development/jetson-cam-utils/util/services/hsi_queue_filler.sh
run frequency: 24 hours?
purpose: find files of interest and put into the /media/msata/queue folder to be downloaded. 
note: need some way to empty the queue. Maybe this deletes old stuff in queue and puts new stuff in.


===============
task: hsi_shutter_control.service
executes: /home/labuser/development/jetson-cam-utils/util/services/hsi_shutter_control.o
run frequency: always ensure running
parameters:
/etc/hsi-config/hsi_shutter_control_config  (contents below)
v_sun_present 	(float analog voltage reading when sun is present?)
log_values 	(0 or 1)
log_period 	(int seconds)
log_path  	(string e.g. '/media/msata/logs/sun_sensor')
purpose: monitor sun sensor and shut shutter when required
note: this should probably be a c binary that is continuously running to monitor the sun sensor.

===============
task: temp_logging.service
executes: /home/labuser/development/jetson-cam-utils/util/services/hsi_temp_logging.sh
run frequency: 
/etc/hsi-config/hsi_temp_log_sample_period  (int seconds)
parameters:
/etc/hsi-config/hsi_temp_log_config  (contents below)
log save path:
/etc/hsi-config/hsi_temp_log_path  (string e.g. '/media/msata/logs/temps')



Transient Service - these are one-shot tasks
---------------------------------------------

===============
transient Service: ctx_record_video.service
executes: /home/labuser/development/jetson-cam-utils/util/services/ctx_record_video.sh
parameters TBD:
video resolution (1920x1080 vs 3160x2140 or whatever)
recording duration (int seconds)
encoding bitrate (in bps? for h265)
purpose: make HD or 4K recordings from the context camera. Watch out for file sizes since long 4K videos could get really big.
script should probably anticipate the final size of the video and check that there is sufficient space before recording. 

===============
transient Service: hsi_clear_queue.service
executes: /home/labuser/development/jetson-cam-utils/util/services/hsi_clear_queue.sh
purpose: delete everything from the dl queue folder so new stuff can come in. not sure if this needs to be a service or could just be a rm command.


=================
take calibration pictures?







