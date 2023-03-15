#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Shader.h"
#include "Texture.h"

class WorldObject
{
public:

	WorldObject();
	~WorldObject();

	void fillVertexBuffer();
	void fillTextureBuffer();
	void draw(Shader *shader);

	unsigned int vertexCount;

	float *mesh;
	float *normals;
	float *textureCoordinates;

	unsigned int vertexArray;
	unsigned int vertexBuffer;
	unsigned int normalBuffer;
	unsigned int textureBuffer;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::quat rotationQuaternion;
	glm::vec3 scale;
	glm::vec4 color;

	bool textured;
	int texCoordsPerVertex;

	Texture *texture;

	float textureRepeatCountS;
	float textureRepeatCountT;
};
