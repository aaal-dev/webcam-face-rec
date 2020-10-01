#include "app.hpp"
int main() {
	if (app::App::get_instance()->initialize() == GL_FALSE)
		return 1;
	app::App::get_instance()->run();
	app::App::get_instance()->stop();
	return 0;
}