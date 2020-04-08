#ifndef SHADER_HPP
#define SHADER_HPP

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
//#include <algorithm>
#include <sstream>
//
//#include <stdlib.h>
//#include <string.h>

// GLAD
#include "glad/glad.h"


namespace app
{

class Shader
{
public:
	// Functions
	static Shader* getInstance();
	static void releaseInstance();
	static void initialize();
	static GLuint getShaderID();
	static void loadShaders(const char * vertex_file_path,const char * fragment_file_path);
	
	// Variables
	
	
private:
	Shader();
	~Shader();
	
	// Functions
	
	
	// Variables
	static Shader* instance;
	static GLuint ProgramID;
	
};

}



#endif
