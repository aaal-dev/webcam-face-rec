#include "app.hpp"
int main () {
	setenv( "MESA_DEBUG", "", 0 );
	if (app::App::getApp ()->initialize () == GL_FALSE)
		return 1;
	app::App::getApp ()->run ();
	app::App::getApp ()->stop ();
	return 0;
}