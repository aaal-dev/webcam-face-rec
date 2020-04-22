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
	std::thread t(&App::grabFrame, this);
	t.detach(); // as opposed to .join, which runs on the current thread
	while (!window->isClosingWindows()) 
	{
		window->updateWindow();
		if (useWebcam)
		{
			// Grab a frame
			if (camera->isFrameGrabed())
			{
				recognizer->frame = camera->getFrame();
				recognizer->detectFaces();
				
				std::array<float,3> face_rotation = recognizer->get_face_rotation();
				glm::mat4 rotation_matrix(1.0);
				rotation_matrix = glm::rotate(rotation_matrix,  face_rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));  // forward-back
				rotation_matrix = glm::rotate(rotation_matrix, -face_rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));  // x-rotation
				rotation_matrix = glm::rotate(rotation_matrix, -face_rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));  // tilt
				window->render->setHeadModelTransformation(rotation_matrix);
				
				window->render->_width   = recognizer->frameRGB.cols;
				window->render->_height  = recognizer->frameRGB.rows;
				window->render->_data    = recognizer->frameRGB.data;
				
				recognizer->boolVariables  = window->boolVariables;
				generateColorVariables(window->colorVariables, recognizer->colorVariables);
			}
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

void App::grabFrame()
{
	while (camera->isOpened())
		camera->grabFrame();
	return;
}

void App::generateColorVariables(std::map<unsigned int, nanogui::Color> &ngColors, std::map<unsigned int, cv::Scalar> &cvColors)
{
	for (auto &ngColor : ngColors)
	{
		cvColors[ngColor.first][0] = ngColor.second.r()*255;
		cvColors[ngColor.first][1] = ngColor.second.g()*255;
		cvColors[ngColor.first][2] = ngColor.second.b()*255;
	}
}

}

