#ifndef VIDEOINPUT_HPP
#define VIDEOINPUT_HPP

#include <thread>

//OpenCV
#include <opencv2/videoio.hpp>

namespace app
{

class VideoInput
{
public:
	// Variables
	
	
	// Functions
	static VideoInput* getInstance();
	static void releaseInstance();
	bool openCamera();
	bool isOpened();
	void grabFrame();
	cv::Mat getFrame();
	bool isFrameGrabed();
	static void releaseCamera();
	
	
private:
	VideoInput();
	~VideoInput();
	
	
	// Variables
	static VideoInput* instance;
	static cv::VideoCapture* webcam;
	cv::Mat frame;
	bool hasFrame;
	
	// Functions
	
	
};

}

#endif // VIDEOINPUT_HPP
