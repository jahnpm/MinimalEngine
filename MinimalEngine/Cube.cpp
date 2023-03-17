#include "Cube.h"
#include "stb_image.h"

Cube::Cube(const char *textureFile)
{
	vertexCount = 36;

	buildMesh();
	updateTextureCoordinates();

	if (textureFile != NULL)
		texture = new Texture(textureFile);
	else
		textured = false;
}

Cube::Cube(const char* tex0, const char* tex1, const char* tex2)
{
	vertexCount = 36;
	texCoordsPerVertex = 3;

	buildMesh();
	updateTextureCoordinatesTriple();

	texture = new Texture(tex0);
	texture1 = new Texture(tex1);
	texture2 = new Texture(tex2);
}

void Cube::buildMesh()
{
	mesh = new float[3 * vertexCount]
	{
		// back
		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		// front
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,

		// left
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,

		// right
		0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,

		// bottom
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,

		// top
		-0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
	};

	fillVertexBuffer();
}

void Cube::updateTextureCoordinates()
{
	textureCoordinates = new float[2 * vertexCount]
	{
		0.0f, 0.0f,
		0.0f, textureRepeatCountT,
		textureRepeatCountS, textureRepeatCountT,
		textureRepeatCountS, textureRepeatCountT,
		textureRepeatCountS, 0.0f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		textureRepeatCountS, 0.0f,
		textureRepeatCountS, textureRepeatCountT,
		textureRepeatCountS, textureRepeatCountT,
		0.0f, textureRepeatCountT,
		0.0f, 0.0f,

		textureRepeatCountS, textureRepeatCountT,
		0.0f, textureRepeatCountT,
		0.0f, 0.0f,
		0.0f, 0.0f,
		textureRepeatCountS, 0.0f,
		textureRepeatCountS, textureRepeatCountT,

		textureRepeatCountS, textureRepeatCountT,
		textureRepeatCountS, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, textureRepeatCountT,
		textureRepeatCountS, textureRepeatCountT,

		0.0f, textureRepeatCountT,
		textureRepeatCountS, textureRepeatCountT,
		textureRepeatCountS, 0.0f,
		textureRepeatCountS, 0.0f,
		0.0f, 0.0f,
		0.0f, textureRepeatCountT,

		0.0f, textureRepeatCountT,
		textureRepeatCountS, textureRepeatCountT,
		textureRepeatCountS, 0.0f,
		textureRepeatCountS, 0.0f,
		0.0f, 0.0f,
		0.0f, textureRepeatCountT,
	};

	fillTextureBuffer();
}

void Cube::updateTextureCoordinatesTriple()
{
	textureCoordinates = new float[3 * vertexCount]
	{
		0.0f, 0.0f, 0.0f,
		0.0f, textureRepeatCountT, 0.0f,
		textureRepeatCountS, textureRepeatCountT, 0.0f,
		textureRepeatCountS, textureRepeatCountT, 0.0f,
		textureRepeatCountS, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		textureRepeatCountS, 0.0f, 0.0f,
		textureRepeatCountS, textureRepeatCountT, 0.0f,
		textureRepeatCountS, textureRepeatCountT, 0.0f,
		0.0f, textureRepeatCountT, 0.0f,
		0.0f, 0.0f, 0.0f,

		textureRepeatCountS, textureRepeatCountT, 0.0f,
		0.0f, textureRepeatCountT, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		textureRepeatCountS, 0.0f, 0.0f,
		textureRepeatCountS, textureRepeatCountT, 0.0f,

		textureRepeatCountS, textureRepeatCountT, 0.0f,
		textureRepeatCountS, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, textureRepeatCountT, 0.0f,
		textureRepeatCountS, textureRepeatCountT, 0.0f,

		0.0f, textureRepeatCountT, 2.0f,
		textureRepeatCountS, textureRepeatCountT, 2.0f,
		textureRepeatCountS, 0.0f, 2.0f,
		textureRepeatCountS, 0.0f, 2.0f,
		0.0f, 0.0f, 2.0f,
		0.0f, textureRepeatCountT, 2.0f,

		0.0f, textureRepeatCountT, 1.0f,
		textureRepeatCountS, textureRepeatCountT, 1.0f,
		textureRepeatCountS, 0.0f, 1.0f,
		textureRepeatCountS, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, textureRepeatCountT, 1.0f,
	};

	fillTextureBuffer();
}

void Cube::drawTripleTexture(Shader* shader)
{
	if (textured)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1->id);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture2->id);
	}

	WorldObject::draw(shader);
}

Cube::~Cube()
{

}
