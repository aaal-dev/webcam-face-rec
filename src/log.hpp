#ifndef LOG_HPP
#define LOG_HPP

#include <time.h>
#include <stdarg.h>
#include <cstdio>

#define LOG_FILE "gl.log"

namespace app {

class Log {
	
public:
	// Enums
	enum LogStages {
		LOG_INFO,
		LOG_WARN,
		LOG_ERROR,
		LOG_DEBUG,
		LOG_MORE,
		NUM_LOGSTATES
	};
	
	
	// Variables
	
	
	
	// Functions
	static Log* getLog ();
	static void releaseLog ();
	
	bool initialize ();
	bool write (LogStages logStage, const char* message, ...);
	
	
private:
	Log ();
	~Log ();
	
	
	// Variables
	static Log* log;
	
	
	// Functions
	void printTime (FILE* file); 
	
	
	
};

}

#endif // LOG_HPP
