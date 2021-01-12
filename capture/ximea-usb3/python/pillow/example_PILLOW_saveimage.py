from ximea import xiapi
import PIL.Image

depth=16 # set bit depth, 8 or 16, assumes mono

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

#cam.set_exposure(10000)
cam.set_exposure(20000) # in us
print('Current exposure is %s us.' %cam.get_exposure())
print('Current gain is %s' %cam.get_gain())

#create instance of Image to store image data and metadata
img = xiapi.Image()

#start data acquisition
print('Starting data acquisition...')
cam.start_acquisition()

#get data and pass them from camera to img
cam.get_image(img)

#create numpy array with data from camera. Dimensions of array are determined
#by imgdataformat
#NOTE: PIL takes RGB bytes in opposite order, so invert_rgb_order is True
data = img.get_image_data_numpy(invert_rgb_order=True)

#stop data acquisition
print('Stopping acquisition...')
cam.stop_acquisition()

print('End exposure is %s us.' %cam.get_exposure())
print('End gain is %s' %cam.get_gain())

#stop communication
cam.close_device()

#show acquired image
print('Saving image...')
# fromarray(object, mode) => modes
# see https://pillow.readthedocs.io/en/stable/handbook/concepts.html#concept-modes
if (depth==16):
    img = PIL.Image.fromarray(data, 'I;16')  # MONO8: L, MONO16: I;16, I;16N
else:
    img = PIL.Image.fromarray(data, 'L')  # MONO8: L, MONO16: I;16, I;16N


if (depth==16):
    img.save('xi_example.tif')
    img.save('xi_example.png')
    #img.save('xi_example.bmp') no 16-bit bmp
    #img.save('xi_example.jpg') no 16-bit jpeg
else:
    img.save('xi_example.tif')
    img.save('xi_example.png')
    img.save('xi_example.bmp')
    img.save('xi_example.jpg')

#img.show()

print('Done.')
