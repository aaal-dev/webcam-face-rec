#include "shader.hpp"

namespace app {

Log* Shader::logger = nullptr;

Shader::Shader(){}
Shader::Shader(const char * vertex_file_path,const char * fragment_file_path)
{
	loadShaders(vertex_file_path, fragment_file_path);
}

Shader::~Shader(){}

GLuint Shader::getShaderID() const
{
	return ProgramID; 
}

bool Shader::loadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Work with Vertex Shader
	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	} else {
		logger->logGLError("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		return false;
	}
	
	// Compile Vertex Shader
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	logger->logGLInfo("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);
	
	// Check Vertex Shader
	GLint Result = GL_FALSE;
	int InfoLogLength;
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		logger->logGLError("%s\n", &VertexShaderErrorMessage[0]);
	}
	
	// Work with fragment shader
	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	} else {
		logger->logGLError("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", fragment_file_path);
		return false;
	}
	
	// Compile Fragment Shader
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	logger->logGLInfo("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		logger->logGLError("%s\n", &FragmentShaderErrorMessage[0]);
	}
	
	// Link the program
	logger->logGLInfo("Linking program\n");
	ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		logger->logGLError("%s\n", &ProgramErrorMessage[0]);
	}
	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	return true;
}


// Log functions
void Shader::logShaderInfo(GLuint shaderID) {
	int InfoLogLength;
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> shaderMessage(InfoLogLength+1);
		glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &shaderMessage[0]);
		logger->logGL("%s\n", &shaderMessage[0]);
	}
}

const char* Shader::GLtypeToString (GLenum type) {
	switch (type) {
		case GL_BOOL: return "bool";
		case GL_INT: return "int";
		case GL_FLOAT: return "float";
		case GL_FLOAT_VEC2: return "vec2";
		case GL_FLOAT_VEC3: return "vec3";
		case GL_FLOAT_VEC4: return "vec4";
		case GL_FLOAT_MAT2: return "mat2";
		case GL_FLOAT_MAT3: return "mat3";
		case GL_FLOAT_MAT4: return "mat4";
		case GL_SAMPLER_2D: return "sampler2D";
		case GL_SAMPLER_3D: return "sampler3D";
		case GL_SAMPLER_CUBE: return "samplerCube";
		case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
		default: break;
	}
	return "other";
}

void Shader::logAllShaderInfo (GLuint programID) {
	logger->logGLInfo("--------------------\nshader program %i info:\n", programID);
	int params = -1;
	glGetProgramiv (programID, GL_LINK_STATUS, &params);
	logger->logGL ("GL_LINK_STATUS = %i\n", params);
	
	glGetProgramiv (programID, GL_ATTACHED_SHADERS, &params);
	logger->logGL ("GL_ATTACHED_SHADERS = %i\n", params);
	
	glGetProgramiv (programID, GL_ACTIVE_ATTRIBUTES, &params);
	logger->logGL ("GL_ACTIVE_ATTRIBUTES = %i\n", params);
	for (GLuint i = 0; i < (GLuint)params; i++) {
		char name[64];
		int max_length = 64;
		int actual_length = 0;
		int size = 0;
		GLenum type;
		glGetActiveAttrib (programID, i, max_length, &actual_length, &size, &type, name);
		if (size > 1) {
			for (int j = 0; j < size; j++) {
				char long_name[64];
				sprintf (long_name, "%s[%i]", name, j);
				int location = glGetAttribLocation (programID, long_name);
				logger->logGL ("  %i) type:%s name:%s location:%i\n", i, GLtypeToString (type), long_name, location);
			}
		} else {
			int location = glGetAttribLocation (programID, name);
			logger->logGL ("  %i) type:%s name:%s location:%i\n", i, GLtypeToString (type), name, location);
		}
	}
	
	glGetProgramiv (programID, GL_ACTIVE_UNIFORMS, &params);
	logger->logGL ("GL_ACTIVE_UNIFORMS = %i\n", params);
	for (GLuint i = 0; i < (GLuint)params; i++) {
		char name[64];
		int max_length = 64;
		int actual_length = 0;
		int size = 0;
		GLenum type;
		glGetActiveUniform (programID, i, max_length, &actual_length, &size, &type, name);
		if (size > 1) {
			for (int j = 0; j < size; j++) {
				char long_name[64];
				sprintf (long_name, "%s[%i]", name, j);
				int location = glGetUniformLocation (programID, long_name);
				logger->logGL ("  %i) type:%s name:%s location:%i\n", i, GLtypeToString (type), long_name, location);
			}
		} else {
			int location = glGetUniformLocation (programID, name);
			logger->logGL ("  %i) type:%s name:%s location:%i\n", i, GLtypeToString (type), name, location);
		}
	}
	
	logShaderInfo (programID);
}

bool Shader::isValid (GLuint programID) {
	glValidateProgram (programID);
	int params = -1;
	glGetProgramiv (programID, GL_VALIDATE_STATUS, &params);
	logger->logGLInfo ("program %i GL_VALIDATE_STATUS = %i\n", programID, params);
	if (GL_TRUE != params) {
		logShaderInfo (programID);
		return false;
	}
	return true;
}

}
