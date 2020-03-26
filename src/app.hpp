#ifndef APP_H
#define APP_H

// Live2D


// OpenCV
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

// dlib
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>

// CMUSphinx
//#include <pocketsphinx/pocketsphinx.h>

// Inner classes
#include "faceshapes.hpp"
#include "opengl.hpp"
#include "timer.hpp"
#include "videoinput.hpp"
#include "window.hpp"

namespace app
{

class App
{
public:
	
	// Functions
	static App* getInstance();
	static void releaseInstance();
	bool initialize();
	bool run();
	
	// Variables
	
	
private:
	App();
	~App();
	
	// Functions
	static void drawFace(cv::Mat frame, dlib::full_object_detection shape, cv::Scalar color, int offset);
	
	// Variables
	static App* instance;
	
	static dlib::frontal_face_detector faceDetector;
	static dlib::shape_predictor faceModel;
	
	
	
	
};

}

#endif // APP_H
