#ifndef WINDOW_HPP
#define WINDOW_HPP

//#include <stdio.h>
#include <map>

// Inner classes
#include "renderer.hpp"

namespace app
{

class Window
{
public:
	// Variables
	char* title;
	static Renderer* render;
	std::map<unsigned int, bool> boolToRecognizer;
	std::map<std::string, bool> boolToRenderer;
	std::map<unsigned int, nanogui::Color> colorToRecognizer;
	
	int width;
	int height;
	
	// Functions
	static Window* getInstance();
	static void releaseInstance();
	bool initialize();
	bool initializeGL();
	bool initializeGui();
	bool createWindow();
	bool isClosingWindows();
	void configureWindow();
	void updateWindow();
	void terminateWindow();
	void configureGui();
	void updateTitle();
	void draw();
	void cleanup();
	
	static int pressedKey();
	
private:
	Window();
	~Window();
	
	
	// Variables
	static Window* instance;
	GLFWwindow* window;
	static bool drawActualPoints;
	static bool drawCorrectedPoints;
	static int keyPressed;
	
	double startTime;
	double numberOfTicks;
	float speed;
	
	static nanogui::Screen* screen;
	nanogui::FormHelper* gui;
	
	
	// Functions
	void startTimer();
	float getSpeedOnMS();
	
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void cursor_position_callback(GLFWwindow* window, double x, double y);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int modifiers);
	static void character_callback(GLFWwindow* window, unsigned int codepoint);
	static void drop_callback(GLFWwindow* window, int count, const char **filenames);
	static void scroll_callback(GLFWwindow* window, double x, double y);
	
	
	
	
};

}

#endif // WINDOW_HPP
