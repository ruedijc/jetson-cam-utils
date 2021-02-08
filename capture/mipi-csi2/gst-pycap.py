#!/usr/bin/env python3
#
# standalone single-image grab from context camera
# using gtreamer pipeline in python
#

import gi 
gi.require_version('Gst', '1.0')
from gi.repository import Gst, GObject
import sys
import time
from datetime import datetime

t=time.time() # get start time for performance checking

Gst.init(sys.argv)

# deal with gstreamer messages
def on_message(bus: Gst.Bus, message: Gst.Message, loop: GObject.MainLoop):
    mtype = message.type
    """
        Gstreamer Message Types and how to parse
        https://lazka.github.io/pgi-docs/Gst-1.0/flags.html#Gst.MessageType
    """
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



#cmd line pipeline
#gst-launch-1.0 videotestsrc num-buffers=50 ! autovideosink

ts = datetime.now()
filename = 'ctx_'+ ts.strftime("%Y-%m-%d_%H-%M-%S.%f")+ '' + '.jpg' 

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
bus.connect("message", on_message, loop)

try:
    loop.run()
except Exception:
    traceback.print_exc()
    loop.quit()

# Stop Pipeline
pipeline.set_state(Gst.State.NULL)

elapsed = time.time() - t
print(f"Elapsed time : {elapsed} seconds" )
