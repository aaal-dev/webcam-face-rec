#include "videoinput.hpp"

namespace app {

Log* VideoInput::logger = nullptr;
StateCon* VideoInput::stater = nullptr;

VideoInput::VideoInput() {
	logger = Log::getLog();
	stater = StateCon::getStateCon();
	hasFrame = false;
}

VideoInput::~VideoInput(){}

bool VideoInput::openCamera() {
	// Initialize OpenCV webcam capture
#ifdef __linux__ 
	webcam = new cv::VideoCapture(0, cv::CAP_V4L2);
#elif _WIN32
	webcam = new cv::VideoCapture(0, cv::CAP_MSMF);
#else
	webcam = new cv::VideoCapture(0, cv::CAP_AVFOUNDATION);
#endif
	if (!webcam->isOpened()) {
		logger->logGLError("Unable to connect to camera.\n" );
		return false;
	}
	std::thread grabFrameThread(&VideoInput::grabFrame, this);
	grabFrameThread.detach();
	return true;
}

bool VideoInput::isOpened() {
	return webcam->isOpened();
}

void VideoInput::grabFrame() {
	while (true) {
		if (!hasFrame) {
			*webcam >> frame;
			hasFrame = true;
		}
	}
}

cv::Mat VideoInput::getFrameData() {
	cv::Mat frameBGR, frameRGB;
	flip(frame, frameBGR, 1);
	cvtColor(frameBGR, frameRGB, cv::COLOR_BGR2RGB);
	hasFrame = false;
	return frameRGB;
}

bool VideoInput::isFrameGrabed() {
	return hasFrame;
}

void VideoInput::releaseCamera() {
	// Release OpenCV webcam capture
	webcam->release();
}

}

