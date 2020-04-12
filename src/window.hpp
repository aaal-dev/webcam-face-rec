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
	static bool initialize();
	static bool initializeGL();
	static bool initializeGui();
	static bool createWindow();
	static bool isClosingWindows();
	static void configureWindow();
	static void updateWindow();
	static void terminateWindow();
	static void configureGui();
	static void updateTitle();
	void draw();
	static void cleanup();
	
	// Variables
	static char* title;
	static Renderer* render;
	static bool bgrbvar;
	static bool bgrhbvar;
	static bool bgrhbbvar;
	static bool graybvar;
	static bool grayhbvar;
	static bool grayhbbvar;
	static nanogui::Color bgrcolval;
	static nanogui::Color bgrhcolval;
	static nanogui::Color bgrhbcolval;
	static nanogui::Color graycolval;
	static nanogui::Color grayhcolval;
	static nanogui::Color grayhbcolval;
	
private:
	Window();
	~Window();
	
	// Functions
	static void startTimer();
	static float getSpeedOnMS();
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void cursor_position_callback(GLFWwindow* window, double x, double y);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int modifiers);
	static void character_callback(GLFWwindow* window, unsigned int codepoint);
	static void drop_callback(GLFWwindow* window, int count, const char **filenames);
	static void scroll_callback(GLFWwindow* window, double x, double y);
	
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
	
	static nanogui::Screen* screen;
	static nanogui::FormHelper* gui;
	
	
};

}

#endif // WINDOW_HPP
