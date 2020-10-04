#ifndef GUI_HPP
#define GUI_HPP

// GLAD
#include <glad/glad.h>

// NanoVG
#include "gui/nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION

// Inner classes
#include "log.hpp"
#include "statecon.hpp"

namespace app {

class Gui {
public:
	Gui();
	~Gui();
	
	// Enums
	enum SidesId {
		LEFT_SIDE,
		RIGHT_SIDE,
		TOP_SIDE,
		BOTTOM_SIDE,
		NUM_SIDES
	};
	
	// Variables
	int width;
	int height;
	
	
	// Functions
	bool initialize_gui();
	void configure_gui();
	bool draw_button(NVGcontext* vg, int preicon, const char* text, float pos_x, float pos_y, float width, float height, NVGcolor colour);
	bool draw_button_centered(NVGcontext* vg, int preicon, const char* text, float pos_y, float width, float height, NVGcolor colour); 
	void draw_plane(NVGcontext* vg, float pos_x, float pos_y, float width, float height, NVGcolor colour);
	void drawMainMenu();
	void drawOptionMenu();
	void drawCreditsMenu();
	void drawSideMenu();
	void cleanup();
	
	
	
private:
	// Variables
	static Log* logger;
	static StateCon* stater;
	NVGcontext* vg;
	bool sideMenu_hovered;
	bool sideMenu_focused;
	float sideMenu_time;
	
	// Functions
	int isBlack(NVGcolor col);
	char* cpToUTF8(int cp, char* str);
	
	
	
};

}

#endif // GUI_HPP
