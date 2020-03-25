#ifndef WINDOW_HPP
#define WINDOW_HPP

// GLFW
#include <GLFW/glfw3.h>

namespace app
{

class Window
{
public:
	// Functions
	static Window* getInstance();
	static void releaseInstance();
	bool initialize();
	bool createWindow();
	bool isShouldClose();
	static void changeTitle(char* titlestr);
	
	
	// Variables
	
	
private:
	Window();
	~Window();
	
	// Functions
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	
	// Variables
	static Window* instance;
	static GLFWwindow* window;
	static bool drawActualPoints;
	static bool drawCorrectedPoints;
};

}

#endif // WINDOW_HPP
