#include "app.hpp"

namespace app {

App* App::instance = nullptr;
Logger* App::logger = nullptr;

App::App() {}
App::~App() {}

App* App::get_instance() {
	if (instance == nullptr)
		instance = new App();
	return instance;
}

void App::release_instance() {
	if (instance != nullptr)
		delete instance;
	instance = nullptr;
}

bool App::initialize() {
	logger = Logger::getInstance();
	logger->restart_gl_log();
	logger->gl_log("initialize appication\n");
	
	useWebcam = false;
	
	mainWindow = new Window(800, 600);
	mainWindow->logger = logger;

	if (!mainWindow->create_window())
		return false;
	if (!mainWindow->initializeGL())
		return false;
	if (!mainWindow->initializeGui())
		return false;
	
	return true;
}

bool App::run() {
	logger->gl_log("run appication");
	
	mainWindow->configureWindow();
	mainWindow->configureGui();
	//useWebcam = camera->openCamera();
	
	while (!mainWindow->isClosingWindows()) {
		
		switch (state) {
			case SPLASH_SCREEN: 
				break;
			case MAIN_MENU:
				if (useWebcam) {
					recognizer->boolFromGui  = mainWindow->boolToRecognizer;
					recognizer->colorFromGui = convert_to_cvColor(mainWindow->colorToRecognizer);
					if (camera->isFrameGrabed())
						recognizer->frame = camera->getFrame();
					mainWindow->render->_data    = recognizer->frameRGB.data;
					std::array<float,3> face_rotation = recognizer->get_face_rotation();
					glm::mat4 rotation_matrix(1.0);
					rotation_matrix = glm::rotate(rotation_matrix,  face_rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));  // forward-back
					rotation_matrix = glm::rotate(rotation_matrix, -face_rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));  // x-rotation
					rotation_matrix = glm::rotate(rotation_matrix, -face_rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));  // tilt
					mainWindow->render->setHeadModelTransformation(rotation_matrix);
				}
				
				mainWindow->render->_width   = recognizer->frameRGB.cols;
				mainWindow->render->_height  = recognizer->frameRGB.rows;
				
				mainWindow->draw();
				break;
			case OPTIONS_MENU:
				break;
			case CREDITS:
				break;
			case FULL_FRAME:
				if (useWebcam) {
					recognizer->boolFromGui  = mainWindow->boolToRecognizer;
					recognizer->colorFromGui = convert_to_cvColor(mainWindow->colorToRecognizer);
					if (camera->isFrameGrabed())
						recognizer->frame = camera->getFrame();
					mainWindow->render->_data    = recognizer->frameRGB.data;
					std::array<float,3> face_rotation = recognizer->get_face_rotation();
					glm::mat4 rotation_matrix(1.0);
					rotation_matrix = glm::rotate(rotation_matrix,  face_rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));  // forward-back
					rotation_matrix = glm::rotate(rotation_matrix, -face_rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));  // x-rotation
					rotation_matrix = glm::rotate(rotation_matrix, -face_rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));  // tilt
					mainWindow->render->setHeadModelTransformation(rotation_matrix);
				}
				
				mainWindow->render->_width   = mainWindow->width;
				mainWindow->render->_height  = mainWindow->height;
				mainWindow->draw();
				break;
			default:
				break;
		}
		
		mainWindow->updateWindow();
	}
	
	mainWindow->cleanup();
	
	// Release OpenCV webcam capture
	camera->releaseCamera();
	camera->releaseInstance();
	
	// Terminates GLFW, clearing any resources allocated by GLFW.
	mainWindow->terminateWindow();
	//window->releaseInstance();
	
	return true;
}

void App::stop() {
	instance->release_instance();
}

std::map<unsigned int, cv::Scalar> App::convert_to_cvColor(std::map<unsigned int, nanogui::Color> &ngColors) {
	std::map<unsigned int, cv::Scalar> cvColors;
	for (auto &ngColor : ngColors) {
		cvColors[ngColor.first][0] = ngColor.second.r()*255;
		cvColors[ngColor.first][1] = ngColor.second.g()*255;
		cvColors[ngColor.first][2] = ngColor.second.b()*255;
	}
	return cvColors;
}

}

