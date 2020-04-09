#include "window.hpp"

namespace app 
{

Window* Window::instance = nullptr;
GLFWwindow* Window::window = nullptr;
bool Window::drawActualPoints = false;
bool Window::drawCorrectedPoints = false;
double Window::startTime;
double Window::numberOfTicks;
float Window::speed;
char* Window::title = (char*)"Demo App";
nanogui::Screen* screen = nullptr;

Renderer* Window::render = nullptr;

Window::Window()
{
	instance->initialize();
	instance->startTimer();
	render = Renderer::getInstance();
}

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
	if(glfwInit())
	{
		// Set all the required options for GLFW
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif 
		glfwWindowHint(GLFW_SAMPLES, 0);
		glfwWindowHint(GLFW_RED_BITS, 8);
		glfwWindowHint(GLFW_GREEN_BITS, 8);
		glfwWindowHint(GLFW_BLUE_BITS, 8);
		glfwWindowHint(GLFW_ALPHA_BITS, 8);
		glfwWindowHint(GLFW_STENCIL_BITS, 8);
		glfwWindowHint(GLFW_DEPTH_BITS, 24);
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		return true;
	}
	fprintf( stderr, "Failed to initialize GLFW\n" );
	getchar();
	return false;
}

bool Window::createWindow() 
{
	window = glfwCreateWindow(800, 600, "Face", nullptr, nullptr);
	if (!window == NULL)
	{
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetKeyCallback(window, key_callback);
		return true;
	}
	fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n" );
	getchar();
	glfwTerminate();
	return false;
}

bool Window::initializeGL()
{
	return render->initialize((GLADloadproc)glfwGetProcAddress);
}

bool Window::initializeGui()
{
	return true;
}


void Window::terminateWindow()
{
	glfwTerminate();
}

bool Window::isShouldClose() 
{
	return glfwWindowShouldClose(window);
}

void Window::updateSpeedInfo() 
{
	char* titlestr = new char[255];
	sprintf(titlestr, "%s (%.1f ms)", title, getSpeedOnMS());
	glfwSetWindowTitle(window, titlestr);
}

void Window::draw()
{
	// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
	glfwPollEvents();
	render->draw();
	screen->drawContents();
	screen->drawWidgets();
	glfwSwapBuffers(window);
}

void Window::cleanup()
{
	render->cleanup();
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
	Renderer::getInstance()->changeViewport(0,0, width, height);
}

void Window::startTimer() 
{
	startTime = glfwGetTime();
	numberOfTicks = 0;
	speed = 0;
}

float Window::getSpeedOnMS()
{
	numberOfTicks++;
	double elastedTime = glfwGetTime() - startTime;
	if (elastedTime >= 1.0) 
	{
		speed = 1000.0/numberOfTicks;
		numberOfTicks = 0;
		startTime += 1.0;
	}
	return speed;
}



}

