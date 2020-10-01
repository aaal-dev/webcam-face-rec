#include "window.hpp"

namespace app {

Logger*   Window::logger = nullptr;
Renderer* Window::render = nullptr;

Window::Window() {
	title = (char* )"Demo App";
	
	boolToRenderer = {{"webcam", false}, 
					  {"head", false}};
	colorToRecognizer = {{0, nanogui::Color(1.0f, 0.0f, 0.0f, 1.f)},
						 {1, nanogui::Color(0.0f, 0.0f, 1.0f, 1.f)},
						 {2, nanogui::Color(1.0f, 0.0f, 1.0f, 1.f)},
						 {3, nanogui::Color(0.0f, 1.0f, 0.0f, 1.f)},
						 {4, nanogui::Color(1.0f, 1.0f, 0.0f, 1.f)},
						 {5, nanogui::Color(0.0f, 1.0f, 1.0f, 1.f)}};
	initialize();
}

Window::Window(int width, int height) {
	this->title = (char* )"Demo App";
	this->width = width;
	this->height = height;
	
	
	
	initialize();
}

Window::~Window() {}

void Window::initialize() {
	if(!glfwInit()) 	// Initialize GLFW
		logger->gl_log_err("Failed to initialize GLFW\n");
	
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
	
	startTimer();
}

bool Window::initializeGL() {
	return render->initialize((GLADloadproc)glfwGetProcAddress);
}

bool Window::initializeGui() {
	//screen = new nanogui::Screen();
	//if (screen) {
	//	screen->initialize(window, true);
		return true;
	//}
	//fprintf( stderr, "Failed to initialize screen for nanogui. \n" );
	//getchar();
	//return false;
}

bool Window::create_window() {
	window = glfwCreateWindow(width, height, "Face", nullptr, nullptr);
	if (!window) {
		logger->gl_log_err("Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n" );
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	return true;
}

void Window::updateWindow() {
	updateTitle();
}

void Window::configureWindow() {
	glfwSetErrorCallback(Window::glfw_error_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);
    glfwSetCharCallback(window, character_callback);
    glfwSetDropCallback(window, drop_callback);
    glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glEnable(GL_CULL_FACE);
	glfwSwapInterval(1);
	glfwSwapBuffers(window);
}

bool Window::isClosingWindows() {
	return glfwWindowShouldClose(window);
}

void Window::terminateWindow() {
	glfwTerminate();
}



void Window::configureGui() {
	//gui = new nanogui::FormHelper(screen);
	//nanogui::ref<nanogui::Window> nanoguiWindow = 
	//gui->addWindow(Eigen::Vector2i(10, 10), "Settings");
	
	//gui->addGroup("Use frame");
	
	//gui->addVariable("Full-sized BGR", boolToRecognizer[0]);
	//gui->addVariable("Color", colorToRecognizer[0]);
	
	//gui->addVariable("Half-sized BGR", boolToRecognizer[1]);
	//gui->addVariable("Color", colorToRecognizer[1]);
	
	//gui->addVariable("Blured half-sized BGR", boolToRecognizer[2]);
	//gui->addVariable("Color", colorToRecognizer[2]);
	
	//gui->addVariable("Full-sized gray", boolToRecognizer[3]);
	//gui->addVariable("Color", colorToRecognizer[3]);
	
	//gui->addVariable("Half-sized gray", boolToRecognizer[4]);
	//gui->addVariable("Color", colorToRecognizer[4]);
	
	//gui->addVariable("Blured half-sized gray", boolToRecognizer[5]);
	//gui->addVariable("Color", colorToRecognizer[5]);
	
	//gui->addGroup("Meshes");
	
	//gui->addVariable("Webcam", boolToRenderer.at("webcam"));
	//gui->addVariable("Face model", boolToRenderer.at("head"));
	
	//screen->setVisible(true);
	//screen->performLayout();
}

void Window::draw() {
	// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
//	glm::mat4 rotation_matrix(1.0);
//	rotation_matrix = glm::rotate(rotation_matrix,  face_rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));  // forward-back
//	rotation_matrix = glm::rotate(rotation_matrix, -face_rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));  // x-rotation
//	rotation_matrix = glm::rotate(rotation_matrix, -face_rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));  // tilt
//	render->setHeadModelTransformation(rotation_matrix);
	render->setDrawProperties(boolToRenderer);
	render->draw();
	//screen->drawContents();
	//screen->drawWidgets();
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::cleanup() {
	render->cleanup();
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
	render->changeViewport(0,0, width, height);
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

