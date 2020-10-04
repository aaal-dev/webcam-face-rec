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

#include "render.hpp"
#include "window.hpp"
#include "gui.hpp"
//#include "recognizer.hpp"
#include "videoinput.hpp"


namespace app {

class App {
	
public:
	// Variables
	
	
	// Functions
	static App* getApp ();
	bool initialize ();
	bool run ();
	void stop ();
	
	
private:
	App ();
	~App ();
	
	
	// Variables
	static App* app;
	static Log* logger;
	static StateCon* stater;
	Window* windower;
	Render* renderer;
	Gui* guier;
	VideoInput* camera;
	//Recognizer* recognizer;
	
	GLFWwindow* mainWindow;
	//bool useWebcam;
	
	
	// Functions
	static void releaseApp ();
	//std::map<unsigned int, cv::Scalar> convert_to_cvColor(std::map<unsigned int, nanogui::Color> &ngColors);
	
	
};

}

#endif // APP_H
