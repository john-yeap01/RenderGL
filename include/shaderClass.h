#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ID;
	// Constructor that build the Shader Program from 2 different shaders
	Shader(const char* vertexFile, const char* fragmentFile);

	// Activates the Shader Program
	void Activate();

	void SetFloat(const char* name, const float f);

	// Wrapper to set the uniforms of the active shader
	void SetVec3(const char* name, const glm::vec3& v);

	void SetMat4(const char* name, const glm::mat4& m);

	// Deletes the Shader Program
	void Delete();

	
private:
	void compileErrors(unsigned int shader, const char* type);
};
#endif