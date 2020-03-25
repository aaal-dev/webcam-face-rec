#ifndef APP_H
#define APP_H

// GLAD
#include "glad/glad.h"

// GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>

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
#include "shader.hpp"
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
	
	static GLuint* VAO;
	static GLuint* VBO;
	static GLuint* EBO;
	static GLuint* webcamTexture;
	static GLuint* modelTexture;
	
	static GLuint programID;
	
	static GLuint DEFAULT_WIDTH;
	static GLuint DEFAULT_HEIGHT;
	
};

}

#endif // APP_H
