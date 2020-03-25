#include "window.hpp"

namespace app 
{

Window* Window::instance = nullptr;
GLFWwindow* Window::window = nullptr;
bool Window::drawActualPoints = false;
bool Window::drawCorrectedPoints = false;

Window::Window(){}
Window::~Window(){}

Window* Window::getInstance() 
{
	if (instance == nullptr)
		instance = new Window();
	return instance;
}

void Window::releaseInstance()
{
	if (instance != nullptr)
		delete instance;
	instance = nullptr;
}

bool Window::initialize()
{
	// Initialize GLFW
	if(!glfwInit())
		return -1;
	
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif 
	return true;
}

bool Window::createWindow() 
{
	window = glfwCreateWindow(800, 600, "Face", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	return true;
}

bool Window::isShouldClose() 
{
	return glfwWindowShouldClose(window);
}

void Window::changeTitle(char* titlestr) 
{
	glfwSetWindowTitle(window, titlestr);
}

// Is called whenever a key is pressed/released via GLFW
void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;
			case GLFW_KEY_F1:
				if (drawActualPoints == false)
					drawActualPoints = true;
				else
					drawActualPoints = false;
				break;
			case GLFW_KEY_F2:
				if (drawCorrectedPoints == false)
					drawCorrectedPoints = true;
				else
					drawCorrectedPoints = false;
				break;
		}
	}
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
}

}

