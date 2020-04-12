#include "app.hpp"

namespace app
{

App* App::instance = nullptr;
bool App::useWebcam = false;
Window* App::window = nullptr;
VideoInput* App::camera = nullptr;
FaceShapes* App::faceshapes = nullptr;


App::App()
{
	window = Window::getInstance();
	camera = VideoInput::getInstance();
	faceshapes = FaceShapes::getInstance();
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
			camera->grabFrame(&faceshapes->frame);
			faceshapes->detectFaceShape();
			
			window->render->_width   = faceshapes->frameRGB.cols;
			window->render->_height  = faceshapes->frameRGB.rows;
			window->render->_data    = faceshapes->frameRGB.data;
			
			faceshapes->bgrbvar      = window->bgrbvar;
			faceshapes->bgrhbvar     = window->bgrhbvar;
			faceshapes->bgrhbbvar    = window->bgrhbbvar;
			faceshapes->graybvar     = window->graybvar;
			faceshapes->grayhbvar    = window->grayhbvar;
			faceshapes->grayhbbvar   = window->grayhbbvar;
			
			faceshapes->bgrcolval[0]    = window->bgrcolval.r()*255;
			faceshapes->bgrcolval[1]    = window->bgrcolval.g()*255;
			faceshapes->bgrcolval[2]    = window->bgrcolval.b()*255;
			faceshapes->bgrhcolval[0]   = window->bgrhcolval.r()*255;
			faceshapes->bgrhcolval[1]   = window->bgrhcolval.g()*255;
			faceshapes->bgrhcolval[2]   = window->bgrhcolval.b()*255;
			faceshapes->bgrhbcolval[0]  = window->bgrhbcolval.r()*255;
			faceshapes->bgrhbcolval[1]  = window->bgrhbcolval.g()*255;
			faceshapes->bgrhbcolval[2]  = window->bgrhbcolval.b()*255;
			faceshapes->graycolval[0]   = window->graycolval.r()*255;
			faceshapes->graycolval[1]   = window->graycolval.g()*255;
			faceshapes->graycolval[2]   = window->graycolval.b()*255;
			faceshapes->grayhcolval[0]  = window->grayhcolval.r()*255;
			faceshapes->grayhcolval[1]  = window->grayhcolval.g()*255;
			faceshapes->grayhcolval[2]  = window->grayhcolval.b()*255;
			faceshapes->grayhbcolval[0] = window->grayhbcolval.r()*255;
			faceshapes->grayhbcolval[1] = window->grayhbcolval.g()*255;
			faceshapes->grayhbcolval[2] = window->grayhbcolval.b()*255;
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

