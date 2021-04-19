#!/usr/bin/env python3
from ximea import xiapi # camera calls
import PIL.Image # for image saving
from datetime import datetime # for timestamping
import time
import configparser   # to read config file
import os #for shutter script calls

import gi 
gi.require_version('Gst', '1.0')
from gi.repository import Gst, GObject
import sys #used by Gst

t=time.time() # get start time for end-to-end performance checks

#load configuration parameters from file -
config = configparser.ConfigParser(allow_no_value=True)
config.sections()
config.read('/etc/hsi-config/hsi_task2.ini')

hsi_exposure_min = int(config['hsi']['HSI_EXPOSURE_MIN'])
hsi_exposure_max = int(config['hsi']['HSI_EXPOSURE_MAX'])
hsi_exposure_auto = int(config['hsi']['HSI_EXPOSURE_AUTO'])
hsi_exposure_steps = int(config['hsi']['HSI_EXPOSURE_STEPS'])
hsi_samples = int(config['hsi']['HSI_SAMPLES'])
ctx_sync_capture = int(config['ctx']['ctx_sync_capture'])
ctx_img_quality = int(config['ctx']['ctx_img_quality'])
hsi_save_path = str(config['filesys']['hsi_save_path'])
hsi_save_path = hsi_save_path.strip('\"') #strip out double quotes
hsi_task_max_disk_gb = float(config['filesys']['hsi_task_max_disk_gb'])

print('hsi-config/hsi_task2.ini  configuration:')
print(f'Xi Min Exposure: {hsi_exposure_min}')
print(f'Xi Max Exposure: {hsi_exposure_max}')
print(f'Xi Exposure Auto: {hsi_exposure_auto}')
print(f'Xi Exposure Steps: {hsi_exposure_steps}')
print(f'Xi HSI Samples: {hsi_samples}')
print(f'Context sync capture: {ctx_sync_capture}')
print(f'Save Path: {hsi_save_path}')
print(f'Max size on disk in GB: {hsi_task_max_disk_gb}')



#before conntinuing, see if the shutter can be opened -
os.system("/home/labuser/development/jetson-cam-utils/util/shutter_safe_open.sh")

#wait 2 seconds to give shutter time to open
time.sleep(2.0)
# check shutter state to see if it is open (0) or closed (1)
f = open("/var/hsi/shutter_state", "r")
shutter_state = int(f.read())
f.close()
if (shutter_state == 0):
    #shutter state = 0
    print("Shutter open. Continuing execution.")
else:
    #shutter state = 1
    print("Shutter closed. Stopping execution.")
    sys.exit("Could not open shutter. Capture execution stopped.")


#setup context camera whether it gets captured or not - 
Gst.init(sys.argv)


# deal with gstreamer messages
def on_gst_message(bus: Gst.Bus, message: Gst.Message, loop: GObject.MainLoop):
    mtype = message.type
    if mtype == Gst.MessageType.EOS:
        print("End of stream")
        loop.quit()
    elif mtype == Gst.MessageType.ERROR:
        err, debug = message.parse_error()
        print(err, debug)
        loop.quit()
    elif mtype == Gst.MessageType.WARNING:
        err, debug = message.parse_warning()
        print(err, debug)
    return True


def capture_context_frame():
    
    #cmd line pipeline
    #gst-launch-1.0 videotestsrc num-buffers=50 ! autovideosink

    ts = datetime.now()
    filename = hsi_save_path + 'ctx_'+ ts.strftime("%Y-%m-%d_%H-%M-%S.%f")+ '' + '.jpg'

    #command = "nvarguscamerasrc num-buffers=1 ! nvjpegenc  ! filesink location=nvarguscamerasrc-frame.jpg"
    command = "nvarguscamerasrc num-buffers=1 ! nvjpegenc  ! filesink location=" + filename

    # Gst.Pipeline https://lazka.github.io/pgi-docs/Gst-1.0/classes/Pipeline.html
    # https://lazka.github.io/pgi-docs/Gst-1.0/functions.html#Gst.parse_launch
    pipeline = Gst.parse_launch(command)

    # https://lazka.github.io/pgi-docs/Gst-1.0/classes/Bus.html
    bus = pipeline.get_bus()
    # allow bus to emit messages to main thread
    bus.add_signal_watch()

    # Start pipeline
    pipeline.set_state(Gst.State.PLAYING)

    # Init GObject loop to handle Gstreamer Bus Events
    loop = GObject.MainLoop()

    # Add handler to specific signal
    # https://lazka.github.io/pgi-docs/GObject-2.0/classes/Object.html#GObject.Object.connect
    bus.connect("message", on_gst_message, loop)

    try:
        loop.run()
    except Exception:
        traceback.print_exc()
        loop.quit()

    # Stop Pipeline
    pipeline.set_state(Gst.State.NULL)
    print("gst pipeline stopped.")
    return True

#kickoff context camera is required
if(ctx_sync_capture == 1):
    capture_context_frame()


depth = 16 # set bit depth, 8 or 16, assumes mono
exposure_start = hsi_exposure_min # 10k = 10ms
exposure_end = hsi_exposure_max # 100k= 100ms
num_captures = hsi_samples # number of captures at eaxh exposures
num_exposures = hsi_exposure_steps # number of exposures 

exposure_list = []
for i in range(num_exposures):
    print(i)
    exposure_list.append( exposure_start + (exposure_end-exposure_start)*(i/(num_exposures-1)) )

print("Exposures to sample:", exposure_list[:])


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
print('Starting data acquisition...')
cam.start_acquisition()


if (hsi_exposure_auto == 1) :
    #take pictures in autoexposure mode
    cam.enable_aeag()

    #wait 5 seconds for autoexposure to adjsut
    time.sleep(5.0 ) 

    print('Current exposure is %s us.' %cam.get_exposure())
    print('Current gain is %s' %cam.get_gain())

    
    for j in range(num_captures):
        #stamp current time
        ts = datetime.now()
        #tempC = 4021
        temp_centiK = int( (273.15+cam.get_temp())*100 )


        #get data and pass them from camera to img
        cam.get_image(ximg)

        real_exp = cam.get_exposure() # get real exposure vs commanded exp

        #create numpy array with data from camera. Dimensions of array are determined
        #by imgdataformat
        #NOTE: PIL takes RGB bytes in opposite order, so invert_rgb_order is True
        data = ximg.get_image_data_numpy(invert_rgb_order=True)

        #stop data acquisition
        #print('Stopping acquisition...')
        #cam.stop_acquisition()

        print('End exposure is %s us.' %cam.get_exposure())
        print('End gain is %s' %cam.get_gain())

        #show acquired image
        print('Saving image...')
        # fromarray(object, mode) => modes
        # see https://pillow.readthedocs.io/en/stable/handbook/concepts.html#concept-modes
        if (depth==16):
            img = PIL.Image.fromarray(data, 'I;16')  # MONO8: L, MONO16: I;16, I;16N
        else:
            img = PIL.Image.fromarray(data, 'L')  # MONO8: L, MONO16: I;16, I;16N


        print('Timestamp was : ',ts.isoformat())
        # tried saving to tmpfs (/dev/shm) but didn't save time.
        # probably just slow writing?

        fname = hsi_save_path + 'hsi_'+ ts.strftime("%Y-%m-%d_%H-%M-%S.%f")+'_' + \
            str(real_exp) + 'us_' + \
            str(j+1) + '-of-' + str(num_captures) + '_' +\
            str(temp_centiK) + \
            '.tif'
        img.save(str(fname))

        #done saving single sample

    #done with number of capures

else:
    for exp in exposure_list:
        print("exposure: ", exp)
        
        cam.disable_aeag()
        cam.set_exposure(exp) # in us

        #wait 2x the exposure time to allow for camera to adjust
        time.sleep(2.0 * (exp/1000000)) 

        print('Current exposure is %s us.' %cam.get_exposure())
        print('Current gain is %s' %cam.get_gain())

        for j in range(num_captures):
            #stamp current time
            ts = datetime.now()
            #tempC = 4021
            temp_centiK = int( (273.15+cam.get_temp())*100 )

            #get data and pass them from camera to img
            cam.get_image(ximg)

            real_exp = cam.get_exposure() # get real exposure vs commanded exp

            #create numpy array with data from camera. Dimensions of array are determined
            #by imgdataformat
            #NOTE: PIL takes RGB bytes in opposite order, so invert_rgb_order is True
            data = ximg.get_image_data_numpy(invert_rgb_order=True)

            #stop data acquisition
            #print('Stopping acquisition...')
            #cam.stop_acquisition()

            #print('End exposure is %s us.' %cam.get_exposure())
            #print('End gain is %s' %cam.get_gain())

            #show acquired image
            print('Saving image...')
            # fromarray(object, mode) => modes
            # see https://pillow.readthedocs.io/en/stable/handbook/concepts.html#concept-modes
            if (depth==16):
                img = PIL.Image.fromarray(data, 'I;16')  # MONO8: L, MONO16: I;16, I;16N
            else:
                img = PIL.Image.fromarray(data, 'L')  # MONO8: L, MONO16: I;16, I;16N


            print('Timestamp was : ',ts.isoformat())
            # tried saving to tmpfs (/dev/shm) but didn't save time.
            # probably just slow writing?

            fname = hsi_save_path + 'hsi_'+ ts.strftime("%Y-%m-%d_%H-%M-%S.%f")+'_' + \
                str(real_exp) + 'us_' + \
                str(j+1) + '-of-' + str(num_captures) + '_' +\
                str(temp_centiK) + \
                '.tif'
            img.save(str(fname))
            #done with samples
    #done with exposure list

#stop data acquisition
print('Stopping acquisition...')
cam.stop_acquisition()

#stop communication
cam.close_device()

#before finishing, close the shutter
os.system("/home/labuser/development/jetson-cam-utils/util/shutter_close.sh")
    
#img.show()

elapsed = time.time() - t

print('Done. Elapsed time: ' + str(elapsed) )
