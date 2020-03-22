
#include "app.hpp"

//#define PI 3.141592654

int main() 
{
	if (app::App::getInstance()->initialize() == GL_FALSE)
		return 1;
	app::App::getInstance()->run();
	return 0;
}