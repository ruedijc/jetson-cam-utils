// xiSample.cpp : program that captures 10 images

#include "stdafx.h"

#include <memory.h>
#ifdef WIN32
  #include "xiapi_dng_store.h"       // Windows
#else
  #include <m3api/xiapi_dng_store.h> // Linux, OSX
#endif

#define HandleResult(res,place) if (res!=XI_OK) {printf("Error after %s (%d)\n",place,res);goto finish;}

int _tmain(int argc, _TCHAR* argv[])
{
	// image buffer
	XI_IMG image;
	memset(&image,0,sizeof(image));
	image.size = sizeof(XI_IMG);

	// Sample for XIMEA API V4.05
	HANDLE xiH = NULL;
	XI_RETURN stat = XI_OK;

	// Retrieving a handle to the camera device
	printf("Opening first camera...\n");
	stat = xiOpenDevice(0, &xiH);
	HandleResult(stat,"xiOpenDevice");

	stat = xiSetParamInt(xiH, XI_PRM_IMAGE_DATA_FORMAT, XI_MONO8);
	HandleResult(stat,"xiSetParam (image format)");
	stat = xiSetParamInt(xiH, XI_PRM_EXPOSURE, 10000);
	HandleResult(stat,"xiSetParam (exposure set)");

	printf("Starting acquisition...\n");
	stat = xiStartAcquisition(xiH);
	HandleResult(stat,"xiStartAcquisition");

	//for filename str
	char fn[80];

	for (int i=0;i<12;i++) {

		
		stat = xiGetImage(xiH, 5000, &image);
		HandleResult(stat,"xiGetImage");

		printf("Writing DNG...\n");
		XI_DNG_METADATA metadata;
		xidngFillMetadataFromCameraParams(xiH, &metadata);
		stat = xidngStore("test.dng", &image, &metadata);

		sprintf(fn, "test_%d.dng", i);
		printf(fn);
		//stat = xidngStore(fn, &image, &metadata);
		
		HandleResult(stat,"xidngStore");

	}
	printf("Stopping acquisition...\n");
	xiStopAcquisition(xiH);
	xiCloseDevice(xiH);
finish:
	printf("Done\n");
#ifdef WIN32
	Sleep(2000);
#endif
	return 0;
}

