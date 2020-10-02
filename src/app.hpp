#ifndef APP_H
#define APP_H

// Live2D


// OpenCV
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>

// CMUSphinx
//#include <pocketsphinx/pocketsphinx.h>

// Inner classes
#include "renderer.hpp"
#include "window.hpp"
#include "gui.hpp"

#include "logger.hpp"
//#include "recognizer.hpp"
//#include "videoinput.hpp"


namespace app {

class App {
	
public:
	// Variables
	
	
	// Functions
	static App* get_instance();
	bool initialize();
	bool run();
	void stop();
	
	
private:
	App();
	~App();
	
	
	// Enums
	enum StatesId {
		SPLASH_SCREEN,
		MAIN_MENU,
		OPTIONS_MENU,
		CREDITS,
		FULL_FRAME,
		NUM_STATES
	};
	
	
	// Variables
	static App* instance;
	static Logger* logger;
	static Window* windower;
	static Renderer* render;
	static Gui* guier;
	
	GLFWwindow* mainWindow;
	
	
	StatesId state;
	
	//bool useWebcam;
	
	//VideoInput* camera;
	//Recognizer* recognizer;
	
	
	// Functions
	static void release_instance();
	//std::map<unsigned int, cv::Scalar> convert_to_cvColor(std::map<unsigned int, nanogui::Color> &ngColors);
	
	
};

}

#endif // APP_H
