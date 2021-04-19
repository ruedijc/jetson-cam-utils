#!/usr/bin/env python3
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
config.read('/etc/hsi-config/hsi_task5.ini')


ctx_vid_duration = int(config['ctx']['ctx_vid_duration'])
ctx_vid_bitrate = int(config['ctx']['ctx_vid_bitrate'])
ctx_vid_4k = int(config['ctx']['ctx_vid_4k'])
save_path = str(config['filesys']['save_path'])
save_path = save_path.strip('\"') #strip out double quotes
task_max_disk_gb = float(config['filesys']['task_max_disk_gb'])

print('hsi-config/hsi_task5.ini  configuration:')
print(f'Video Duration: {ctx_vid_duration}')
print(f'Video bitrate: {ctx_vid_bitrate}')
print(f'Video resolution 4k?: {ctx_vid_4k}')
print(f'Save Path: {save_path}')
print(f'Max size on disk in GB: {task_max_disk_gb}')


#before conntinuing, attempt to open the shutter -
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


def capture_context_video():
    
    #cmd line pipeline
    #gst-launch-1.0 videotestsrc num-buffers=50 ! autovideosink

    ts = datetime.now()
    filename = hsi_save_path + 'ctx_'+ ts.strftime("%Y-%m-%d_%H-%M-%S.%f")+ '' + '.jpg'

    #command = "nvarguscamerasrc num-buffers=1 ! nvjpegenc  ! filesink location=nvarguscamerasrc-frame.jpg"
    command = "nvarguscamerasrc num-buffers=1 ! nvjpegenc  ! filesink location=" + filename


    if (ctx_vid_4k):
        w = 3840
        h = 2160
    else:
        w = 1920
        h = 1080

    command = "nvarguscamerasrc sensor-id=0  timeout=" + ctx_vid_duration + " !\
        'video/x-raw(memory:NVMM), width=" + w + ", height=" + h + ", format=NV12, framerate=30/1' !\
        nvvidconv flip-method=2 !\
        'video/x-raw(memory:NVMM), format=I420' !\
        omxh265enc peak-bitrate=" + ctx_vid_bitrate  + " !\
        'video/x-h265, stream-format=byte-stream' !\
        h265parse !\
        qtmux !\
        filesink location=" + filename + " -e"


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

#kickoff context camera video capture
capture_context_video()

#before finishing, close the shutter
os.system("/home/labuser/development/jetson-cam-utils/util/shutter_close.sh")
    
#img.show()

elapsed = time.time() - t

print('Done. Elapsed time: ' + str(elapsed) )
