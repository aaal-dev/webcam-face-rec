#include "render.hpp"

namespace app
{

const char* SHADER_PATH = "../../data/shaders/";
Log* Render::logger = nullptr;
StateCon* Render::stater = nullptr;

Render::Render () {
	logger = Log::getLog ();
	stater = StateCon::getStateCon ();
	width = stater->framebuffer_width;
	height = stater->framebuffer_height;
}

Render::~Render () {}

bool Render::initialize_GL(GLADloadproc glfwProcAddress) {
	if (!gladLoadGLLoader(glfwProcAddress)) {
		logger->logGLError("Failed to initialize OpenGL context.\n");
		return false;
	}
	
	logger->logGLInfo("Renderer: %s\n", glGetString (GL_RENDERER));
	logger->logGLInfo("OpenGL version supported %s\n", glGetString (GL_VERSION));
	
	logGLparams ();
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glViewport(0, 0, width, height);
	
	// Create and compile our GLSL program from the shaders
	if (!setWebcamMesh()) {
		logger->logGLError("Failed to set mesh for webcam.\n");
		return false;
	}
	logger->logGL("Set mesh for webcam.\n");
	
	Mesh headModel = setHeadModelMesh();
	glm::mat4 normalization(1.0);
	normalization = glm::scale(normalization, glm::vec3(1.2, 1.2, 1.2));
	normalization = glm::translate(normalization, glm::vec3(0, 5.0, 0));
	normalization = glm::rotate(normalization, 1.55f, glm::vec3(0.0f, 1.0f, 0.0f));
	headModel.normalization = normalization;
	//meshes.push_back(headModel);
	
	return true;
}

bool Render::setWebcamMesh() {
	webcam.name = "webcam";
	if (!webcam.load_shader( "../../data/shaders/webcam.vert", "../../data/shaders/webcam.frag" )) {
		return false;
	}
	
	webcam.vertices = webcam.CreateQuad(-1.0f, 1.0f, 0.0f, 2.0f, 0.0f);
	
	glGenBuffers(1, &webcam.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, webcam.VBO);
	glBufferData(GL_ARRAY_BUFFER, webcam.vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	
	glGenVertexArrays(1, &webcam.VAO);
	glBindVertexArray(webcam.VAO);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, TexCoords)));
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, TexId)));
	
	// Set up index
	webcam.indices = { 0, 1, 2, 2, 3, 0 };
	
	glGenBuffers(1, &webcam.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, webcam.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(webcam.indices), &webcam.indices.front(), GL_STATIC_DRAW);
	
	glGenTextures(1, &webcam.texture);
	glBindTexture(GL_TEXTURE_2D, webcam.texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	webcam.isDrawing = true;
	return true;
}

Mesh Render::setHeadModelMesh() {
	Mesh headModel;
	headModel.name = "head";
	headModel.load_model("../../data/model/cartoon/head.obj");
	//headModel.load_model("../../data/model/female/female.obj");
	headModel.load_shader( "../../data/shaders/shader2.vert", "../../data/shaders/shader2.frag" );
	
	glUseProgram(headModel.shaderID);
	
	GLuint l_positionMatrixID = glGetUniformLocation(headModel.shaderID, "l_position");
	glm::vec3 l_position = glm::vec3(10.0f, 15.0f, 10.0f);
	glUniform3fv(l_positionMatrixID, 1, &l_position[0]);
	
	GLuint l_colorMatrixID = glGetUniformLocation(headModel.shaderID, "l_color");
	glm::vec3 l_color = glm::vec3(1.0f, 1.0f, 1.0f);
	glUniform3fv(l_colorMatrixID, 1, &l_color[0]);
	
	GLuint colorMatrixID = glGetUniformLocation(headModel.shaderID, "color");
	glm::vec3 m_color = headModel.base_color;
	glUniform3fv(colorMatrixID, 1, &m_color[0]);
	
	glGenBuffers(1, &headModel.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, headModel.VBO);
	glBufferData(GL_ARRAY_BUFFER, headModel.vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	
	glGenVertexArrays(1, &headModel.VAO);
	glBindVertexArray(headModel.VAO);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, Normal)));
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, TexCoords)));
	
	// Set up index
	glGenBuffers(1, &headModel.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, headModel.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, headModel.indices.size() * sizeof(unsigned int), &headModel.indices.front(), GL_STATIC_DRAW);
	
	return headModel;
}

void Render::setHeadModelTransformation(glm::mat4 transformation) {
	if (meshes.size() > 0)
		meshes[meshes.size()-1].transformation = transformation;
}

void Render::drawCleanUp() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	glClearColor(0.0f, 0.2f, 0.0f, 0.0f);
	glViewport(0, 0, width, height);
}

void Render::drawWebCamMesh() {
	if (webcam.isDrawing)	{
		glUseProgram(webcam.shaderID);
		
		glBindTexture(GL_TEXTURE_2D, webcam.texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cameraFrame.width, cameraFrame.height, 0, GL_RGB, GL_UNSIGNED_BYTE, cameraFrame.data);
		
		glBindBuffer(GL_ARRAY_BUFFER, webcam.VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, webcam.vertices.size() * sizeof(Vertex), &webcam.vertices.front());
		
		glBindVertexArray(webcam.VAO);
		glDrawElements(GL_TRIANGLES, webcam.vertices.size() * sizeof(Vertex), GL_UNSIGNED_INT, 0);
	}
}

void Render::draw() {
	for (const auto &mesh : meshes) {
		if (mesh.isDrawing) {
			glUseProgram(mesh.shaderID);
			
			float aspect_ratio = (float)width / (float)height;
			
			GLuint modelMatrixID = glGetUniformLocation(mesh.shaderID, "model");
			glm::mat4 mesh_transform = mesh.normalization * mesh.transformation;
			glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &mesh_transform[0][0]);
			
			GLuint viewMatrixID = glGetUniformLocation(mesh.shaderID, "view");
			glm::mat4 view = glm::lookAt(glm::vec3(15.0, 5.0, 0.0), glm::vec3(0.0, 5.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
			glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &view[0][0]);
			
			GLuint projectionMatrixID = glGetUniformLocation(mesh.shaderID, "projection");
			glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.5f, 100.0f);
			glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, &projection[0][0]);
			
			glBindTexture(GL_TEXTURE_2D, mesh.texture);
			if (mesh.name == "webcam")
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cameraFrame.width, cameraFrame.height, 0, GL_RGB, GL_UNSIGNED_BYTE, cameraFrame.data);
			if (mesh.name == "head")
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			
			glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices.front());
			
			glBindVertexArray(mesh.VAO);
			glDrawElements(GL_TRIANGLES, mesh.vertices.size() * sizeof(Vertex), GL_UNSIGNED_INT, 0);
		}
	}
}

void Render::cleanup() {
	// Cleanup VBO
	for (const auto &mesh : meshes) {
		glDeleteBuffers(1, &mesh.VBO);
		glDeleteBuffers(1, &mesh.EBO);
		glDeleteVertexArrays(1, &mesh.VAO);
		glDeleteTextures(1, &mesh.texture);
		glDeleteProgram(mesh.shaderID);
	}

}

void Render::changeViewport(int bX, int bY, int eX, int eY) {
	glViewport(bX, bY, eX, eY);
}

void Render::setDrawProperties(std::map<std::string, bool> boolFromGui) {
	for (Mesh &mesh : meshes) 	{
		if (mesh.name == "webcam")
			mesh.isDrawing = boolFromGui.at(mesh.name);	
		if (mesh.name == "head")
			mesh.isDrawing = boolFromGui.at(mesh.name);
	}
}

// Log functions
void Render::logGLparams () {
	 GLenum params[] = {
		 GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		 GL_MAX_CUBE_MAP_TEXTURE_SIZE,
		 GL_MAX_DRAW_BUFFERS,
		 GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
		 GL_MAX_TEXTURE_IMAGE_UNITS,
		 GL_MAX_TEXTURE_SIZE,
		 GL_MAX_VARYING_FLOATS,
		 GL_MAX_VERTEX_ATTRIBS,
		 GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		 GL_MAX_VERTEX_UNIFORM_COMPONENTS,
		 GL_MAX_VIEWPORT_DIMS,
		 GL_STEREO,
	};
	const char* names[] = {
		"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
		"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
		"GL_MAX_DRAW_BUFFERS",
		"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
		"GL_MAX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_TEXTURE_SIZE",
		"GL_MAX_VARYING_FLOATS",
		"GL_MAX_VERTEX_ATTRIBS",
		"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
		"GL_MAX_VIEWPORT_DIMS",
		"GL_STEREO",
	};
	
	logger->logGLInfo ("GL Context Params:\n");
	//char msg[256];
	
	// integers - only works if the order is 0-10 integer return types
	for (int i = 0; i < 10; i++) {
		int v = 0;
		glGetIntegerv (params[i], &v);
		logger->logGL ("%s %i\n", names[i], v);
	}
	
	// others
	int v[2];
	v[0] = v[1] = 0;
	glGetIntegerv (params[10], v);
	logger->logGL ("%s %i %i\n", names[10], v[0], v[1]);
	unsigned char s = 0;
	glGetBooleanv (params[11], &s);
	logger->logGL ("%s %u\n", names[11], (unsigned int) s);
	logger->logGL ("-----------------------------\n");
}

}