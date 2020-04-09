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
	useWebcam = camera->openCamera();
	while (!window->isShouldClose()) 
	{
		window->updateSpeedInfo();
		if (useWebcam)
		{
			// Grab a frame
			camera->grabFrame(&faceshapes->frame);
			faceshapes->detectFaceShape();
		
			window->render->_width = faceshapes->frameRGB.cols;
			window->render->_height = faceshapes->frameRGB.rows;
			window->render->_data = faceshapes->frameRGB.data;
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

