#ifndef CONFIG_HPP
#define CONFIG_HPP

//#include "inih/INIReader.h"
#include <stdio.h>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <string.h>

// Inner classes
#include "log.hpp"

namespace app {

#define MAX_LINE_LENGTH 255

class Config {
public:
	Config();
	Config(const char* filename);
	~Config();
	
	struct Property {
		char* name;
		char* value;
	};
	
	struct Section {
		char* name;
		std::vector<Property> properties;
	};
	
	// Variables
	std::vector<Section> sections;

	// Functions
	int ParseError() const;
	const std::set<std::string>& Sections() const;
	std::string Get(std::string section, std::string name, std::string default_value) const;
	long GetInteger(std::string section, std::string name, long default_value) const;
	double GetReal(std::string section, std::string name, double default_value) const;
	float GetFloat(std::string section, std::string name, float default_value) const;
	bool GetBoolean(std::string section, std::string name, bool default_value) const;
	
	
protected:
	// Variables
	static Log* logger;
	
	int _error;
	std::map<std::string, std::string> _values;
	std::set<std::string> _sections;
	
	// Functions
	void parseFile(const char* filename);
	
	void skipSpaceFromLeft(char* s);
	void skipSpaceFromRight(char* s);
	char* goToChar(const char* chars, const char* line);
	
	static std::string MakeKey(std::string section, std::string name);
	static int ValueHandler(void* user, const char* section, const char* name, const char* value);
};

}

#endif // CONFIG_HPP
