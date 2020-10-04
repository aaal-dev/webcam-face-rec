#include "window.hpp"

namespace app {

Log* Window::logger = nullptr;
StateCon* Window::stater = nullptr;

Window::Window() {
	logger = Log::getLog();
	stater = StateCon::getStateCon();
	width = stater->framebuffer_width;
	height = stater->framebuffer_height;
	
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

bool Window::initialize() {
	if(!glfwInit()) {	// Initialize GLFW
		logger->logGLError("Failed to initialize GLFW\n");
		return false;
	}
	glfwSetErrorCallback(glfw_error_callback);
	logger->logGLInfo ("initialize GLFW: %s\n", glfwGetVersionString());
	
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
	
	if (!getMonitors()) {
		return false;
	}
	return true;
}

GLFWwindow* Window::create_window() {
	GLFWwindow* window = glfwCreateWindow(width, height, "Face", nullptr, nullptr);
	if (!window) {
		logger->logGLError("Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n" );
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	logger->logGLInfo("Open main GLFW window\n");
	return window;
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
}

bool Window::is_closing_window(GLFWwindow* window) {
	return glfwWindowShouldClose(window);
}

void Window::check_window(GLFWwindow* window) {}

void Window::update_window(GLFWwindow* window) {
	update_title(window);
	glfwSwapBuffers(window);
}

void Window::update() {
	glfwPollEvents();
}

void Window::close_window(GLFWwindow* window) {
	glfwSetWindowShouldClose(window, GL_TRUE);
}

void Window::terminate_window() {
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
}

void Window::cleanup() {}

float Window::getFPS() {
	static double previous_seconds = glfwGetTime ();
	static int frame_count;
	double current_seconds = glfwGetTime ();
	double elapsed_seconds = current_seconds - previous_seconds;
	static double fps;
	if (elapsed_seconds > 0.25) {
		previous_seconds = current_seconds;
		fps = (double)frame_count / elapsed_seconds;
		frame_count = 0;
	}
	frame_count++;
	return fps;
}

void Window::update_title(GLFWwindow* window) {
	char* titlestr = new char[255];
	sprintf(titlestr, "%s (%.2f)", title, getFPS());
	glfwSetWindowTitle(window, titlestr);
}

bool Window::getMonitors() {
	GLFWmonitor** monitors = glfwGetMonitors(&monitorsCount);
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	if (monitors == NULL) {
		logger->logGLError("No monitors was found by GLFW\n");
		return false;
	}
	Monitor** newMonitors = new Monitor*[monitorsCount];
	for (int i = 0; i < monitorsCount; i++) {
		Monitor newMonitor;
		newMonitor.monitor = monitors[i];
		newMonitor.name = glfwGetMonitorName(monitors[i]);
		newMonitor.videoModes = glfwGetVideoModes(monitors[i], &newMonitor.videoModesCount);
		newMonitor.curentVideoMode = glfwGetVideoMode(monitors[i]);
		newMonitor.isPrimary = false;
		if (monitors[i] == primaryMonitor) {
			newMonitor.isPrimary = true;
			this->primaryMonitor = &newMonitor;
		}
		newMonitors[i] = &newMonitor;
	}
	this->avalableMonitors = newMonitors;
	return true;
}



  ////////////////////////
 // Callback functions //
////////////////////////

void Window::glfw_error_callback(int error, const char* description) {
	logger->logGLError("GLFW ERROR: code %i msg: %s\n", error, description);
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
	stater->framebuffer_width = width;
	stater->framebuffer_height = height;
	//screen->resizeCallbackEvent(width, height);
	//render->changeViewport(0,0, width, height);
}

void Window::cursor_position_callback(GLFWwindow* window, double x, double y) {
	stater->mouse_pos_x = x;
	stater->mouse_pos_y = y;
	//screen->cursorPosCallbackEvent(x, y);
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int modifiers) {
	if (action == GLFW_PRESS) {
		switch (button) {
			case GLFW_MOUSE_BUTTON_LEFT:
				stater->mouseButton[GLFW_MOUSE_BUTTON_LEFT] = true;
				break;
			case GLFW_MOUSE_BUTTON_MIDDLE:
				stater->mouseButton[GLFW_MOUSE_BUTTON_MIDDLE] = true;
				break;
			case GLFW_MOUSE_BUTTON_RIGHT:
				stater->mouseButton[GLFW_MOUSE_BUTTON_RIGHT] = true;
				break;
		}
	} else if (action == GLFW_RELEASE) {
		switch (button) {
			case GLFW_MOUSE_BUTTON_LEFT:
				stater->mouseButton[GLFW_MOUSE_BUTTON_LEFT] = false;
				break;
			case GLFW_MOUSE_BUTTON_MIDDLE:
				stater->mouseButton[GLFW_MOUSE_BUTTON_MIDDLE] = false;
				break;
			case GLFW_MOUSE_BUTTON_RIGHT:
				stater->mouseButton[GLFW_MOUSE_BUTTON_RIGHT] = false;
				break;
		}
	}
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

