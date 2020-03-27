#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <stdio.h>

// Inner classes
#include "opengl.hpp"

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
	void terminateWindow();
	bool isShouldClose();
	static void changeTitle(char* titlestr);
	static void swapBuffers();
	GLFWglproc getProcAddress();
	
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
