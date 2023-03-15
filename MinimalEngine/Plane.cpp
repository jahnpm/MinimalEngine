#include "Plane.h"



Plane::Plane(const char *textureFile)
{
	normal = glm::vec3(0, 0, 1);
	up = glm::vec3(0, 1, 0);
	right = glm::vec3(1, 0, 0);

	vertexCount = 6;

	mesh = new float[3 * vertexCount]
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f
	};

	updateTextureCoordinates();

	fillVertexBuffer();
	fillTextureBuffer();

	texture = new Texture(textureFile);
}

void Plane::updateTextureCoordinates()
{
	textureCoordinates = new float[2 * vertexCount]
	{
		0.0f, 0.0f,
		textureRepeatCountS, 0.0f,
		0.0f, textureRepeatCountT,
		textureRepeatCountS, textureRepeatCountT,
		0.0f, textureRepeatCountT,
		textureRepeatCountS, 0.0f
	};

	fillTextureBuffer();
}

Plane::~Plane()
{
}
