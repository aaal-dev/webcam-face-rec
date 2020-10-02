#include "logger.hpp"

namespace app {

Logger* Logger::instance = nullptr;

Logger::Logger() {}
Logger::~Logger() {}

Logger* Logger::get_instance() {
	if (instance == nullptr)
		instance = new Logger();
	return instance;
}

void Logger::release_instance() {
	if (instance != nullptr)
		delete instance;
	instance = nullptr;
}

bool Logger::restart_gl_log() {
	FILE* file = fopen(GL_LOG_FILE, "w");
	if (!file) {
		fprintf(stderr, "ERROR: could not open GL_LOG_FILE log file %s for writing\n", GL_LOG_FILE);
		return false;
	};
	time_t now = time(NULL);
	char* date = ctime(&now);
	fprintf(file, "GL_LOG_FILE log. local time %s\n", date);
	fclose(file);
	return true;
}

bool Logger::gl_log(const char* message, ...) {
	va_list argptr;
	FILE* file = fopen(GL_LOG_FILE, "a");
	if (!file) {
		fprintf(stderr, "ERROR: could not open GL_LOG_FILE log file %s for appending\n", GL_LOG_FILE);
		return false;
	};
	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);
	fclose(file);
	return true;
}

bool Logger::gl_log_err(const char* message, ...) {
	va_list argptr;
	FILE* file = fopen(GL_LOG_FILE, "a");
	if (!file) {
		fprintf(stderr, "ERROR: could not open GL_LOG_FILE log file %s for appending\n", GL_LOG_FILE);
		return false;
	};
	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);
	va_start(argptr, message);
	vfprintf(stderr, message, argptr);
	va_end(argptr);
	fclose(file);
	return true;
}

}
