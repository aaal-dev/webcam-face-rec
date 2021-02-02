#ifndef CONFIG_HPP
#define CONFIG_HPP

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
	
	enum ValueType { STRING_VAL, INT_VAL, DOUBLE_VAL, BOOL_VAL };
	
	struct Property {
		const char* name;
		ValueType type;
		union Value {
			const char* stringVal;
			int intVal;
			double doubleVal;
			bool boolVal;
			
			Value() = default;
			Value(const char* s): stringVal(s) {};
			Value(int i): intVal(i) {};
			Value(double d): doubleVal(d) {};
			Value(bool b): boolVal(b) {};
		} value;
		
		
		Property(const char* n = "", ValueType t = STRING_VAL, const char* v = ""): name(n), type(t), value(v) {};
		Property(const char* n = "", ValueType t = INT_VAL, int v = 0): name(n), type(t), value(v) {};
		Property(const char* n = "", ValueType t = DOUBLE_VAL, double v = 0.0): name(n), type(t), value(v) {};
		Property(const char* n = "", ValueType t = BOOL_VAL, bool v = false): name(n), type(t), value(v) {};
		Property() = default;
		
		bool operator<(const Property& rhs) const {
			return name < rhs.name;
		}
		
		bool operator==(const Property& rhs) const {
			return name == rhs.name;
		}
	};
	
	struct Section {
		const char* name;
		std::set<Property*> properties;
		
		Section(const char* n = ""): name(n) {};		
		
		bool operator<(const Section& rhs) const {
			return name < rhs.name;
		}
		
		bool operator==(const Section& rhs) const {
			return name == rhs.name;
		}
	};
	
	// Variables
	std::set<Section*> sections;

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
	
	void skipSpaceFromLeft(const char* s);
	void skipSpaceFromRight(const char* s);
	char* goToChar(const char* chars, const char* line);
	
	static std::string MakeKey(std::string section, std::string name);
	static int ValueHandler(void* user, const char* section, const char* name, const char* value);
};

}

#endif // CONFIG_HPP
