#ifndef KINECTSENSOR_H
#define KINECTSENSOR_H

#include <kinectv2\Kinect.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

class KinectSensor {
public:
	KinectSensor();
	~KinectSensor();

	IKinectSensor* kinect;
	ICoordinateMapper* coordinateMapper;

	IColorFrameReader* colorFrameReader;
	IColorFrameSource* colorFrameSource;
	IColorFrame* colorFrame;

	IDepthFrameReader* depthFrameReader;
	IDepthFrameSource* depthFrameSource;
	IDepthFrame* depthFrame;

	IInfraredFrameReader* infraredFrameReader;
	IInfraredFrameSource* infraredFrameSource;
	IInfraredFrame* infraredFrame;

	ILongExposureInfraredFrameReader* longExposureInfraredFrameReader;
	ILongExposureInfraredFrameSource* longExposureInfraredFrameSource;
	ILongExposureInfraredFrame* longExposureInfraredFrame;

	std::vector<unsigned short> infraredBuffer;
	int infraredWidth;
	int infraredHeight;
	unsigned int infraredBytesPerPixel;
	cv::Mat infraredMat;

	std::vector<unsigned short> depthBuffer;
	int depthWidth;
	int depthHeight;
	unsigned int depthBytesPerPixel;
	cv::Mat depthMat;
	cv::Mat pointCloudMat;

	int initialize();
	int close();

	int initializeInfrared();
	int initializeDepth();
	int initializeLongExposureInfrared();
	int initializeColor();
	int getDepthCameraIntrinsics();

	int open(int sourceIndex);
	static const int COLORINDEX = 0;
	static const int INFRAREDINDEX = 1;
	static const int LONG_EXPOSURE_INFRAREDINDEX = 2;
	static const int DEPTHINDEX = 3;

	int read(int sourceIndex);

	cv::Mat colorMat;


};

#endif