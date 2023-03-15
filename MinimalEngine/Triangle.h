#pragma once

#include "WorldObject.h"

class Triangle : public WorldObject
{
public:
	Triangle(const char *textureFile);
	~Triangle();

	void updateTextureCoordinates();
};

