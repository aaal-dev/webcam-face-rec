#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <time.h>
#include <stdarg.h>
#include <cstdio>

#define GL_LOG_FILE "gl.log"

namespace app {

class Logger {
	
public:
	// Variables
	static Logger* instance;
	
	
	// Functions
	static Logger* get_instance();
	
	
	bool restart_gl_log();
	bool gl_log(const char* message, ...);
	bool gl_log_err(const char* message, ...); 
	
	
private:
	Logger();
	~Logger();
	
	
	
	// Variables
	
	
	
	// Functions
	static void release_instance();
	
	
	
	
};

}

#endif // LOGGER_HPP
