#pragma once

#include "Game.h"
#include "Cube.h"
#include "Shader.h"
#include "Camera.h"
#include "Plane.h"

struct PortalSurface
{
	Plane* plane;
	bool hasPortal;
};

class Portals : public Game
{
public:

	Portals();
	~Portals();

private:

	void init();
	void update();
	void draw();
	void drawPortalContents(int i, Plane* portal1, Plane* portal2, Camera* cam, int clip);
	void drawPortalOutline(Plane* p, Shader* s);

	void handleUserInput(GLFWwindow *window, float deltaTime);
	void handlePlayerMovement();
	void movePortal(Plane* portal);
	glm::quat getRotationQuat(glm::vec3 v1, glm::vec3 v2, glm::vec3 perp);

	glm::vec2 previousMouse;

	float previousTime;

	glm::mat4 view;
	glm::mat4 projection;

	Shader* shader;

	Camera* camera;
	Camera* cam1;
	Camera* cam2;

	Plane* portal1, * portal2;
	PortalSurface portalSurfaces[6];

	Cube* box, * tower, * player;

	int portalDepth;
};

