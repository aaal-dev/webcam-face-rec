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
	window->width  = 800;
	window->height = 600;
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
		switch (window->pressedKey())
		{
			case GLFW_KEY_1:
				this->_state = SPLASH_SCREEN;
				break;
			case GLFW_KEY_2:
				this->_state = MAIN_MENU;
				break;
			case GLFW_KEY_3:
				this->_state = OPTIONS_MENU;
				break;
			case GLFW_KEY_4:
				this->_state = CREDITS;
				break;
			case GLFW_KEY_5:
				this->_state = FULL_FRAME;
				break;
		}
		
		switch (this->_state)
		{
			case SPLASH_SCREEN: 
				break;
			case MAIN_MENU:
				if (useWebcam) 
				{
					recognizer->boolFromGui  = window->boolToRecognizer;
					recognizer->colorFromGui = convertToCvColor(window->colorToRecognizer);
					if (camera->isFrameGrabed())
						recognizer->frame = camera->getFrame();
					window->render->_data    = recognizer->frameRGB.data;
					std::array<float,3> face_rotation = recognizer->get_face_rotation();
					glm::mat4 rotation_matrix(1.0);
					rotation_matrix = glm::rotate(rotation_matrix,  face_rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));  // forward-back
					rotation_matrix = glm::rotate(rotation_matrix, -face_rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));  // x-rotation
					rotation_matrix = glm::rotate(rotation_matrix, -face_rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));  // tilt
					window->render->setHeadModelTransformation(rotation_matrix);
				}
				
				window->render->_width   = recognizer->frameRGB.cols;
				window->render->_height  = recognizer->frameRGB.rows;
				
				window->draw();
				break;
			case OPTIONS_MENU:
				break;
			case CREDITS:
				break;
			case FULL_FRAME:
				if (useWebcam) 
				{
					recognizer->boolFromGui  = window->boolToRecognizer;
					recognizer->colorFromGui = convertToCvColor(window->colorToRecognizer);
					if (camera->isFrameGrabed())
						recognizer->frame = camera->getFrame();
					window->render->_data    = recognizer->frameRGB.data;
					std::array<float,3> face_rotation = recognizer->get_face_rotation();
					glm::mat4 rotation_matrix(1.0);
					rotation_matrix = glm::rotate(rotation_matrix,  face_rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));  // forward-back
					rotation_matrix = glm::rotate(rotation_matrix, -face_rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));  // x-rotation
					rotation_matrix = glm::rotate(rotation_matrix, -face_rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));  // tilt
					window->render->setHeadModelTransformation(rotation_matrix);
				}
				
				window->render->_width   = window->width;
				window->render->_height  = window->height;
				window->draw();
				break;
			default:
				break;
		}
		
		window->updateWindow();
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

bool App::stop()
{
	return true;
}

std::map<unsigned int, cv::Scalar> App::convertToCvColor(std::map<unsigned int, nanogui::Color> &ngColors)
{
	std::map<unsigned int, cv::Scalar> cvColors;
	for (auto &ngColor : ngColors)
	{
		cvColors[ngColor.first][0] = ngColor.second.r()*255;
		cvColors[ngColor.first][1] = ngColor.second.g()*255;
		cvColors[ngColor.first][2] = ngColor.second.b()*255;
	}
	return cvColors;
}

}

