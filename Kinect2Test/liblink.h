#pragma once

#include <opencv2/opencv.hpp>

#define LIB_PATH "D:/dev/lib64/"
#define CV_LIB_PATH "D:/dev/lib64/"
#define KINECT_LIB_PATH "C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/Lib/x64/"
#define CV_VER_NUM  CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)

#ifdef _DEBUG
#define LIB_EXT "d.lib"
#else
#define LIB_EXT ".lib"
#endif

#pragma comment(lib, CV_LIB_PATH "opencv_ts"       CV_VER_NUM LIB_EXT)
#pragma comment(lib, CV_LIB_PATH "opencv_core" CV_VER_NUM LIB_EXT)
#pragma comment(lib, CV_LIB_PATH "opencv_highgui" CV_VER_NUM LIB_EXT)
#pragma comment(lib, KINECT_LIB_PATH "Kinect20" LIB_EXT)

