#include "app.hpp"
int main() 
{
	if (app::App::getInstance()->initialize() == GL_FALSE)
		return 1;
	app::App::getInstance()->run();
	app::App::getInstance()->stop();
	return 0;
}