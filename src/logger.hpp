#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <cstdio>

#include <time.h>
#include <stdarg.h>

#define GL_LOG_FILE "gl.log"

namespace app {

class Logger {
	
public:
	// Variables
	static Logger* instance;
	
	
	// Functions
	static Logger* getInstance();
	static void releaseInstance();
	
	bool restart_gl_log();
	bool gl_log(const char* message, ...);
	bool gl_log_err(const char* message, ...); 
	
	
private:
	Logger();
	~Logger();
};

}

#endif // LOGGER_HPP
