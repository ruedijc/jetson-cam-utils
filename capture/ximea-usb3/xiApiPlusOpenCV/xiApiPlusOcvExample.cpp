// xiApiPlusOcvExample.cpp : program opens first camera, captures and displays 40 images

#include <iostream>
#include "xiApiPlusOcv.hpp"

using namespace cv;
using namespace std;
int main(int argc, char* argv[])
{
	try
	{
		// Sample for XIMEA OpenCV
		xiAPIplusCameraOcv cam;

		// Retrieving a handle to the camera device
		cout << "Opening first camera..." << endl;
		cam.OpenFirst();
		
		// Set exposure
		cam.SetExposureTime(10000); //10000 us = 10 ms
		// Note: The default parameters of each camera might be different in different API versions
		
		cout << "Starting acquisition..." << endl;
		cam.StartAcquisition();

		cout << "First pixel value:" << endl;
		XI_IMG_FORMAT format = cam.GetImageDataFormat();
		#define EXPECTED_IMAGES 150
		for (int images=0;images < EXPECTED_IMAGES;images++)
		{
			Mat cv_mat_image = cam.GetNextImageOcvMat();
	
			// display image and print 1st px value
			switch (format)
			{
				case XI_MONO16:
				case XI_RAW16:
				case XI_RAW16X2:
				case XI_RAW16X4:
				case XI_RGB16_PLANAR:
				{
					cout << cv_mat_image.at<ushort>(0, 0) << endl;
					// normalize to 16-bit for proper view
					Mat norm_cv_mat_image = cv_mat_image.clone();
					normalize(cv_mat_image, norm_cv_mat_image, 0, 65536, NORM_MINMAX, -1, Mat()); // 0 - 65536, 16 bit unsigned integer range
					imshow("Image from camera", norm_cv_mat_image);
				}
				break;
				case XI_RGB24:
				{
					cout << cv_mat_image.at<Vec3b>(0, 0) << endl;
					imshow("Image from camera", cv_mat_image);
				}
				break;
				case XI_RGB32:
				{
					cout << cv_mat_image.at<Vec4b>(0, 0) << endl;
					imshow("Image from camera", cv_mat_image);
				}
				break;
				case XI_RGB48:
				{
					cout << cv_mat_image.at<Vec3w>(0, 0) << endl;
					imshow("Image from camera", cv_mat_image);
				}
				break;
				case XI_RGB64:
				{
					cout << cv_mat_image.at<Vec4w>(0, 0) << endl;
					imshow("Image from camera", cv_mat_image);
				}
				break;
				case XI_RAW32FLOAT:
				{
					cout << cv_mat_image.at<float>(0, 0) << endl;
					imshow("Image from camera", cv_mat_image);
				}
				break;
				default:
				{
					cout << +cv_mat_image.at<uchar>(0, 0) << endl;
					imshow("Image from camera", cv_mat_image);
				}
			}
			waitKey(2);
		}
		
		cam.StopAcquisition();
		cam.Close();
		cout << "Done" << endl;
		waitKey(1000);
	}
	catch(xiAPIplus_Exception& exp)
	{
		cout << "Error:" << endl;
		exp.PrintError();
#ifdef WIN32
		Sleep(3000);
#endif
		waitKey(3000);
		return -1;
	}
	return 0;
}