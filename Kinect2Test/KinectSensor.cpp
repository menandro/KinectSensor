#include "KinectSensor.h"

KinectSensor::KinectSensor() {
	this->initialize();
}

KinectSensor::~KinectSensor() {
	if (kinect != nullptr) {
		kinect->Close();
	}
}

int KinectSensor::initialize() {
	HRESULT hResult = S_OK;
	hResult = GetDefaultKinectSensor(&kinect);
	if FAILED(hResult) {
		std::cout << "Error: GetDefaultKinectSensor" << std::endl;
		return -1;
	}
	// Open
	hResult = kinect->Open();
	if FAILED(hResult) {
		std::cout << "Error: IKinectSensor::Open()" << std::endl;
		return -1;
	}

	return 0;
}

int KinectSensor::close() {
	if (kinect != nullptr) {
		kinect->Close();
	}
	return 0;
}

int KinectSensor::getDepthCameraIntrinsics() {
	//Needs to wait a couple of seconds to be reliable
	// Retrieve Coordinate Mapper
	HRESULT hResult = kinect->get_CoordinateMapper(&coordinateMapper);
	if FAILED(hResult) {
		std::cout << "Error: IKinectSensor::get_CoordinateMapper()" << std::endl;
		return -1;
	}

	// Get camera intrinsics
	CameraIntrinsics *cameraIntrinsics;
	if FAILED(coordinateMapper->GetDepthCameraIntrinsics(cameraIntrinsics)) {
		std::cout << "Error: GetDepthCameraIntrinsics()" << std::endl;
	}
	std::cout << "Depth camera intrinsics:" << std::endl;
	std::cout << "fx: " << cameraIntrinsics->FocalLengthX << std::endl;
	std::cout << "fy: " << cameraIntrinsics->FocalLengthY << std::endl;
	std::cout << "principal point x: " << cameraIntrinsics->PrincipalPointX << std::endl;
	std::cout << "principal point y: " << cameraIntrinsics->PrincipalPointY << std::endl;
	return 0;
}

int KinectSensor::initializeDepth() {
	HRESULT hResult = S_OK;
	hResult = kinect->get_DepthFrameSource(&depthFrameSource);
	if FAILED(hResult) {
		std::cout << "Error: get_InfraredFrameSource()" << std::endl;
		return 1;
	}
	hResult = depthFrameSource->OpenReader(&depthFrameReader);
	if FAILED(hResult) {
		std::cout << "Error: get_InfraredFrameReader()" << std::endl;
		return 1;
	}
	// Retrieve Depth Description
	IFrameDescription* depthFrameDescription;
	depthFrameSource->get_FrameDescription(&depthFrameDescription);
	depthFrameDescription->get_Width(&depthWidth); // 512
	depthFrameDescription->get_Height(&depthHeight); // 424
	depthFrameDescription->get_BytesPerPixel(&depthBytesPerPixel); // 2

	depthBuffer = std::vector<unsigned short>();
	depthBuffer.resize(depthWidth * depthHeight);
	depthMat = cv::Mat::zeros(depthHeight, depthWidth, CV_16U);

	return 0;
}


int KinectSensor::initializeInfrared() {
	HRESULT hResult = S_OK;
	hResult = kinect->get_InfraredFrameSource(&infraredFrameSource);
	if FAILED(hResult) {
		std::cout << "Error: get_InfraredFrameSource()" << std::endl;
		return 1;
	}
	hResult = infraredFrameSource->OpenReader(&infraredFrameReader);
	if FAILED(hResult) {
		std::cout << "Error: get_InfraredFrameReader()" << std::endl;
		return 1;
	}
	// Retrieve Depth Description
	IFrameDescription* infraredFrameDescription;
	infraredFrameSource->get_FrameDescription(&infraredFrameDescription);
	infraredFrameDescription->get_Width(&infraredWidth); // 512
	infraredFrameDescription->get_Height(&infraredHeight); // 424
	infraredFrameDescription->get_BytesPerPixel(&infraredBytesPerPixel); // 2

	infraredBuffer = std::vector<unsigned short>();
	infraredBuffer.resize(infraredWidth * infraredHeight);
	infraredMat = cv::Mat::zeros(infraredHeight, infraredWidth, CV_16U);
	return 0;
}

int KinectSensor::initializeLongExposureInfrared() {
	HRESULT hResult = S_OK;
	hResult = kinect->get_LongExposureInfraredFrameSource(&longExposureInfraredFrameSource);
	if FAILED(hResult) {
		std::cout << "Error: get_InfraredFrameSource()" << std::endl;
		return 1;
	}
	hResult = longExposureInfraredFrameSource->OpenReader(&longExposureInfraredFrameReader);
	if FAILED(hResult) {
		std::cout << "Error: get_InfraredFrameReader()" << std::endl;
		return 1;
	}
	// Retrieve Depth Description
	IFrameDescription* longExposureInfraredFrameDescription;
	longExposureInfraredFrameSource->get_FrameDescription(&longExposureInfraredFrameDescription);
	longExposureInfraredFrameDescription->get_Width(&infraredWidth); // 512
	longExposureInfraredFrameDescription->get_Height(&infraredHeight); // 424
	longExposureInfraredFrameDescription->get_BytesPerPixel(&infraredBytesPerPixel); // 2

	infraredBuffer = std::vector<unsigned short>();
	infraredBuffer.resize(infraredWidth * infraredHeight);
	infraredMat = cv::Mat::zeros(infraredHeight, infraredWidth, CV_16U);
	return 0;
}

int KinectSensor::initializeColor() {
	HRESULT hResult = S_OK;
	hResult = kinect->get_ColorFrameSource(&colorFrameSource);
	if FAILED(hResult) {
		std::cout << "Error: get_ColorFrameSource()" << std::endl;
		return -1;
	}

	hResult = colorFrameSource->OpenReader(&colorFrameReader);
	if FAILED(hResult) {
		std::cout << "Error: get_ColorFrameReader()" << std::endl;
		return -1;
	}
	return 0;
}

int KinectSensor::open(int sourceIndex) {
	if (sourceIndex == this->COLORINDEX) {
		this->initializeColor();
	}
	else if (sourceIndex == this->INFRAREDINDEX) {
		this->initializeInfrared();
	}
	else if (sourceIndex == this->LONG_EXPOSURE_INFRAREDINDEX) {
		this->initializeLongExposureInfrared();
	}
	else if (sourceIndex == this->DEPTHINDEX) {
		this->initializeDepth();
	}
	return 0;
}

int KinectSensor::read(int sourceIndex) {
	HRESULT hResult;
	if (sourceIndex == this->COLORINDEX) {
		hResult = this->colorFrameReader->AcquireLatestFrame(&colorFrame);
	}

	else if (sourceIndex == this->DEPTHINDEX) {
		hResult = this->depthFrameReader->AcquireLatestFrame(&depthFrame);
		if (SUCCEEDED(hResult)) {
			hResult = depthFrame->CopyFrameDataToArray(static_cast<UINT>(depthBuffer.size()), &depthBuffer[0]);
			if (SUCCEEDED(hResult)) {
				depthMat = cv::Mat(depthHeight, depthWidth, CV_16U, depthBuffer.data());
			}
			else {
				return 1;
			}
		}
		else {
			return 1;
		}
		if (depthFrame != NULL) {
			depthFrame->Release();
			depthFrame = NULL;
		}
	}

	else if (sourceIndex == this->INFRAREDINDEX) {
		hResult = this->infraredFrameReader->AcquireLatestFrame(&infraredFrame);
		if (SUCCEEDED(hResult)) {
			hResult = infraredFrame->CopyFrameDataToArray(static_cast<UINT>(infraredBuffer.size()), &infraredBuffer[0]);
			if (SUCCEEDED(hResult)) {
				infraredMat = cv::Mat(infraredHeight, infraredWidth, CV_16U, infraredBuffer.data());
			}
			else {
				return 1;
			}
		}
		else {
			return 1;
		}
		if (infraredFrame != NULL) {
			infraredFrame->Release();
			infraredFrame = NULL;
		}
	}

	else if (sourceIndex == this->LONG_EXPOSURE_INFRAREDINDEX) {
		hResult = this->longExposureInfraredFrameReader->AcquireLatestFrame(&longExposureInfraredFrame);
		if (SUCCEEDED(hResult)) {
			hResult = longExposureInfraredFrame->CopyFrameDataToArray(static_cast<UINT>(infraredBuffer.size()), &infraredBuffer[0]);
			if (SUCCEEDED(hResult)) {
				infraredMat = cv::Mat(infraredHeight, infraredWidth, CV_16U, infraredBuffer.data());
			}
			else {
				return 1;
			}
		}
		else {
			return 1;
		}
		if (longExposureInfraredFrame != NULL) {
			longExposureInfraredFrame->Release();
			longExposureInfraredFrame = NULL;
		}
	}
	return 0;
}
