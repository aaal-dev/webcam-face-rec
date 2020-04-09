#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <stdio.h>

// Inner classes
#include "renderer.hpp"

namespace app
{

class Window
{
public:
	// Functions
	static Window* getInstance();
	static void releaseInstance();
	bool initialize();
	bool initializeGL();
	void initializeGui();
	bool createWindow();
	static bool isClosingWindows();
	void configureWindow();
	static void updateWindow();
	static void terminateWindow();
	void configureGui();
	static void updateTitle();
	void draw();
	static void cleanup();
	
	// Variables
	static char* title;
	static Renderer* render;
	
private:
	Window();
	~Window();
	
	// Functions
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void startTimer();
	static float getSpeedOnMS();
	
	// Variables
	static Window* instance;
	static GLFWwindow* window;
	static bool drawActualPoints;
	static bool drawCorrectedPoints;
	
	static int width;
	static int height;
	static double startTime;
	static double numberOfTicks;
	static float speed;
	
	nanogui::Screen* screen;
	nanogui::FormHelper* gui;
	
	
};

}

#endif // WINDOW_HPP
