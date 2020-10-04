#ifndef LOG_HPP
#define LOG_HPP

#include <time.h>
#include <stdarg.h>
#include <cstdio>

#define GL_LOG_FILE "gl.log"

namespace app {

class Log {
	
public:
	// Variables
	static Log* log;
	
	
	// Functions
	static Log* getLog ();
	
	
	bool restartGLlog ();
	bool logGL (const char* message, ...);
	bool logGLInfo (const char* message, ...);
	bool logGLWarning (const char* message, ...);
	bool logGLError (const char* message, ...); 
	
	
private:
	Log ();
	~Log ();
	
	
	
	// Variables
	
	
	
	// Functions
	static void releaseLog ();
	void printTime (FILE* file); 
	
	
	
};

}

#endif // LOG_HPP
