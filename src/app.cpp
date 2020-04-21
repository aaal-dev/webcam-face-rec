#include "app.hpp"

namespace app
{

App* App::instance = nullptr;
bool App::useWebcam = false;
Window* App::window = nullptr;
VideoInput* App::camera = nullptr;
Recognizer* App::recognizer = nullptr;


App::App()
{
	window = Window::getInstance();
	camera = VideoInput::getInstance();
	recognizer = Recognizer::getInstance();
}

App::~App(){}

App* App::getInstance() 
{
	if (instance == nullptr)
		instance = new App();
	return instance;
}

void App::releaseInstance()
{
	if (instance != nullptr)
		delete instance;
	instance = nullptr;
}

bool App::initialize() 
{
	if (window->createWindow())
		if (window->initializeGL())
			if (window->initializeGui())
				return true;
	return false;
}

bool App::run()
{
	window->configureWindow();
	window->configureGui();
	useWebcam = camera->openCamera();
	while (!window->isClosingWindows()) 
	{
		window->updateWindow();
		if (useWebcam)
		{
			// Grab a frame
			recognizer->frame = camera->grabFrame();
			recognizer->detectFaces();
			
			//glm::mat4 transformation = recognizer->
			//window->render->setHeadModelTransformation(transformation);
			
			window->render->_width   = recognizer->frameRGB.cols;
			window->render->_height  = recognizer->frameRGB.rows;
			window->render->_data    = recognizer->frameRGB.data;
			
			recognizer->bgrbvar      = window->bgrbvar;
			recognizer->bgrhbvar     = window->bgrhbvar;
			recognizer->bgrhbbvar    = window->bgrhbbvar;
			recognizer->graybvar     = window->graybvar;
			recognizer->grayhbvar    = window->grayhbvar;
			recognizer->grayhbbvar   = window->grayhbbvar;
			
			recognizer->bgrcolval[0]    = window->bgrcolval.r()*255;
			recognizer->bgrcolval[1]    = window->bgrcolval.g()*255;
			recognizer->bgrcolval[2]    = window->bgrcolval.b()*255;
			recognizer->bgrhcolval[0]   = window->bgrhcolval.r()*255;
			recognizer->bgrhcolval[1]   = window->bgrhcolval.g()*255;
			recognizer->bgrhcolval[2]   = window->bgrhcolval.b()*255;
			recognizer->bgrhbcolval[0]  = window->bgrhbcolval.r()*255;
			recognizer->bgrhbcolval[1]  = window->bgrhbcolval.g()*255;
			recognizer->bgrhbcolval[2]  = window->bgrhbcolval.b()*255;
			recognizer->graycolval[0]   = window->graycolval.r()*255;
			recognizer->graycolval[1]   = window->graycolval.g()*255;
			recognizer->graycolval[2]   = window->graycolval.b()*255;
			recognizer->grayhcolval[0]  = window->grayhcolval.r()*255;
			recognizer->grayhcolval[1]  = window->grayhcolval.g()*255;
			recognizer->grayhcolval[2]  = window->grayhcolval.b()*255;
			recognizer->grayhbcolval[0] = window->grayhbcolval.r()*255;
			recognizer->grayhbcolval[1] = window->grayhbcolval.g()*255;
			recognizer->grayhbcolval[2] = window->grayhbcolval.b()*255;
		}
		window->draw();
	}
	
	window->cleanup();
	
	// Release OpenCV webcam capture
	camera->releaseCamera();
	camera->releaseInstance();
	
	// Terminates GLFW, clearing any resources allocated by GLFW.
	window->terminateWindow();
	window->releaseInstance();
	
	return true;
}

}

