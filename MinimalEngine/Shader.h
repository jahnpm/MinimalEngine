#pragma once


#include <iostream>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:

	Shader(const char *vsSource, const char *fsSource);

	void setUniformBool(const char* name, bool value);
	void setUniformVec3(const char* name, glm::vec3 value);
	void setUniformVec4(const char* name, glm::vec4 value);
	void setUniformMat4(const char* name, glm::mat4 value);
	void setUniformTex(const char* name, unsigned int value);

	~Shader();

	unsigned int id;

private:

	char *readShaderSource(const char *sourceFile);
};

