#include "app.hpp"

namespace app {

App* App::app = nullptr;
Log* App::logger = nullptr;
StateCon* App::stater = nullptr;

App::App () {
	logger = Log::getLog();
	stater = StateCon::getStateCon();
}

App::~App () {}

App* App::getApp () {
	if (app == nullptr)
		app = new App();
	return app;
}

void App::releaseApp () {
	if (app != nullptr)
		delete app;
	app = nullptr;
}

bool App::initialize () {
	logger->initialize();
	logger->write(Log::LOG_INFO, "initialize appication\n");
	
	stater->setAppState(StateCon::SPLASH_SCREEN);
	stater->framebuffer_width = 800;
	stater->framebuffer_height = 600;
	
	configurer = new Config();
	
	windower = new Window();
	if (!windower->initialize ())
		return false;
		
	mainWindow = windower->create_window();
	windower->configure_window(mainWindow);
	
	renderer = new Render();
	if (!renderer->initialize_GL ((GLADloadproc)glfwGetProcAddress))
		return false;
	
	guier = new Gui();
	if (!guier->initialize_gui())
		return false;
	
	camera = new VideoInput();
	camera->useWebcam = false;
	if (!camera->openCamera())
		return false;
	
	commander = new Command();
	
	return true;
}

void App::run () {
	logger->logGLInfo ("run appication");
	
	guier->configure_gui();
	
	while (!windower->is_closing_window (mainWindow)) {
		renderer->drawCleanUp ();
		switch (stater->getAppState ()) {
			case stater->SPLASH_SCREEN: {
				//if ()
				stater->setAppState (stater->MAIN_MENU);
				break;
			}
			case stater->MAIN_MENU: {
				if (camera->isFrameGrabed()) {
					cv::Mat frame = camera->getFrameData();
					renderer->cameraFrame.data = frame.data;
					renderer->cameraFrame.width = frame.cols;
					renderer->cameraFrame.height = frame.rows;
				}
				/*
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
				windower->render->setHeadModelTransformation(rotation_matrix);*/
				renderer->drawWebCamMesh();
				renderer->draw();
				guier->drawMainMenu();
				windower->draw(mainWindow);
				break;
			}
			case stater->OPTIONS_MENU: {
				guier->drawOptionMenu();
				windower->draw(mainWindow);
				break;
			}
			case stater->CREDITS: {
				guier->drawCreditsMenu();
				windower->draw(mainWindow);
				break;
			}
			case stater->FULL_FRAME: {
				/*
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
				windower->render->setHeadModelTransformation(rotation_matrix);*/
				
				//windower->render->_width   = windower->width;
				//windower->render->_height  = windower->height;
				renderer->drawWebCamMesh();
				renderer->draw();
				guier->drawSideMenu();
				windower->draw(mainWindow);
				break;
			}
			case stater->EXIT_APP: {
				windower->close_window(mainWindow);
				break;
			}
			default:
				break;
		}
		windower->update_window(mainWindow);
		windower->update();
	}
}

void App::stop () {
	
	guier->cleanup();
	//windower->cleanup();
	
	// Release OpenCV webcam capture
	//camera->releaseCamera();
	//camera->releaseInstance();
	
	// Terminates GLFW, clearing any resources allocated by GLFW.
	windower->terminate_window();
	
	
	delete commander;
	//delete recognizer;
	//useWebcam = false;
	delete camera;
	delete guier;
	delete renderer;
	mainWindow = nullptr;
	delete windower;
	delete configurer;
	stater->releaseStateCon();
	logger->releaseLog();
	app->releaseApp();
}

void App::processInput () {
	
}

void App::updateApp () {
	
}

void App::generateOutput () {
	
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

