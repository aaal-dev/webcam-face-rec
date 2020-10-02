#include "window.hpp"

namespace app {

Window* Window::instance = nullptr;
Logger* Window::logger = nullptr;

Window::Window() {
	title = (char* )"Demo App";
	
	//boolToRenderer = {{"webcam", false}, 
	//				  {"head", false}};
	//colorToRecognizer = {{0, nanogui::Color(1.0f, 0.0f, 0.0f, 1.f)},
	//					 {1, nanogui::Color(0.0f, 0.0f, 1.0f, 1.f)},
	//					 {2, nanogui::Color(1.0f, 0.0f, 1.0f, 1.f)},
	//					 {3, nanogui::Color(0.0f, 1.0f, 0.0f, 1.f)},
	//					 {4, nanogui::Color(1.0f, 1.0f, 0.0f, 1.f)},
	//					 {5, nanogui::Color(0.0f, 1.0f, 1.0f, 1.f)}};
}

Window::~Window() {}

Window* Window::get_instance() {
	if (instance == nullptr)
		instance = new Window();
	return instance;
}

void Window::release_instance() {
	if (instance != nullptr)
		delete instance;
	instance = nullptr;
}

bool Window::initialize() {
	if(!glfwInit()) {	// Initialize GLFW
		logger->gl_log_err("Failed to initialize GLFW\n");
		return false;
	}
	logger->gl_log ("initialize GLFW\n%s\n", glfwGetVersionString());
	
	// Set the required options for GLFW
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
	
	glfwSetErrorCallback(glfw_error_callback);
	glfwSwapInterval(1);
	
	startTimer();
	return true;
}

GLFWwindow* Window::create_window() {
	GLFWwindow* window = glfwCreateWindow(width, height, "Face", nullptr, nullptr);
	if (!window) {
		logger->gl_log_err("Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n" );
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	logger->gl_log("Open mian window");
	return window;
}

void Window::updateWindow() {
	updateTitle();
}

void Window::configure_window(GLFWwindow* window) {
	glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);
    glfwSetCharCallback(window, character_callback);
    glfwSetDropCallback(window, drop_callback);
    glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwGetFramebufferSize(window, &width, &height);
	glfwSwapBuffers(window);
}

bool Window::is_closing_window(GLFWwindow* window) {
	return glfwWindowShouldClose(window);
}

void Window::terminateWindow() {
	glfwTerminate();
}


void Window::draw(GLFWwindow* window) {
	// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
//	glm::mat4 rotation_matrix(1.0);
//	rotation_matrix = glm::rotate(rotation_matrix,  face_rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));  // forward-back
//	rotation_matrix = glm::rotate(rotation_matrix, -face_rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));  // x-rotation
//	rotation_matrix = glm::rotate(rotation_matrix, -face_rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));  // tilt
//	render->setHeadModelTransformation(rotation_matrix);
	//render->setDrawProperties(boolToRenderer);
	//render->draw();
	//screen->drawContents();
	//screen->drawWidgets();
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::cleanup() {
	//render->cleanup();
}



void Window::startTimer() {
	startTime = glfwGetTime();
	numberOfTicks = 0;
	speed = 0;
}

float Window::getSpeedOnMS() {
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

void Window::updateTitle() {
	char* titlestr = new char[255];
	sprintf(titlestr, "%s (%.1f ms)", title, getSpeedOnMS());
	glfwSetWindowTitle(window, titlestr);
}


  ////////////////////////
 // Callback functions //
////////////////////////

void Window::glfw_error_callback(int error, const char* description) {
	logger->gl_log_err("GLFW ERROR: code %i msg: %s\n", error, description);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;
			case GLFW_KEY_F1:
				break;
			case GLFW_KEY_F2:
				break;
		}
	}
	//screen->keyCallbackEvent(key, scancode, action, mode);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	(void)window;
	//screen->resizeCallbackEvent(width, height);
	//render->changeViewport(0,0, width, height);
}

void Window::cursor_position_callback(GLFWwindow* window, double x, double y) {
	//screen->cursorPosCallbackEvent(x, y);
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int modifiers) {
	//screen->mouseButtonCallbackEvent(button, action, modifiers);
}

void Window::character_callback(GLFWwindow* window, unsigned int codepoint) {
	//screen->charCallbackEvent(codepoint);
}

void Window::drop_callback(GLFWwindow* window, int count, const char **filenames) {
	//screen->dropCallbackEvent(count, filenames);
}

void Window::scroll_callback(GLFWwindow* window, double x, double y) {
	//screen->scrollCallbackEvent(x, y);
}

}

