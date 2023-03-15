#include "Triangle.h"


Triangle::Triangle(const char *textureFile)
{
	vertexCount = 3;

	mesh = new float[3 * vertexCount]
	{
		-0.5f, -glm::root_three<float>() / 4, 0.0f,
		0.5f, -glm::root_three<float>() / 4, 0.0f,
		0.0f,  glm::root_three<float>() / 4, 0.0f,
	};

	updateTextureCoordinates();

	fillVertexBuffer();
	fillTextureBuffer();

	texture = new Texture(textureFile);
}

void Triangle::updateTextureCoordinates()
{
	textureCoordinates = new float[2 * vertexCount]
	{
		0.0f, 0.0f,
		textureRepeatCountS, 0.0f,
		0.5f, 1.0f,
	};

	fillTextureBuffer();
}

Triangle::~Triangle()
{
}
