#pragma once

#include "WorldObject.h"

class Cube :
	public WorldObject
{
public:

	Cube(const char *textureFile);
	Cube(const char *tex0, const char *tex1, const char *tex2);
	~Cube();

	void buildMesh();
	void updateTextureCoordinates();
	void updateTextureCoordinatesTriple();
	void drawTripleTexture(Shader* shader);

	Texture* texture1;
	Texture* texture2;
};

