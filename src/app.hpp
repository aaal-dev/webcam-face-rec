#ifndef APP_H
#define APP_H

// Live2D


// OpenCV
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>

// CMUSphinx
//#include <pocketsphinx/pocketsphinx.h>

// Inner classes
#include "log.hpp"
#include "statecon.hpp"

#include "config.hpp"
#include "render.hpp"
#include "window.hpp"
#include "gui.hpp"
//#include "recognizer.hpp"
#include "videoinput.hpp"
#include "command.hpp"


namespace app {

class App {
	
public:
	// Variables
	
	
	// Functions
	static App* getApp();
	bool initialize();
	void run();
	void stop();
	
	
private:
	App();
	~App();
	
	
	// Variables
	static App* app;
	static Log* logger;
	static StateCon* stater;
	Config* configurer;
	Window* windower;
	GLFWwindow* mainWindow;
	Render* renderer;
	Gui* guier;
	VideoInput* camera;
	//Recognizer* recognizer;
	Command* commander;
	bool isRunning;
	
	
	
	// Functions
	static void releaseApp();
	void processInput();
	void updateApp();
	void generateOutput();
	
	//std::map<unsigned int, cv::Scalar> convert_to_cvColor(std::map<unsigned int, nanogui::Color> &ngColors);
	
	
};

}

#endif // APP_H
