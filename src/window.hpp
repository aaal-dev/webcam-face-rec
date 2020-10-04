#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <map>

// GLFW
#include <GLFW/glfw3.h>

// Inner classes
#include "log.hpp"
#include "statecon.hpp"

namespace app {

struct Monitor {
	GLFWmonitor* monitor;
	const char* name;
	bool isPrimary;
	int videoModesCount;
	const GLFWvidmode* videoModes;
	const GLFWvidmode* curentVideoMode;
};

class Window {
	
public:
	Window();
	~Window();
	
	
	// Variables
	char* title;
	//std::map<unsigned int, bool> boolToRecognizer;
	//std::map<std::string, bool> boolToRenderer;
	//std::map<unsigned int, nanogui::Color> colorToRecognizer;
	
	int width;
	int height;
	
	int monitorsCount;
	Monitor** avalableMonitors;
	Monitor* primaryMonitor;
	
	
	
	// Functions
	bool initialize();
	GLFWwindow* create_window();
	
	void configure_window(GLFWwindow* window);
	bool is_closing_window(GLFWwindow* window);
	void check_window(GLFWwindow* window);
	void update_window(GLFWwindow* window);
	void update();
	void close_window(GLFWwindow* window);
	void terminate_window();
	void update_title(GLFWwindow* window);
	void draw(GLFWwindow* window);
	void cleanup();
	
	
	
private:
	// Variables
	static Log* logger;
	static StateCon* stater;
	
	
	// Functions
	float getFPS();
	
	bool getMonitors();
	bool getPrimaryMonitor();
	
	// Callback functions
	static void glfw_error_callback(int error, const char* description);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void cursor_position_callback(GLFWwindow* window, double x, double y);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int modifiers);
	static void character_callback(GLFWwindow* window, unsigned int codepoint);
	static void scroll_callback(GLFWwindow* window, double x, double y);
	static void drop_callback(GLFWwindow* window, int count, const char **filenames);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void monitor_callback(GLFWmonitor* monitor, int event);
	
};

}

#endif // WINDOW_HPP
