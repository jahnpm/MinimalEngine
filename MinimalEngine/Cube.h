#pragma once

#include "WorldObject.h"

class Cube :
	public WorldObject
{
public:

	Cube(const char *textureFile);
	Cube(const char *tex0, const char *tex1, const char *tex2);
	~Cube();

	void updateTextureCoordinatesTriple();

	void updateTextureCoordinates();
	void drawTripleTexture(Shader* shader);

	Texture* texture0;
	Texture* texture1;
	Texture* texture2;
};

