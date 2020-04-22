#include "videoinput.hpp"

namespace app
{

VideoInput* VideoInput::instance = nullptr;
cv::VideoCapture* VideoInput::webcam = nullptr;

VideoInput::VideoInput()
{
	hasFrame = false;
}

VideoInput::~VideoInput(){}

VideoInput* VideoInput::getInstance() 
{
	if (instance == nullptr)
		instance = new VideoInput();
	return instance;
}

void VideoInput::releaseInstance()
{
	if (instance != nullptr)
		delete instance;
	instance = nullptr;
}

bool VideoInput::openCamera()
{
	// Initialize OpenCV webcam capture
	webcam = new cv::VideoCapture(0);
	if (webcam->isOpened())
		return true;
	fprintf( stderr, "Unable to connect to camera.\n" );
	//getchar();
	return false;
}

bool VideoInput::isOpened()
{
	return webcam->isOpened();
}

void VideoInput::grabFrame()
{
	*webcam >> frame;
	hasFrame = true;
}

cv::Mat VideoInput::getFrame()
{
	hasFrame = false;
	return frame;
}

bool VideoInput::isFrameGrabed()
{
	return hasFrame;
}

void VideoInput::releaseCamera()
{
	// Release OpenCV webcam capture
	webcam->release();
}

}

