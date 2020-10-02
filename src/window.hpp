#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <map>

// GLFW
#include <GLFW/glfw3.h>

// Inner classes
#include "logger.hpp"

namespace app {

class Window {
	
public:
	
	
	
	// Variables
	static Logger* logger;
	
	char* title;
	//std::map<unsigned int, bool> boolToRecognizer;
	//std::map<std::string, bool> boolToRenderer;
	//std::map<unsigned int, nanogui::Color> colorToRecognizer;
	
	int width;
	int height;
	
	// Functions
	static Window* get_instance();
	
	bool initialize();
	GLFWwindow* create_window();
	
	
	void configure_window(GLFWwindow* window);
	bool is_closing_window(GLFWwindow* window);
	void updateWindow();
	void terminateWindow();
	void configureGui();
	void updateTitle();
	void draw(GLFWwindow* window);
	void cleanup();
	
	
	
private:
	Window();
	~Window();
	
	
	// Variables
	static Window* instance;
	
	double startTime;
	double numberOfTicks;
	float speed;
	
	//nanogui::Screen* screen;
	//nanogui::FormHelper* gui;
	
	
	// Functions
	static void release_instance();
	void startTimer();
	float getSpeedOnMS();
	
	// Callback functions
	static void glfw_error_callback(int error, const char* description);
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
