Services
==============


"hsi_task_a.service"

executes :  /home/labuser/development/jetson-cam-utils/util/services/grab_hsi_and_ctx_images.py

run frequency: 
/etc/hsi-config/hsi_task_a_frequency (int seconds)

parameters:
/etc/hsi-config/hsi_task_a_config  (contents below)

[hsi_task_config]
# hsi exposure settings
hsi_exposure_auto 	(0 or 1)

# number of samples of hsi images (at each exposure if multiple exposures)
hsi_samples		(int samples)

# if not auto exposure, use these settings
hsi_exposure_min 	(long nanoseconds)
hsi_exposure_min 	(long nanoseconds)
hsi_exposure_steps (int number of different exposures to sample)

# operation modes
hsi_save_path 		('string' eg '/mega/msata/task_a/' )
hsi_task_a_frequency  	(in seconds)
hsi_task_a_max_disk_MB 	(float MB)    (if hsi_save_path gets to max_disk_MB, delete oldest entries in hsi_save_path

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







