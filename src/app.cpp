#include "app.hpp"

namespace app {

App* App::instance = nullptr;
Logger* App::logger = nullptr;
Window* App::windower = nullptr;
Renderer* App::render = nullptr;
Gui* App::guier = nullptr;

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
	logger = Logger::get_instance();
	logger->restart_gl_log();
	logger->gl_log("initialize appication\n");
	
	//useWebcam = false;
	
	windower = Window::get_instance();
	windower->width = 800;
	windower->height = 600;
	windower->logger = logger;
	if (!windower->initialize())
		return false;
		
	
	
	render = Renderer::get_instance();
	render->logger = logger;
	if (!render->initialize_GL((GLADloadproc)glfwGetProcAddress))
		return false;
	
	guier = Gui::get_instance();
	guier->logger = logger;
	if (!guier->initialize_gui())
		return false;
	
	return true;
}

bool App::run() {
	logger->gl_log("run appication");
	
	mainWindow = windower->create_window();
	
	windower->configure_window(mainWindow);
	guier->configure_gui();
	//useWebcam = camera->openCamera();
	
	while (!windower->is_closing_window(mainWindow)) {
		
		switch (state) {
			case SPLASH_SCREEN: 
				break;
			case MAIN_MENU:
/*				if (useWebcam) {
					recognizer->boolFromGui  = windower->boolToRecognizer;
					recognizer->colorFromGui = convert_to_cvColor(windower->colorToRecognizer);
					if (camera->isFrameGrabed())
						recognizer->frame = camera->getFrame();
					windower->render->_data    = recognizer->frameRGB.data;
					std::array<float,3> face_rotation = recognizer->get_face_rotation();
					glm::mat4 rotation_matrix(1.0);
					rotation_matrix = glm::rotate(rotation_matrix,  face_rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));  // forward-back
					rotation_matrix = glm::rotate(rotation_matrix, -face_rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));  // x-rotation
					rotation_matrix = glm::rotate(rotation_matrix, -face_rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));  // tilt
					windower->render->setHeadModelTransformation(rotation_matrix);
				}*/
				
				//windower->render->_width   = recognizer->frameRGB.cols;
				//windower->render->_height  = recognizer->frameRGB.rows;
				
				windower->draw(mainWindow);
				break;
			case OPTIONS_MENU:
				break;
			case CREDITS:
				break;
			case FULL_FRAME:
/*				if (useWebcam) {
					recognizer->boolFromGui  = windower->boolToRecognizer;
					recognizer->colorFromGui = convert_to_cvColor(windower->colorToRecognizer);
					if (camera->isFrameGrabed())
						recognizer->frame = camera->getFrame();
					windower->render->_data    = recognizer->frameRGB.data;
					std::array<float,3> face_rotation = recognizer->get_face_rotation();
					glm::mat4 rotation_matrix(1.0);
					rotation_matrix = glm::rotate(rotation_matrix,  face_rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));  // forward-back
					rotation_matrix = glm::rotate(rotation_matrix, -face_rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));  // x-rotation
					rotation_matrix = glm::rotate(rotation_matrix, -face_rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));  // tilt
					windower->render->setHeadModelTransformation(rotation_matrix);
				}*/
				
				//windower->render->_width   = windower->width;
				//windower->render->_height  = windower->height;
				windower->draw(mainWindow);
				break;
			default:
				break;
		}
		
		windower->updateWindow();
	}
	
	windower->cleanup();
	
	// Release OpenCV webcam capture
	//camera->releaseCamera();
	//camera->releaseInstance();
	
	// Terminates GLFW, clearing any resources allocated by GLFW.
	windower->terminateWindow();
	//window->releaseInstance();
	
	return true;
}

void App::stop() {
	instance->release_instance();
}

/*std::map<unsigned int, cv::Scalar> App::convert_to_cvColor(std::map<unsigned int, nanogui::Color> &ngColors) {
	std::map<unsigned int, cv::Scalar> cvColors;
	for (auto &ngColor : ngColors) {
		cvColors[ngColor.first][0] = ngColor.second.r()*255;
		cvColors[ngColor.first][1] = ngColor.second.g()*255;
		cvColors[ngColor.first][2] = ngColor.second.b()*255;
	}
	return cvColors;
}*/

}

