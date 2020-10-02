#include "gui.hpp"

namespace app {

Gui* Gui::instance = nullptr;
Logger* Gui::logger = nullptr;

Gui::Gui()
{
}

Gui::~Gui()
{
}

Gui* Gui::get_instance() {
	if (instance == nullptr)
		instance = new Gui();
	return instance;
}

void Gui::release_instance() {
	if (instance != nullptr)
		delete instance;
	instance = nullptr;
}

bool Gui::initialize_gui() {
	//screen = new nanogui::Screen();
	//if (screen) {
	//	screen->initialize(window, true);
		return true;
	//}
	//fprintf( stderr, "Failed to initialize screen for nanogui. \n" );
	//getchar();
	//return false;
}

void Gui::configure_gui() {
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

}
