#include "config.hpp"

namespace app {

Log* Config::logger = nullptr;

Config::Config () {}

Config::Config(const char* filename) {
	logger = Log::getLog ();
	parseFile(filename, 1, this);
}

Config::~Config () {}


void Config::parseFile(const char* filename, int handler, void* user) {
	FILE* file = fopen(filename, "r");
	if (!file) {
		logger->logGLWarning("Something wrong. There is no config file");
		return;
	}
	
	char* line = (char*)malloc(MAX_LINE_LENGTH);
	if (!line) {
		
		return;
	}
	
	char section[50] = "";
	char prev_name[50] = "";
	char* start;
	char* end;
	char* name;
	char* value;
	int lineNumber = 0;
	int error = 0;
	
	/* Scan through stream line by line */
	while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
		lineNumber++;
		start = line;
		
		if (lineNumber == 1 && 
			(unsigned char)start[0] == 0xEF &&
			(unsigned char)start[1] == 0xBB &&
			(unsigned char)start[2] == 0xBF) 
		{
			start += 3;
		}
		skipSpaceFromLeft(start);
		skipSpaceFromRight(start);
		
		if (*start == ';' || *start == '#') {
		} else if (*start == '[') {
			end = find_chars_or_comment(start + 1, "]");
			if (*end == ']') {
				*end = '\0';
				strncpy0(section, start + 1, sizeof(section));
				*prev_name = '\0';
			} else if (!error) {
				/* No ']' found on section line */
				logger->logGLWarning("Something wrong on line " + itoa(lineNumber) + " in config file");
				error++;
			}
		} else if (*start) {
			/* Not a comment, must be a name[=:]value pair */
			end = find_chars_or_comment(start, "=:");
			if (*end == '=' || *end == ':') {
				*end = '\0';
				name = rstrip(start);
				value = lskip(end + 1);
				end = find_chars_or_comment(value, NULL);
				if (*end)
					*end = '\0';
				rstrip(value);
				
				/* Valid name[=:]value pair found, call handler */
				strncpy0(prev_name, name, sizeof(prev_name));
				if (!handler(user, section, name, value) && !error)
					error = lineno;
			} else if (!error) {
				/* No '=' or ':' found on name[=:]value line */
				error = lineno;
			}
		}
		
		if (error > 0)
			break;
	}
	
	free(line);
	fclose(file);
}



void Config::skipSpaceFromLeft(const char* line) {
	while (*line && isspace((unsigned char)(*line)))
		line++;
}

void Config::skipSpaceFromRight(char* line) {
	char* end = line + strlen(line);
	while (end > line && isspace((unsigned char)(*--end)))
		*end = '\0';
}

char* find_chars_or_comment(const char* line, const char* chars) {
	int was_space = 0;
	while (*line && 
		(!chars || !strchr(chars, *line)) && 
		!(was_space && 
		strchr(INI_INLINE_COMMENT_PREFIXES, *line))) 
	{
		was_space = isspace((unsigned char)(*line));
		line++;
	}
	
	while (*line && (!chars || !strchr(chars, *line)))
		line++;
	
	return (char*)line;
}

char* strncpy0(char* dest, const char* src, size_t size) {
	strncpy(dest, src, size);
	dest[size - 1] = '\0';
	return dest;
}



inline int INIReader::ParseError() const
{
    return _error;
}

inline const std::set<std::string>& INIReader::Sections() const
{
    return _sections;
}

inline std::string INIReader::Get(std::string section, std::string name, std::string default_value) const
{
    std::string key = MakeKey(section, name);
    return _values.count(key) ? _values.at(key) : default_value;
}

inline long INIReader::GetInteger(std::string section, std::string name, long default_value) const
{
    std::string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    long n = strtol(value, &end, 0);
    return end > value ? n : default_value;
}

inline double INIReader::GetReal(std::string section, std::string name, double default_value) const
{
    std::string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    double n = strtod(value, &end);
    return end > value ? n : default_value;
}

inline float INIReader::GetFloat(std::string section, std::string name, float default_value) const
{
    std::string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    float n = strtof(value, &end);
    return end > value ? n : default_value;
}

inline bool INIReader::GetBoolean(std::string section, std::string name, bool default_value) const
{
    std::string valstr = Get(section, name, "");
    // Convert to lower case to make string comparisons case-insensitive
    std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
    if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
        return true;
    else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
        return false;
    else
        return default_value;
}

inline std::string INIReader::MakeKey(std::string section, std::string name)
{
    std::string key = section + "=" + name;
    // Convert to lower case to make section/name lookups case-insensitive
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    return key;
}

inline int Config::ValueHandler(void* user, const char* section, const char* name,
                            const char* value)
{
    INIReader* reader = (INIReader*)user;
    std::string key = MakeKey(section, name);
    if (reader->_values[key].size() > 0)
        reader->_values[key] += "\n";
    reader->_values[key] += value;
    reader->_sections.insert(section);
    return 1;
}


}
