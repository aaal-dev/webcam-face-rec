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

bool Log::initialize () {
	FILE* file = fopen (LOG_FILE, "w");
	if (!file) {
		fprintf (stderr, "ERROR: could not open LOG_FILE log file %s for writing\n", LOG_FILE);
		return false;
	};
	printTime (file);
	fprintf (file, "[INFO] ");
	fprintf (file, "LOG_FILE log started\n");
	fclose (file);
	return true;
}

bool Log::write (LogStages logStage, const char* message, ...) {
	va_list argptr;
	FILE* file = fopen (LOG_FILE, "a");
	if (!file) {
		fprintf(stderr, "ERROR: could not open LOG_FILE log file %s for appending\n", LOG_FILE);
		return false;
	};
	switch(logStage) {
		case LOG_INFO: {
			printTime (file);
			fprintf (file, "[INFO] ");
			break;
		}
		case LOG_WARN: {
			printTime (file);
			fprintf (file, "[WARN] ");
			break;
		}
		case LOG_ERROR: {
			printTime (file);
			fprintf (file, "[ERROR] ");
			break;
		}
		case LOG_DEBUG: {
			printTime (file);
			fprintf (file, "[DEBUG] ");
			break;
		}
		case LOG_MORE: {
			fprintf (file, "                   ");
			break;
		}
	}
	va_start (argptr, message);
	vfprintf (file, message, argptr);
	va_end (argptr);
	if (logStage == LOG_ERROR) {
		va_start (argptr, message);
		vfprintf (stderr, message, argptr);
		va_end (argptr);
	}
	fclose (file);
	return true;
}


}
