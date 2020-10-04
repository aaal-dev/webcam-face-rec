#include "log.hpp"

namespace app {

Log* Log::log = nullptr;

Log::Log () {}
Log::~Log () {}

Log* Log::getLog () {
	if (log == nullptr)
		log = new Log();
	return log;
}

void Log::releaseLog () {
	if (log != nullptr)
		delete log;
	log = nullptr;
}

void Log::printTime (FILE* file) {
	time_t now = time (NULL);
	tm lt = *localtime (&now);
	fprintf (file, "%d/%d/%d %d:%d:%d ", lt.tm_mday, 1 + lt.tm_mon, 1900 + lt.tm_year, 1 + lt.tm_hour, 1 + lt.tm_min, 1 + lt.tm_sec);
}

bool Log::restartGLlog () {
	FILE* file = fopen (GL_LOG_FILE, "w");
	if (!file) {
		fprintf (stderr, "ERROR: could not open GL_LOG_FILE log file %s for writing\n", GL_LOG_FILE);
		return false;
	};
	printTime (file);
	fprintf (file, "[INFO] ");
	fprintf (file, "GL_LOG_FILE log started\n");
	fclose (file);
	return true;
}

bool Log::logGL (const char* message, ...) {
	va_list argptr;
	FILE* file = fopen (GL_LOG_FILE, "a");
	if (!file) {
		fprintf(stderr, "ERROR: could not open GL_LOG_FILE log file %s for appending\n", GL_LOG_FILE);
		return false;
	};
	fprintf (file, "                   ");
	va_start (argptr, message);
	vfprintf (file, message, argptr);
	va_end (argptr);
	fclose (file);
	return true;
}

bool Log::logGLInfo (const char* message, ...) {
	va_list argptr;
	FILE* file = fopen (GL_LOG_FILE, "a");
	if (!file) {
		fprintf(stderr, "ERROR: could not open GL_LOG_FILE log file %s for appending\n", GL_LOG_FILE);
		return false;
	};
	printTime (file);
	fprintf (file, "[INFO] ");
	va_start (argptr, message);
	vfprintf (file, message, argptr);
	va_end (argptr);
	fclose (file);
	return true;
}

bool Log::logGLWarning (const char* message, ...) {
	va_list argptr;
	FILE* file = fopen (GL_LOG_FILE, "a");
	if (!file) {
		fprintf (stderr, "ERROR: could not open GL_LOG_FILE log file %s for appending\n", GL_LOG_FILE);
		return false;
	};
	printTime (file);
	fprintf (file, "[WARN] ");
	va_start (argptr, message);
	vfprintf (file, message, argptr);
	va_end (argptr);
	fclose (file);
	return true;
}

bool Log::logGLError (const char* message, ...) {
	va_list argptr;
	FILE* file = fopen (GL_LOG_FILE, "a");
	if (!file) {
		fprintf (stderr, "ERROR: could not open GL_LOG_FILE log file %s for appending\n", GL_LOG_FILE);
		return false;
	};
	printTime (file);
	fprintf (file, "[ERROR] ");
	va_start (argptr, message);
	vfprintf (file, message, argptr);
	va_end (argptr);
	va_start (argptr, message);
	vfprintf (stderr, message, argptr);
	va_end (argptr);
	fclose (file);
	return true;
}

}
