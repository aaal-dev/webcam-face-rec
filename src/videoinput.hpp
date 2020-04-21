#ifndef VIDEOINPUT_HPP
#define VIDEOINPUT_HPP

//OpenCV
#include <opencv2/videoio.hpp>

namespace app
{

class VideoInput
{
public:
	// Functions
	static VideoInput* getInstance();
	static void releaseInstance();
	bool openCamera();
	cv::Mat grabFrame();
	static void releaseCamera();
	
	
	// Variables
	
	
private:
	VideoInput();
	~VideoInput();
	
	// Functions
	
	
	// Variables
	static VideoInput* instance;
	static cv::VideoCapture* webcam;
};

}

#endif // VIDEOINPUT_HPP
