#pragma once

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

#include "shader.hpp"

namespace app
{

class App
{
	public:
	static App* GetInstance();
	static void ReleaseInstance();
	bool Initialize();
	
	
	
	private:
	App();
	~App();
	
};

};

#endif // APP_H
