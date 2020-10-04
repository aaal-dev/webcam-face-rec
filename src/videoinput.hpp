#ifndef VIDEOINPUT_HPP
#define VIDEOINPUT_HPP

#include <thread>

//OpenCV
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

// Inner classes
#include "log.hpp"
#include "statecon.hpp"

namespace app
{

class VideoInput
{
public:
	VideoInput();
	~VideoInput();
	
	
	// Variables
	
	
	// Functions
	bool openCamera();
	bool isOpened();
	void grabFrame();
	cv::Mat getFrameData();
	bool isFrameGrabed();
	void releaseCamera();
	
	
private:
	// Variables
	static Log* logger;
	static StateCon* stater;
	cv::VideoCapture* webcam;
	cv::Mat frame;
	bool hasFrame;
	
	
	// Functions
	
	
	
};

}

#endif // VIDEOINPUT_HPP
