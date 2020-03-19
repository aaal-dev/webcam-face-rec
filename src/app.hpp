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
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

// dlib
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>

// CMUSphinx
#include <pocketsphinx/pocketsphinx.h>


#include "shader.hpp"

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
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	template < typename T >
	static void dlibDetectAndDraw(cv::Mat frame, T cimg, dlib::frontal_face_detector detector, dlib::shape_predictor pose_model, cv::Scalar color);
	
	// Variables
	static App* instance;
	
	static GLFWwindow* window;
	
	static cv::VideoCapture* webcam;
	
	static dlib::frontal_face_detector faceDetector;
	static dlib::shape_predictor faceModel;
	
	static GLuint* VAO;
	static GLuint* VBO;
	static GLuint* EBO;
	static GLuint* webcamTexture;
	static GLuint* modelTexture;
	
	static cv::KalmanFilter* kalmanFilter[68];
	
	static GLuint programID;
	
	static bool drawActualPoints;
	static bool drawCorrectedPoints;
	static GLuint DEFAULT_WIDTH;
	static GLuint DEFAULT_HEIGHT;
	
};

class Timer
{
public:
	// Functions
	static Timer* getInstance();
	static void releaseInstance();
	void initialize();
	//static void setStartTime(double startTime);
	static float getSpeedOnMS();
	
	// Variables
	
	
private:
	Timer();
	~Timer();
	
	// Functions
	
	
	// Variables
	static Timer* instance;
	static double startTime;
	static double numberOfTicks;
};

}

#endif // APP_H
