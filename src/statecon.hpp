#ifndef STATECON_HPP
#define STATECON_HPP

#include "log.hpp"

namespace app {

class StateCon {
	
public:
	
	// Enums
	enum StatesId {
		SPLASH_SCREEN,
		MAIN_MENU,
		OPTIONS_MENU,
		CREDITS,
		FULL_FRAME,
		EXIT_APP,
		NUM_STATES
	};
	
	enum MouseButtonId {
		MOUSE_BUTTON_1,
		MOUSE_BUTTON_2,
		MOUSE_BUTTON_3,
		MOUSE_BUTTON_4,
		MOUSE_BUTTON_5,
		MOUSE_BUTTON_6,
		MOUSE_BUTTON_7,
		MOUSE_BUTTON_8,
		NUM_MOUSE_BUTTONS
	};
	
	MouseButtonId MOUSE_BUTTON_LAST 	= MOUSE_BUTTON_8;
	int MOUSE_BUTTON_LEFT 	= MOUSE_BUTTON_1;
	int MOUSE_BUTTON_RIGHT 	= MOUSE_BUTTON_2;
	int MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_3;
	
	
	// Variables
	StatesId appState;
	double mouse_pos_x;
	double mouse_pos_y;
	bool mouseButton[8] = {false};
	float framebuffer_width;
	float framebuffer_height;
	
	
	// Functions
	static StateCon* getStateCon();
	static void releaseStateCon();
	void setAppState(StatesId appState);
	StatesId getAppState();
	
	
	
private:
	StateCon();
	~StateCon();
	
	
	// Variables
	static StateCon* stateCon;
	static Log* logger;
	
	
	// Functions
	

};

}

#endif // STATECON_HPP
