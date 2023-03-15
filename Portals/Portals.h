#pragma once

#include "Game.h"
#include "Cube.h"
#include "Shader.h"
#include "Camera.h"
#include "Plane.h"

class Portals : public Game
{
public:

	Portals();
	~Portals();

private:

	void init();
	void update();
	void draw();
	void handleUserInput(GLFWwindow *window, float deltaTime);
	glm::quat getRotationQuat(glm::vec3 v1, glm::vec3 v2, glm::vec3 perp);

	glm::vec2 previousMouse;

	float previousTime;

	glm::mat4 view;
	glm::mat4 projection;

	Shader *shader;
	Cube *box, *tower, *player;
	Camera *camera;

	Plane *portal1, *portal2;
	Plane *wallNorth, *wallEast, *wallSouth, *wallWest;
	Plane *floor, *ceiling;

	glm::vec3 fpa;
	glm::vec3 fpb;
	glm::vec3 fpc;
	glm::vec3 spa;
	glm::vec3 spb;
	glm::vec3 spc;

	int portalDepth;
};

