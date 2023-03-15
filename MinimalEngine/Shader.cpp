#include "Shader.h"
#include <glad/glad.h>


Shader::Shader(const char *vsSource, const char *fsSource)
{
	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	char *vertexShaderSource = readShaderSource(vsSource);
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	char *fragShaderSource = readShaderSource(fsSource);
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);
	// check for linking errors
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	delete vertexShaderSource;
	delete fragShaderSource;
}

void Shader::setUniformBool(const char * name, bool value)
{
	int location = glGetUniformLocation(id, name);
	glUniform1i(location, value);
}

void Shader::setUniformVec3(const char * name, glm::vec3 value)
{
	int location = glGetUniformLocation(id, name);
	glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::setUniformVec4(const char * name, glm::vec4 value)
{
	int location = glGetUniformLocation(id, name);
	glUniform4fv(location, 1, glm::value_ptr(value));
}

void Shader::setUniformMat4(const char * name, glm::mat4 value)
{
	int location = glGetUniformLocation(id, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniformTex(const char * name, unsigned int value)
{
	glUseProgram(id);
	int location = glGetUniformLocation(id, name);
	glUniform1i(location, value);
}

char *Shader::readShaderSource(const char *sourceFile)
{
	std::string line;
	std::string source = "";

	std::ifstream file(sourceFile);

	while (std::getline(file, line)) {

		source += line + "\n";
	}

	file.close();

	unsigned int srcLength = source.length() + 1;
	char *src = new char[srcLength];
	for (unsigned int i = 0; i < srcLength; i++)
		src[i] = source.data()[i];

	return src;
}



Shader::~Shader()
{
}
