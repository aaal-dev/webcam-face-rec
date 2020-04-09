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
	bool initializeGui();
	bool createWindow();
	void terminateWindow();
	bool isShouldClose();
	static void updateSpeedInfo();
	static void draw();
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
	
	static double startTime;
	static double numberOfTicks;
	static float speed;
	
	nanogui::Screen* screen;
	
	
};

}

#endif // WINDOW_HPP
