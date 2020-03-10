//#include <iostream>

// GLAD
#include "glad/glad.h"

// GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>

// Live2D
#include <live2d/Live2D.h>
#include <live2d/Live2DModelOpenGL.h>

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

#include "common/shader.hpp"

#define PI 3.141592654

// Window dimensions
const GLuint DEFAULT_WIDTH = 800, DEFAULT_HEIGHT = 600;
cv::KalmanFilter* kalmanFilter[68];
bool drawActualPoints = false, drawCorrectedPoints = false;

// Is called whenever a key is pressed/released via GLFW
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
	{
		if (drawActualPoints == false)
		{
			drawActualPoints = true;
		} else {
			drawActualPoints = false;
		}
	}
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
	{
		if (drawCorrectedPoints == false)
		{
			drawCorrectedPoints = true;
		} else {
			drawCorrectedPoints = false;
		}
	}
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
}

template < typename T >
void dlibDetectAndDraw(cv::Mat frame, T cimg, dlib::frontal_face_detector detector, dlib::shape_predictor pose_model, cv::Scalar color)
{
	
	// Detect faces 
		std::vector<dlib::rectangle> faces = detector(cimg);
		
		// Find the pose of each face.
		std::vector<dlib::full_object_detection> shapes;
		for (unsigned long i = 0; i < faces.size(); ++i)
			shapes.push_back(pose_model(cimg, faces[i]));
		
		if (!shapes.empty())
		{
			cv::Point actualPosition[68] = {};
			cv::Point correctedPosition[68] = {};
			for (int i = 0; i < 68; i++)
			{
				cv::Mat predictedPosition = kalmanFilter[i]->predict();
				cv::Mat position(2, 1, CV_32F, cv::Scalar::all(0));
				
				actualPosition[i] = cv::Point(shapes[0].part(i).x()*2, shapes[0].part(i).y()*2);
				position.at<float>(0, 0) = actualPosition[i].x;
				position.at<float>(1, 0) = actualPosition[i].y;
				
				cv::Mat cPosition = kalmanFilter[i]->correct(position);
				correctedPosition[i] = cv::Point(cPosition.at<float>(0), cPosition.at<float>(1));
			}
			
			// Simple visualization of face detection
			//for (int i = 0; i < 68; i++)
			//{
			//	cv::circle(frame, cvPoint(shapes[0].part(i).x()*2, shapes[0].part(i).y()*2), 2, cv::Scalar(255, 255, 255), -1);
			//	cv::circle(frame, correctedPosition[i], 2, cv::Scalar(255, 0, 0), -1);
			//}
			
			// Advanced visualization of face detection
			// Shape of face (parts 0-16)
			for (int i = 0; i < 16; i++)
			{
				if (drawActualPoints == true)
					cv::line(frame, actualPosition[i], actualPosition[i+1], color, 1);
				if (drawCorrectedPoints == true)
					cv::line(frame, correctedPosition[i], correctedPosition[i+1], color, 1);
			}
			
			// Right eyebrow shape (parts 17-21)
			for (int i = 17; i < 21; i++)
			{
				if (drawActualPoints == true)
					cv::line(frame, actualPosition[i], actualPosition[i+1], color, 1);
				if (drawCorrectedPoints == true)
					cv::line(frame, correctedPosition[i], correctedPosition[i+1], color, 1);
			}
			
			// Left eyebrow shape (parts 22-26)
			for (int i = 22; i < 26; i++)
			{
				if (drawActualPoints == true)
					cv::line(frame, actualPosition[i], actualPosition[i+1], color, 1);
				if (drawCorrectedPoints == true)
					cv::line(frame, correctedPosition[i], correctedPosition[i+1], color, 1);
			}
			
			// Nose shape (parts 27-35)
			for (int i = 27; i < 35; i++)
			{
				if (drawActualPoints == true)
					cv::line(frame, actualPosition[i], actualPosition[i+1], color, 1);
				if (drawCorrectedPoints == true)
					cv::line(frame, correctedPosition[i], correctedPosition[i+1], color, 1);
			}
			
			cv::putText(
					frame, 
					std::to_string(shapes[0].part(35).x()*2 - shapes[0].part(31).x()*2), 
					cvPoint(shapes[0].part(33).x()*2, shapes[0].part(33).y()*2), 
					cv::FONT_HERSHEY_SIMPLEX, 0.4, color, 1);
			
			// Right eye shape (parts 36-41)
			long int rightEye_x_min = actualPosition[36].x;
			long int rightEye_x_max = actualPosition[41].x;
			long int rightEye_y_min = actualPosition[36].y;
			long int rightEye_y_max = actualPosition[41].y;
			for (int i = 36; i < 41; i++)
			{
				if ( i == 40) 
				{
					if (drawActualPoints == true)
						cv::line(frame, actualPosition[i], actualPosition[36], color, 1);
					if (drawCorrectedPoints == true)
						cv::line(frame, correctedPosition[i], correctedPosition[36], color, 1);
				} else {
					if (drawActualPoints == true)
						cv::line(frame, actualPosition[i], actualPosition[i+1], color, 1);
					if (drawCorrectedPoints == true)
						cv::line(frame, correctedPosition[i], correctedPosition[i+1], color, 1);
				}
				
				// Found min and max of x and y for locate right eye
				if (actualPosition[i].x < rightEye_x_min)
					rightEye_x_min = actualPosition[i].x;
				if (actualPosition[i].x > rightEye_x_max)
					rightEye_x_max = actualPosition[i].x;
				if (actualPosition[i].y < rightEye_y_min)
					rightEye_y_min = actualPosition[i].y;
				if (actualPosition[i].y > rightEye_y_max)
					rightEye_y_max = actualPosition[i].y;
			}
			
			// Left eye shape (parts 42-47)
			long int leftEye_x_min = actualPosition[42].x; 
			long int leftEye_x_max = actualPosition[47].x; 
			long int leftEye_y_min = actualPosition[42].y; 
			long int leftEye_y_max = actualPosition[47].y;
			for (int i = 42; i < 47; i++)
			{
				if ( i == 46) 
				{
					if (drawActualPoints == true)
						cv::line(frame, actualPosition[i], actualPosition[42], color, 1);
					if (drawCorrectedPoints == true)
						cv::line(frame, correctedPosition[i], correctedPosition[42], color, 1);
				} else {
					if (drawActualPoints == true)
						cv::line(frame, actualPosition[i], actualPosition[i+1], color, 1);
					if (drawCorrectedPoints == true)
						cv::line(frame, correctedPosition[i], correctedPosition[i+1], color, 1);
				}
				if (actualPosition[i].x < leftEye_x_min)
					leftEye_x_min = actualPosition[i].x;
				if (actualPosition[i].x > leftEye_x_max)
					leftEye_x_max = actualPosition[i].x;
				if (actualPosition[i].y < leftEye_y_min)
					leftEye_y_min = actualPosition[i].y;
				if (actualPosition[i].y > leftEye_y_max)
					leftEye_y_max = actualPosition[i].y;
			}
			
			// Outer mouth shape (parts 48-59)
			for (int i = 48; i < 59; i++)
			{
				if ( i == 58) 
				{
					if (drawActualPoints == true)
						cv::line(frame, actualPosition[i], actualPosition[48], color, 1);
					if (drawCorrectedPoints == true)
						cv::line(frame, correctedPosition[i], correctedPosition[48], color, 1);
				} else {
					if (drawActualPoints == true)
						cv::line(frame, actualPosition[i], actualPosition[i+1], color, 1);
					if (drawCorrectedPoints == true)
						cv::line(frame, correctedPosition[i], correctedPosition[i+1], color, 1);
				}
			}
			
			// Inner mouth shape (parts 60-67)
			for (int i = 60; i < 68; i++)
			{
				if (i == 67) 
				{
					if (drawActualPoints == true)
						cv::line(frame, actualPosition[i], actualPosition[60], color, 1);
					if (drawCorrectedPoints == true)
						cv::line(frame, correctedPosition[i], correctedPosition[60], color, 1);
				} else {
					if (drawActualPoints == true)
						cv::line(frame, actualPosition[i], actualPosition[i+1], color, 1);
					if (drawCorrectedPoints == true)
						cv::line(frame, correctedPosition[i], correctedPosition[i+1], color, 1);
				}
			}
		}
}

int main() 
{   
	// Initialise GLFW
	if(!glfwInit())
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}
	
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif 
	
	live2d::Live2DModelOpenGL* live2DModel;
	
	cv::VideoCapture* camera = new cv::VideoCapture(0);
	if (!camera->isOpened())
	{
		fprintf( stderr, "Unable to connect to camera.\n" );
		getchar();
		return 1;
	}
	
	dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
	dlib::shape_predictor pose_model;
	dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;

	fprintf( stdout, "Starting GLFW context, OpenGL 3.3...\n" );
	
	GLFWwindow* window1 = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Face", NULL, NULL);
	if (window1 == NULL) {
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window1);
	glfwSetFramebufferSizeCallback(window1, framebuffer_size_callback);

	// Set the required callback functions
	glfwSetKeyCallback(window1, key_callback);
	
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		fprintf( stderr, "Failed to initialize OpenGL context.\n" );
		getchar();
		return -1;
	}

	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
	
	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "vertexshader", "fragmentshader" );
	
	static const GLfloat vertices[] = {
	// |     Position     ||  TexCoord  |
	   -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, // top left
		1.0f,  1.0f,  0.0f,  1.0f,  0.0f, // top right
	   -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, // below left
		1.0f, -1.0f,  0.0f,  1.0f,  1.0f  // below right 
	};
	// Set up index
	static const GLuint indices[] = {
		0, 2, 1, 1, 2, 3
	};
	
	GLuint* VAO = new GLuint();
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);
	
	GLuint* VBO = new GLuint();
	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	
	GLuint* EBO = new GLuint();
	glGenBuffers(1, EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	GLuint* texture = new GLuint();
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	float fltTransitionMatrixValues[4][4] = { { 1, 0, 1, 0 },
											  { 0, 1, 0, 1 },
											  { 0, 0, 1, 0 },
											  { 0, 0, 0, 1 } };
											  
	float fltMeasurementMatrixValues[2][4] = { { 1, 0, 0, 0 },
											   { 0, 1, 0, 0 } };
	
	// Kalman 
	for (int i=0; i<68; i++)
	{
		
		kalmanFilter[i] = new cv::KalmanFilter(4, 2, 0);
		kalmanFilter[i]->transitionMatrix = cv::Mat(4, 4, CV_32F, fltTransitionMatrixValues);
		kalmanFilter[i]->measurementMatrix = cv::Mat(2, 4, CV_32F, fltMeasurementMatrixValues);
		
		//cv::setIdentity(kalmanFilter[i]->processNoiseCov, cv::Scalar::all(0.0001));
		//cv::setIdentity(kalmanFilter[i]->measurementNoiseCov, cv::Scalar::all(10));
		//cv::setIdentity(kalmanFilter[i]->errorCovPost, cv::Scalar::all(0.1));
		cv::setIdentity(kalmanFilter[i]->processNoiseCov, cv::Scalar::all(1e-4));
		cv::setIdentity(kalmanFilter[i]->measurementNoiseCov, cv::Scalar::all(1e-1));
		cv::setIdentity(kalmanFilter[i]->errorCovPost, cv::Scalar::all(.1));
	}
	
	
	// Main loop
	int nbFrames = 0;
	double lastTime = glfwGetTime();
	char* titlestr = new char[255];
	
	while (!glfwWindowShouldClose(window1)) {
		
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if( currentTime - lastTime >= 1.0 ) {
			sprintf( titlestr, "videoInput Demo App (%.1f ms)", 1000.0/double(nbFrames) );
			glfwSetWindowTitle(window1, titlestr);
			nbFrames = 0;
			lastTime += 1.0;
		}
		
		cv::Mat tframe, frame,
			frameRGB, frameRGBBilateraled, frameRGBResized, frameRGBBluredResized,
			frameGray, frameGrayBlured, frameGrayResized, frameGrayBluredResized, frameGrayEqualized, 
			frameCannied, frameBilateraled;
		
		// Grab a frame
		*camera >> tframe;
		cv::flip(tframe, tframe, 1);
		
		cv::cvtColor(tframe, frameRGB, cv::COLOR_BGR2RGB);
		cv::resize(frameRGB, frameRGBResized, cv::Size(), 0.5, 0.5);
		cv::GaussianBlur(frameRGBResized, frameRGBBluredResized, cv::Size( 5, 5 ), 0, 0 );
		
		cv::cvtColor(tframe, frameGray, cv::COLOR_BGR2GRAY);
		cv::resize(frameGray, frameGrayResized, cv::Size(), 0.5, 0.5);
		cv::GaussianBlur(frameGrayResized, frameGrayBluredResized, cv::Size( 5, 5 ), 0, 0 );
				
		//
		cv::equalizeHist(frameGray, frameGrayEqualized);
		cv::Canny(tframe, frameCannied, 10, 20, 7, true);
		//cv::erode(frame, tleftEye, kernel);
		
		frame = frameCannied;
		
		IplImage imgRGB = cvIplImage(frameRGBBluredResized);
		IplImage imgGray = cvIplImage(frameGrayBluredResized);
		
		dlib::cv_image<dlib::rgb_pixel> cimgRGB(&imgRGB);
		dlib::cv_image<unsigned char> cimgGray(&imgGray);
		
		dlibDetectAndDraw(frame, cimgRGB, detector, pose_model, cv::Scalar(255,255,0));
		dlibDetectAndDraw(frame, cimgGray, detector, pose_model, cv::Scalar(127,127,127));

		
		glfwMakeContextCurrent(window1);
		glBindTexture(GL_TEXTURE_2D, *texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frame.cols, frame.rows, 0, GL_RED, GL_UNSIGNED_BYTE, frame.data);
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(programID);
		glBindVertexArray(*VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glfwSwapBuffers(window1);

		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
	}
	
	// Cleanup VBO
	glDeleteBuffers(1, VBO);
	glDeleteBuffers(1, EBO);
	glDeleteVertexArrays(1, VAO);
	glDeleteProgram(programID);
	
	camera->release();
	
	// Terminates GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}