#pragma once

#include "WorldObject.h"

class Plane : public WorldObject
{

public:
	Plane(const char *textureFile);
	~Plane();

	void updateTextureCoordinates();

	glm::vec3 normal;
	glm::vec3 up;
	glm::vec3 right;
};

