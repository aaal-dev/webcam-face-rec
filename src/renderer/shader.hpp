#ifndef SHADER_HPP
#define SHADER_HPP

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
//#include <algorithm>
#include <sstream>

// GLAD
#include "glad/glad.h"

// Inner classes
#include "../log.hpp"

namespace app {

class Shader {
public:
	Shader();
	Shader(const char * vertex_file_path,const char * fragment_file_path);
	~Shader();
	
	
	// Variables
	 
	
	// Functions
	static void initialize();
	bool loadShaders(const char * vertex_file_path,const char * fragment_file_path);
	GLuint getShaderID() const;
	
	
	
	
private:
	// Variables
	GLuint ProgramID;
	static Log* logger;
	
	
	// Functions
	
	
	
	
};


}

#endif
