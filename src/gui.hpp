#ifndef GUI_HPP
#define GUI_HPP

// NanoVG
#include "gui/nanovg.h"
#include "gui/nanovg_gl.h"

// Inner classes
#include "logger.hpp"

namespace app {

class Gui {
public:
	
	// Variables
	static Logger* logger;
	
	
	
	// Functions
	static Gui* get_instance();
	bool initialize_gui();
	void configure_gui();
	
	
private:
	Gui();
	~Gui();
	
	
	// Variables
	static Gui* instance;
	
	
	
	// Functions
	static void release_instance();
	

};

}

#endif // GUI_HPP
