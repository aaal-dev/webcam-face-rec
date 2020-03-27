#include "app.hpp"

namespace app
{

App* App::instance = nullptr;




App::App(){}
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
	Timer::getInstance()->start();
	
	if (!Window::getInstance()->createWindow())
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		return -1;
	}
	
	VideoInput::getInstance()->openCamera();
	
	
	
	if (!OpenGL::getInstance()->initialize(Window::getInstance()->getProcAddress())) {
		fprintf( stderr, "Failed to initialize OpenGL context.\n" );
		getchar();
		return -1;
	}
	
	OpenGL::getInstance()->run();
	
	return true;
}

bool App::run()
{
	char* titlestr = new char[255];
	while (!Window::getInstance()->isShouldClose()) 
	{
		sprintf( titlestr, "videoInput Demo App (%.1f ms)", Timer::getSpeedOnMS() );
		Window::getInstance()->changeTitle(titlestr);
		
		// Grab a frame
		VideoInput::getInstance()->grabFrame(&FaceShapes::getInstance()->frame);
		FaceShapes::getInstance()->detectFaceShape();
		
		OpenGL::getInstance()->width = FaceShapes::getInstance()->frameRGB.cols;
		OpenGL::getInstance()->height = FaceShapes::getInstance()->frameRGB.rows;
		OpenGL::getInstance()->data = FaceShapes::getInstance()->frameRGB.data;
		
		OpenGL::getInstance()->draw();
		Window::getInstance()->swapBuffers();
		
	}
	
	OpenGL::getInstance()->cleanup();
	
	// Release OpenCV webcam capture
	VideoInput::getInstance()->releaseCamera();
	VideoInput::releaseInstance();
	
	// Terminates GLFW, clearing any resources allocated by GLFW.
	Window::getInstance()->terminateWindow();
	Window::releaseInstance();
	
	return true;
}

}

