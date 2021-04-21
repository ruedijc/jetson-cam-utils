#!/usr/bin/env python3
from ximea import xiapi # camera calls
import PIL.Image # for image saving
from datetime import datetime # for timestamping
import time
import configparser   # to read config file


import os #used by sysfs GPIO

t=time.time() # get start time for end-to-end performance checks

#load configuration parameters from file -
config = configparser.ConfigParser(allow_no_value=True)
config.sections()
config.read('/etc/hsi-config/hsi_task9.ini')

hsi_exposure = int(config['hsi']['HSI_EXPOSURE'])
hsi_samples = int(config['hsi']['HSI_SAMPLES'])
hsi_cal_save_path = str(config['filesys']['HSI_CAL_SAVE_PATH'])
hsi_cal_save_path = hsi_cal_save_path.strip('\"') #strip out double quotes
hsi_task_max_disk_gb = float(config['filesys']['HSI_CAL_TASK_MAX_DISK_GB'])
hsi_cal_dwell_msec = int(config['hsi']['HSI_CAL_DWELL_MS'])

print('hsi-config/hsi_task9.ini  configuration:')
print(f'Xi cal Exposure: {hsi_exposure}')
print(f'Xi cal HSI Samples: {hsi_samples}')
print(f'Cal Save Path: {hsi_cal_save_path}')
print(f'Max size on disk in GB: {hsi_task_max_disk_gb}')
print(f'Dwell time between conditions in msec: {hsi_cal_dwell_msec}')

# try to mkdir for save path, incase it doesn't already exist
#test:
print( "mkdir " + str(hsi_cal_save_path) )
os.system("mkdir " + str(hsi_cal_save_path) )


#setup internal led state holders
led1_state = 0
led2_state = 0
led3_state = 0
led4_state = 0

def set_led(n,x):
    global led1_state
    global led2_state
    global led3_state
    global led4_state

    if (n==1) :
        #see Spacely GPIO breakout map for sysfs assignments
        if(x==0):
            os.system("echo 0 > /sys/class/gpio/gpio222/value")
            print("LED1 off")
            led1_state = 0
        elif(x==1):
            os.system("echo 1 > /sys/class/gpio/gpio222/value")
            print("LED1 on")
            led1_state = 1
        else:
            print("no action")

    elif (n==2) :
        if(x==0):
            os.system("echo 0 > /sys/class/gpio/gpio223/value")
            print("LED2 off")
            led2_state = 0
        elif(x==1):
            os.system("echo 1 > /sys/class/gpio/gpio223/value")
            print("LED2 on")
            led2_state = 1
        else:
            print("no action")

    elif (n==3) :
        if(x==0):
            os.system("echo 0 > /sys/class/gpio/gpio224/value")
            print("LED3 off")
            led3_state = 0
        elif(x==1):
            os.system("echo 1 > /sys/class/gpio/gpio224/value")
            print("LED3 on")
            led3_state = 1
        else:
            print("no action")

    elif (n==4) :
        if(x==0):
            os.system("echo 0 > /sys/class/gpio/gpio225/value")
            print("LED4 off")
            led4_state = 0
        elif(x==1):
            os.system("echo 1 > /sys/class/gpio/gpio225/value")
            print("LED4 on")
            led4_state = 1
        else:
            print("no action")
    else :
        print("invalid LED number. no action")



#before continuing, close the shutter
os.system("/home/labuser/development/jetson-cam-utils/util/shutter_close.sh")


depth = 16 # set bit depth, 8 or 16, assumes mono
num_captures = hsi_samples # number of captures at each condition

#create instance for first connected camera 
cam = xiapi.Camera()

#start communication
print('Opening first camera...')
cam.open_device()

#settings
if (depth==16):
    cam.set_imgdataformat('XI_MONO16')
else:
    cam.set_imgdataformat('XI_MONO8')


#create instance of Image to store image data and metadata
ximg = xiapi.Image()


#start data acquisition
print('Starting image acquisition...')
cam.start_acquisition()


if (hsi_exposure > 3 and  hsi_exposure < 1000001 ) :
    #requesting a valid fixed exposure
    cam.disable_aeag()
    cam.set_exposure(hsi_exposure)
else :
    #invalid exposure or -1. set to autoexposure
    cam.enable_aeag()
    #set to exposure priority (vs gain priority)
    cam.set_exp_priority(1.0)  
    #set auto exposure max limit to 1 second (in usec)
    cam.set_ae_max_limit(1000000)


# set LED initial Condition
set_led(1,0)
set_led(2,0)
set_led(3,0)
set_led(4,0)


#set of conditions to test. row values are [ LED1 LED2 LED3 LED4 ]
# subset of set of 4 (should be n^2 values!)
led_conditions = [  [0, 0, 0, 0], 
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


#Cycle over all LED test conditions
for i in range(0, len(led_conditions), 1) :

    # set the LED outputs
    #e.g. led_conditions[i] = [ 1 1 1 1]
    set_led(1, led_conditions[i][0] )
    set_led(2, led_conditions[i][1] )
    set_led(3, led_conditions[i][2] )
    set_led(4, led_conditions[i][3] )

    print("testing condition:")
    print("led1: " + str(led_conditions[i][0]) )
    print("led2: " + str(led_conditions[i][1]) )
    print("led3: " + str(led_conditions[i][2]) )
    print("led4: " + str(led_conditions[i][3]) )


    #!!! IMPORTANT !!!
    # wait for dwell time, for camera autexposure to adjust...
    # otherwise, you may get cross-talk between light values
    time.sleep(hsi_cal_dwell_msec/1000)

    #print('Current exposure is %s us.' %cam.get_exposure())
    #print('Current gain is %s' %cam.get_gain())


    for j in range(num_captures):
        #stamp current time
        ts = datetime.now()
        #tempC = cam.get_temp()
        temp_centiK = int( (273.15+cam.get_temp())*100 )
        #print("cam temperature centiK: " + str(int( (273.15+cam.get_temp())*100 )))

        #take 5 pre-images to allow autoexposure to work if active
        for x in range(5):
            #get data and pass them from camera to img
            cam.get_image(ximg)

        #get data and pass them from camera to img
        cam.get_image(ximg)

        real_exp = cam.get_exposure() # get real exposure vs commanded exp

        #create numpy array with data from camera. Dimensions of array are determined
        #by imgdataformat
        #NOTE: PIL takes RGB bytes in opposite order, so invert_rgb_order is True
        data = ximg.get_image_data_numpy(invert_rgb_order=True)

        print('Saving image...')
        # fromarray(object, mode) => modes
        # see https://pillow.readthedocs.io/en/stable/handbook/concepts.html#concept-modes
        if (depth==16):
            img = PIL.Image.fromarray(data, 'I;16')  # MONO8: L, MONO16: I;16, I;16N
        else:
            img = PIL.Image.fromarray(data, 'L')  # MONO8: L, MONO16: I;16, I;16N

        fname = hsi_cal_save_path + 'hsi_cal_'+ ts.strftime("%Y-%m-%d_%H-%M-%S.%f")+'_' + \
            str(real_exp) + 'us_' + \
            str(j+1) + '-of-' + str(num_captures) + '_' +\
            str(led1_state) + str(led2_state) + str(led3_state) + str(led4_state) + '_' + \
            str(temp_centiK) + \
            '.tif'

        img.save(str(fname))

        #wait n x  exposure time to make sure any previous capture is complete before continuing
        time.sleep(2*real_exp/1000000)


# done with cal sequence
#stop data acquisition
print('Stopping acquisition...')
cam.stop_acquisition()

#stop communication
cam.close_device()

#set all leds to off
set_led(1,0)
set_led(2,0)
set_led(3,0)
set_led(4,0) 

#img.show()

elapsed = time.time() - t

print('Done. Elapsed time: ' + str(elapsed) )
