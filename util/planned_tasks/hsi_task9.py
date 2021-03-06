#!/usr/bin/env python3
from ximea import xiapi  # camera calls
import PIL.Image  # for image saving
from datetime import datetime  # for timestamping
import time
import configparser  # to read config file
import os  # for shutter script calls
import numpy as np

# import gi
# gi.require_version('Gst', '1.0')
# from gi.repository import Gst, GObject
# import sys #used by Gst

t = time.time()  # get start time for end-to-end performance checks

# load configuration parameters from file -
config = configparser.ConfigParser(allow_no_value=True)
config.sections()
config.read('/etc/hsi-config/hsi_task9.ini')

hsi_exposure_min = int(config['hsi']['HSI_EXPOSURE_MIN'])
hsi_exposure_max = int(config['hsi']['HSI_EXPOSURE_MAX'])
hsi_exposure_auto = int(config['hsi']['HSI_EXPOSURE_AUTO'])
hsi_exposure_steps = int(config['hsi']['HSI_EXPOSURE_STEPS'])
hsi_samples = int(config['hsi']['HSI_SAMPLES'])
hsi_save_path = str(config['filesys']['hsi_save_path'])
hsi_save_path = hsi_save_path.strip('\"')  # strip out double quotes
hsi_task_max_disk_gb = float(config['filesys']['hsi_task_max_disk_gb'])

hsi_dwell_msec = int(config['hsi']['HSI_DWELL_MS'])

print('hsi-config/hsi_task9.ini  configuration:')
print(f'Xi Min Exposure: {hsi_exposure_min}')
print(f'Xi Max Exposure: {hsi_exposure_max}')
print(f'Xi Exposure Auto: {hsi_exposure_auto}')
print(f'Xi Exposure Steps: {hsi_exposure_steps}')
print(f'Xi HSI Samples: {hsi_samples}')
print(f'Save Path: {hsi_save_path}')
print(f'Max size on disk in GB: {hsi_task_max_disk_gb}')
print(f'hsi dwell time, ms: {hsi_dwell_msec}')

# close the shutter!
os.system("/home/labuser/development/jetson-cam-utils/util/shutter_close.sh")
time.sleep(2.0)

# set all LEDs off 
os.system("/home/labuser/development/jetson-cam-utils/util/all_leds_off.sh")
# setup internal led state holders
led1_state = 0
led2_state = 0
led3_state = 0
led4_state = 0


# define the LED combo setting function
def set_led(led_id, led_state):
    global led1_state
    global led2_state
    global led3_state
    global led4_state

    if led_id == 1:
        # see Spacely GPIO breakout map for sysfs assignments
        if led_state == 0:
            os.system("echo 0 > /sys/class/gpio/gpio222/value")
            print("LED1 off")
            led1_state = 0
        elif led_state == 1:
            os.system("echo 1 > /sys/class/gpio/gpio222/value")
            print("LED1 on")
            led1_state = 1
        else:
            print("no action")

    elif led_id == 2:
        if led_state == 0:
            os.system("echo 0 > /sys/class/gpio/gpio223/value")
            print("LED2 off")
            led2_state = 0
        elif led_state == 1:
            os.system("echo 1 > /sys/class/gpio/gpio223/value")
            print("LED2 on")
            led2_state = 1
        else:
            print("no action")

    elif led_id == 3:
        if led_state == 0:
            os.system("echo 0 > /sys/class/gpio/gpio224/value")
            print("LED3 off")
            led3_state = 0
        elif led_state == 1:
            os.system("echo 1 > /sys/class/gpio/gpio224/value")
            print("LED3 on")
            led3_state = 1
        else:
            print("no action")

    elif led_id == 4:
        if led_state == 0:
            os.system("echo 0 > /sys/class/gpio/gpio225/value")
            print("LED4 off")
            led4_state = 0
        elif led_state == 1:
            os.system("echo 1 > /sys/class/gpio/gpio225/value")
            print("LED4 on")
            led4_state = 1
        else:
            print("no action")
    else:
        print("invalid LED number. no action")

    # wait 1 second for LED to change
    time.sleep(1.0)


# set of conditions to test. row values are [ LED1 LED2 LED3 LED4 ]
# subset of set of 4 (should be n^2 values!)
led_conditions = [[0, 0, 0, 0],
                  [1, 0, 0, 0],
                  [0, 1, 0, 0],
                  [0, 0, 1, 0],
                  [0, 0, 0, 1],
                  [1, 1, 0, 0],
                  [1, 0, 1, 0],
                  [1, 0, 0, 1],
                  [0, 1, 1, 0],
                  [0, 1, 0, 1],
                  [0, 0, 1, 1],
                  [1, 1, 1, 0],
                  [0, 1, 1, 1],
                  [1, 0, 1, 1],
                  [1, 1, 0, 1],
                  [1, 1, 1, 1],
                  [0, 0, 0, 0]
                  ]

bit_depth = 16  # set bit depth, 8 or 16, assumes mono
exposure_start = hsi_exposure_min  # 10k = 10ms
exposure_end = hsi_exposure_max  # 100k= 100ms
num_captures = hsi_samples  # number of captures at each exposures
num_exposures = hsi_exposure_steps  # number of exposures

exposure_list = []

if num_exposures > 1:
    for i in range(num_exposures):
        print(i)
        exposure_list.append(exposure_start + (exposure_end - exposure_start) * (i / (num_exposures - 1)))
else:
    exposure_list.append(exposure_end)

print("Exposures to sample:", exposure_list[:])

# create instance for first connected camera
cam = xiapi.Camera()

# start communication
print('Opening first camera...')
cam.open_device()

# settings
if bit_depth == 16:
    cam.set_imgdataformat('XI_MONO16')
else:
    cam.set_imgdataformat('XI_MONO8')

# create instance of Image to store image data and metadata
ximg = xiapi.Image()

# start data acquisition
print('Starting data acquisition...')
cam.start_acquisition()

if hsi_exposure_auto == 1:

    print("System set to use autoexposure")
    # set to autoexposure
    cam.enable_aeag()
    # set to exposure priority (vs gain priority)
    cam.set_exp_priority(1.0)
    # set auto exposure max limit to 1 second (in usec)
    cam.set_ae_max_limit(1000000)

    # take 10 pre-roll images to allow autoexposure to work if active
    for x in range(10):
        # get data and pass them from camera to img
        cam.get_image(ximg)

    print('Current exposure is %s us.' % cam.get_exposure())
    print('Current gain is %s' % cam.get_gain())

    # Cycle over all LED test conditions
    for i in range(0, len(led_conditions), 1):

        # set the LED outputs
        # e.g. led_conditions[i] = [ 1 1 1 1]
        set_led(1, led_conditions[i][0])
        set_led(2, led_conditions[i][1])
        set_led(3, led_conditions[i][2])
        set_led(4, led_conditions[i][3])

        print("testing condition:")
        print("led1: " + str(led_conditions[i][0]))
        print("led2: " + str(led_conditions[i][1]))
        print("led3: " + str(led_conditions[i][2]))
        print("led4: " + str(led_conditions[i][3]))

        # !!! IMPORTANT !!!
        # wait for dwell time, for camera autexposure to adjust...
        # otherwise, you may get cross-talk between light values
        time.sleep(hsi_dwell_msec / 1000)

        # stamp current time
        ts = datetime.now()
        # tempC = 4021
        temp_centiK = int((273.15 + cam.get_temp()) * 100)

        img_data = []
        for j in range(num_captures):
            # get data and pass them from camera to img
            cam.get_image(ximg)

            real_exp = cam.get_exposure()  # get real exposure vs commanded exp

            # create numpy array with data from camera. Dimensions of array are determined
            # by imgdataformat
            data = ximg.get_image_data_numpy()

            img_data.append(data)

            print('End exposure is %s us.' % real_exp)  # %cam.get_exposure())
            print('End gain is %s' % cam.get_gain())

            print('Timestamp was : ', ts.isoformat())
        img_data_mean = np.nanmean(img_data, axis=0)
        # img_data_std = np.std(img_data, axis=0)
        # output_data = np.zeros((2, *img_data_mean.shape))
        # output_data[0] = img_data_mean
        # output_data[1] = img_data_std

        fname = hsi_save_path + 'hsi_cal_' + ts.strftime("%Y-%m-%d-%H-%M-%S.%f") + '_' + \
                str(real_exp) + 'us_' + \
                str(led1_state) + str(led2_state) + str(led3_state) + str(led4_state) + '_' + \
                str(temp_centiK) + \
                '.npz'
        np.savez_compressed(str(fname), img_data_mean)

        time.sleep(
            real_exp / 1000000.0)  # sleep for an exposure time to ensure next pic is new
        # time.sleep(real_exp/1000000.0) #sleep for an exposure time to ensure next pic is new

        # done saving single sample

        # wait for changing LEDs
        time.sleep(hsi_dwell_msec / 1000)
        # done with number of capures

    # done with seqeunce of LEDs

else:

    print("System in static exposure mode")
    cam.disable_aeag()
    cam.disable_auto_wb()

    # Cycle over all LED test conditions
    for i in range(0, len(led_conditions), 1):

        # set the LED outputs
        # e.g. led_conditions[i] = [ 1 1 1 1]
        set_led(1, led_conditions[i][0])
        set_led(2, led_conditions[i][1])
        set_led(3, led_conditions[i][2])
        set_led(4, led_conditions[i][3])

        print("testing condition:")
        print("led1: " + str(led_conditions[i][0]))
        print("led2: " + str(led_conditions[i][1]))
        print("led3: " + str(led_conditions[i][2]))
        print("led4: " + str(led_conditions[i][3]))

        # !!! IMPORTANT !!!
        # wait for dwell time, for camera autexposure to adjust...
        # otherwise, you may get cross-talk between light values
        time.sleep(hsi_dwell_msec / 1000)

        for exp in exposure_list:
            print("exposure: ", exp)

            # cam.disable_aeag()

            try:
                cam.set_exposure(exp)  # in us
            except:
                print("Failed to set cam exposure - is there a ximea connection problem?")

            # wait 2x the exposure time to allow for camera to adjust
            # time.sleep(2.0 * (exp/1000000))

            print('Current exposure is %s us.' % cam.get_exposure())
            print('Current gain is %s' % cam.get_gain())

            # take 3 pre-roll images to allow autoexposure to work if active
            for x in range(3):
                # get data and pass them from camera to img
                cam.get_image(ximg)

            img_data = []
            for j in range(num_captures):
                # get data and pass them from camera to img
                cam.get_image(ximg)
                # stamp current time

                # tempC = 4021
                temp_centiK = int((273.15 + cam.get_temp()) * 100)
                real_exp = cam.get_exposure()  # get real exposure vs commanded exp

                # create numpy array with data from camera. Dimensions of array are determined
                # by imgdataformat
                data = ximg.get_image_data_numpy()

                img_data.append(data)

                time.sleep(
                    2.0 * real_exp / 1000000.0)  # sleep for 2  exposure times to ensure next pic is new

                print('End exposure is %s us.' % real_exp)  # %cam.get_exposure())
                print('End gain is %s' % cam.get_gain())
                ts = datetime.now()
                print('Timestamp was : ', ts.isoformat())

            img_data_mean = np.nanmean(img_data, axis=0)
            # img_data_std = np.std(img_data, axis=0)
            # output_data = np.zeros((2, *img_data_mean.shape))
            # output_data[0] = img_data_mean
            # output_data[1] = img_data_std

            fname = hsi_save_path + 'hsi_cal_' + ts.strftime("%Y-%m-%d-%H-%M-%S.%f") + '_' + \
                    str(real_exp) + 'us_' + \
                    str(led1_state) + str(led2_state) + str(led3_state) + str(led4_state) + '_' + \
                    str(temp_centiK) + \
                    '.npz'
            np.savez_compressed(str(fname), img_data_mean)

            # done with samples
        # done with exposure list
        # wait before changing LEDs
        time.sleep(hsi_dwell_msec / 1000)
    # done with LED combos

# stop data acquisition
print('Stopping acquisition...')
cam.stop_acquisition()

# stop communication
cam.close_device()

# before finishing, close the shutter
os.system("/home/labuser/development/jetson-cam-utils/util/shutter_close.sh")

# img.show()

elapsed = time.time() - t

print('Done. Elapsed time: ' + str(elapsed))
