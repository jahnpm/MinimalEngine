#include "Portals.h"


Portals::Portals()
{
	screenWidth = 1920;
	screenHeight = 1080;
	fullScreen = false;
	vsync = 0;
}

void Portals::init()
{
	glEnable(GL_STENCIL_TEST);

	shader = new Shader("portalsvs.glsl", "portalsfs.glsl");
	glUseProgram(shader->id);

	portalDepth = 16;

	camera = new Camera(screenWidth, screenHeight);
	camera->position = glm::vec3(0, 0, 0);
	camera->direction = glm::normalize(glm::vec3(0, 0, -1));
	camera->nearClip = 0.0001f;
	camera->farClip = 170.0f;

	cam1 = new Camera(screenWidth, screenHeight);
	cam2 = new Camera(screenWidth, screenHeight);

	projection = glm::perspective(camera->fov, camera->aspectRatio, camera->nearClip, camera->farClip);
	shader->setUniformMat4("projection", projection);

	portal1 = new Plane(NULL);
	portal1->textured = false;
	portal1->color = glm::vec4(0.5f, 0.5f, 1, 1);
	portal1->normal = glm::vec3(0, 0, 1);
	portal1->position = glm::vec3(0, 0, -5);
	portal1->scale = glm::vec3(2, 3, 0);
	portal1->up = glm::vec3(0, 1, 0);
	portal1->right = glm::vec3(1, 0, 0);
	portal1->rotationQuaternion = glm::mat4_cast(
		getRotationQuat(portal1->normal, glm::vec3(0, 0, 1), portal1->up));

	portal2 = new Plane(NULL);
	portal2->textured = false;
	portal2->color = glm::vec4(1, 0.65f, 0, 1);
	portal2->normal = glm::vec3(-1, 0, 0);
	portal2->position = glm::vec3(7.5f, 0, 0);
	portal2->scale = glm::vec3(2, 3, 0);
	portal2->up = glm::vec3(0, 1, 0);
	portal2->right = glm::vec3(0, 0, 1);
	portal2->rotationQuaternion = glm::mat4_cast(
		getRotationQuat(portal2->normal, glm::vec3(0, 0, 1), portal2->up));

	portalSurfaces[0].plane = new Plane("wallTexture.jpg");
	portalSurfaces[0].plane->texture->setWrapS(GL_MIRRORED_REPEAT);
	portalSurfaces[0].plane->textureRepeatCountS = 3;
	portalSurfaces[0].plane->updateTextureCoordinates();
	portalSurfaces[0].plane->position = glm::vec3(0, 0, -5);
	portalSurfaces[0].plane->normal = glm::vec3(0, 0, 1);
	portalSurfaces[0].plane->up = glm::vec3(0, 1, 0);
	portalSurfaces[0].plane->scale = glm::vec3(15, 5, 1);
	portalSurfaces[0].hasPortal = false;

	portalSurfaces[1].plane = new Plane("wallTexture.jpg");
	portalSurfaces[1].plane->texture->setWrapS(GL_MIRRORED_REPEAT);
	portalSurfaces[1].plane->textureRepeatCountS = 3;
	portalSurfaces[1].plane->updateTextureCoordinates();
	portalSurfaces[1].plane->position = glm::vec3(7.5f, 0, 0);
	portalSurfaces[1].plane->normal = glm::vec3(-1, 0, 0);
	portalSurfaces[1].plane->up = glm::vec3(0, 1, 0);
	portalSurfaces[1].plane->rotation.y = -90;
	portalSurfaces[1].plane->scale = glm::vec3(10, 5, 1);
	portalSurfaces[1].hasPortal = false;

	portalSurfaces[2].plane = new Plane("wallTexture.jpg");
	portalSurfaces[2].plane->texture->setWrapS(GL_MIRRORED_REPEAT);
	portalSurfaces[2].plane->textureRepeatCountS = 3;
	portalSurfaces[2].plane->updateTextureCoordinates();
	portalSurfaces[2].plane->position = glm::vec3(0, 0, 5);
	portalSurfaces[2].plane->normal = glm::vec3(0, 0, -1);
	portalSurfaces[2].plane->up = glm::vec3(0, 1, 0);
	portalSurfaces[2].plane->rotation.y = 180;
	portalSurfaces[2].plane->scale = glm::vec3(15, 5, 1);
	portalSurfaces[2].hasPortal = false;

	portalSurfaces[3].plane = new Plane("wallTexture.jpg");
	portalSurfaces[3].plane->texture->setWrapS(GL_MIRRORED_REPEAT);
	portalSurfaces[3].plane->textureRepeatCountS = 3;
	portalSurfaces[3].plane->updateTextureCoordinates();
	portalSurfaces[3].plane->position = glm::vec3(-7.5f, 0, 0);
	portalSurfaces[3].plane->normal = glm::vec3(1, 0, 0);
	portalSurfaces[3].plane->up = glm::vec3(0, 1, 0);
	portalSurfaces[3].plane->rotation.y = 90;
	portalSurfaces[3].plane->scale = glm::vec3(10, 5, 1);
	portalSurfaces[3].hasPortal = false;

	portalSurfaces[4].plane = new Plane("floorTexture.jpg");
	portalSurfaces[4].plane->texture->setWrapS(GL_MIRRORED_REPEAT);
	portalSurfaces[4].plane->texture->setWrapT(GL_MIRRORED_REPEAT);
	portalSurfaces[4].plane->textureRepeatCountS = 4;
	portalSurfaces[4].plane->textureRepeatCountT = 4;
	portalSurfaces[4].plane->updateTextureCoordinates();
	portalSurfaces[4].plane->position = glm::vec3(0, -2.5f, 0);
	portalSurfaces[4].plane->normal = glm::vec3(0, 1, 0);
	portalSurfaces[4].plane->up = glm::vec3(0, 0, 1);
	portalSurfaces[4].plane->rotation.x = -90;
	portalSurfaces[4].plane->scale = glm::vec3(15, 10, 1);
	portalSurfaces[4].hasPortal = false;

	portalSurfaces[5].plane = new Plane("ceilingTexture.jpg");
	portalSurfaces[5].plane->texture->setWrapS(GL_MIRRORED_REPEAT);
	portalSurfaces[5].plane->texture->setWrapT(GL_MIRRORED_REPEAT);
	portalSurfaces[5].plane->textureRepeatCountS = 3;
	portalSurfaces[5].plane->textureRepeatCountT = 3;
	portalSurfaces[5].plane->updateTextureCoordinates();
	portalSurfaces[5].plane->position = glm::vec3(0, 2.5f, 0);
	portalSurfaces[5].plane->normal = glm::vec3(0, -1, 0);
	portalSurfaces[5].plane->up = glm::vec3(0, 0, -1);
	portalSurfaces[5].plane->rotation.x = 90;
	portalSurfaces[5].plane->scale = glm::vec3(15, 10, 1);
	portalSurfaces[5].hasPortal = false;

	box = new Cube("objectTexture.png");
	box->position = glm::vec3(-3, -2, -2);
	box->rotation.y = 45;

	tower = new Cube("objectTexture.png");
	tower->texture->setWrapS(GL_REPEAT);
	tower->texture->setWrapT(GL_REPEAT);
	tower->textureRepeatCountS = 1;
	tower->textureRepeatCountT = 4;
	tower->updateTextureCoordinates();
	tower->position = glm::vec3(4.5f, -1.5, 2);
	tower->scale = glm::vec3(0.5f, 2, 0.5f);

	player = new Cube(NULL);
	player->position = camera->position;
	player->scale = glm::vec3(0.5f, 0.5f, 0.5f);

	shader->setUniformVec3("portal1Pos", portal1->position);
	shader->setUniformVec3("portal2Pos", portal2->position);
	shader->setUniformVec3("portal1Norm", portal1->normal);
	shader->setUniformVec3("portal2Norm", portal2->normal);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	previousMouse = glm::vec2((float)mouseX, (float)mouseY);

	previousTime = glfwGetTime();
}

void Portals::update()
{
	float currentTime = glfwGetTime();
	float deltaTime = currentTime - previousTime;
	previousTime = currentTime;

	handleUserInput(window, deltaTime);

	player->position = camera->position;
}

void Portals::handleUserInput(GLFWwindow *window, float deltaTime)
{
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	glm::vec2 currentMouse = glm::vec2((float)mouseX, (float)mouseY);
	glm::vec2 mouseOffset = currentMouse - previousMouse;
	previousMouse = currentMouse;

	camera->updateFlying(window, deltaTime, mouseOffset);

	handlePlayerMovement();

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		movePortal(portal1);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		movePortal(portal2);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Portals::handlePlayerMovement()
{
	glm::vec3 camToFirstPort = camera->position - portal1->position;
	float firstAngle = glm::dot(camToFirstPort, portal1->normal);

	glm::vec3 camToSecPort = camera->position - portal2->position;
	float secondAngle = glm::dot(camToSecPort, portal2->normal);

	if (glm::length(camToFirstPort) > 1 && glm::length(camToSecPort) > 1)
	{
		/* Keep PLayer inside the box if not close to either portal */

		if (camera->position.x > 7)
			camera->position.x = 7;
		if (camera->position.x < -7)
			camera->position.x = -7;
		if (camera->position.y > 2)
			camera->position.y = 2;
		if (camera->position.y < -2)
			camera->position.y = -2;
		if (camera->position.z > 4.5f)
			camera->position.z = 4.5f;
		if (camera->position.z < -4.5f)
			camera->position.z = -4.5f;
	}
	else
	{
		/* Teleport player and adjust look direction when they walk through a portal */

		if (firstAngle < 0 && glm::length(camToFirstPort) <= 1)
		{
			glm::vec3 transformedPos1 = portal1->rotationQuaternion * glm::vec4(camera->position - portal1->position, 0);
			camera->position = portal2->position - transformedPos1.x * portal2->right + transformedPos1.y * portal2->up - transformedPos1.z * portal2->normal;
			glm::vec3 transformedDir1 = portal1->rotationQuaternion * glm::vec4(camera->direction, 0);
			camera->direction = -transformedDir1.x * portal2->right + transformedDir1.y * portal2->up - transformedDir1.z * portal2->normal;

			camera->right = glm::normalize(glm::cross(camera->direction, camera->up));
		}
		else if (secondAngle < 0 && glm::length(camToSecPort) <= 1)
		{
			glm::vec3 transformedPos2 = portal2->rotationQuaternion * glm::vec4(camera->position - portal2->position, 0);
			camera->position = portal1->position - transformedPos2.x * portal1->right + transformedPos2.y * portal1->up - transformedPos2.z * portal1->normal;
			glm::vec3 transformedDir2 = portal2->rotationQuaternion * glm::vec4(camera->direction, 0);
			camera->direction = -transformedDir2.x * portal1->right + transformedDir2.y * portal1->up - transformedDir2.z * portal1->normal;

			camera->right = glm::normalize(glm::cross(camera->direction, camera->up));
		}
	}
}

void Portals::movePortal(Plane* portal)
{
	/* Move portal to the position on the wall the player is looking at*/

	for (PortalSurface wall : portalSurfaces)
	{
		wall.hasPortal = false;

		float nl = glm::dot(wall.plane->normal, camera->direction);

		if (nl != 0)
		{
			float d = glm::dot(wall.plane->position - camera->position, wall.plane->normal) / nl;
			glm::vec3 x = d * camera->direction + camera->position;

			if (x.x >= -7.5f && x.x <= 7.5f
				&& x.y >= -2.5f && x.y <= 2.5f
				&& x.z >= -5 && x.z <= 5
				&& d >= 0)
			{
				portal->position = x;
				portal->normal = wall.plane->normal;
				portal->up = wall.plane->up;
				portal->right = -glm::normalize(glm::cross(portal->normal, portal->up));
				portal->rotationQuaternion = glm::mat4_cast(
					getRotationQuat(portal->normal, glm::vec3(0, 0, 1), portal->up));

				wall.hasPortal = true;
			}
		}
	}

	shader->setUniformVec3("portal1Pos", portal1->position);
	shader->setUniformVec3("portal1Norm", portal1->normal);
	shader->setUniformVec3("portal2Pos", portal2->position);
	shader->setUniformVec3("portal2Norm", portal2->normal);
}

glm::quat Portals::getRotationQuat(glm::vec3 v1, glm::vec3 v2, glm::vec3 perp)
{
	glm::quat rotQuat;

	glm::vec3 n1 = glm::normalize(v1);
	glm::vec3 n2 = glm::normalize(v2);
	glm::vec3 nperp = glm::normalize(perp);

	if (n1 == n2)
		return rotQuat;

	if (n1 == -n2)
		return glm::angleAxis(glm::pi<float>(), nperp);

	glm::vec3 cross = glm::cross(n1, n2);
	float angle = glm::asin(glm::length(cross));
	if (glm::dot(n1, n2) < 0)
		angle = glm::pi<float>() - angle;
	glm::vec3 axis = glm::normalize(cross);

	return glm::angleAxis(angle, axis);
}

void Portals::draw()
{
	cam1->position = camera->position;
	cam1->direction = camera->direction;
	cam1->up = camera->up;

	cam2->position = camera->position;
	cam2->direction = camera->direction;
	cam2->up = camera->up;

	for (int i = 0; i < portalDepth; i++)
	{
		int j = i + portalDepth;

		if (i == 0)
		{
			view = glm::lookAt(camera->position, camera->position + camera->direction, camera->up);
			shader->setUniformMat4("view", view);

			glStencilMask(0xFF);
			glClear(GL_STENCIL_BUFFER_BIT);

			glStencilOp(GL_KEEP, GL_REPLACE, GL_KEEP);

			glDepthFunc(GL_NEVER);

			glStencilFunc(GL_ALWAYS, i + 1, 0xFF);
			portal2->draw(shader);

			glStencilFunc(GL_ALWAYS, j + 1, 0xFF);
			portal1->draw(shader);

			glDepthFunc(GL_LESS);

			glStencilFunc(GL_EQUAL, 0, 0xFF);

			drawPortalOutline(portal2, shader);
			drawPortalOutline(portal1, shader);

			glStencilMask(0x00);

			for (PortalSurface wall : portalSurfaces)
				wall.plane->draw(shader);

			glStencilFunc(GL_ALWAYS, 0, 0xFF);
			box->draw(shader);
			tower->draw(shader);
		}
		else
		{
			drawPortalContents(i, portal1, portal2, cam1, 0);
			drawPortalContents(j, portal2, portal1, cam2, 1);
		}
	}
}

void Portals::drawPortalContents(int i, Plane* port1, Plane* port2, Camera* cam, int clip)
{
	glm::vec3 transformedPos = port2->rotationQuaternion * glm::vec4(cam->position - port2->position, 0);
	glm::vec3 transformedDir = port2->rotationQuaternion * glm::vec4(cam->direction, 0);
	glm::vec3 transformedUp = port2->rotationQuaternion * glm::vec4(cam->up, 0);

	cam->position = port1->position - transformedPos.x * port1->right + transformedPos.y * port1->up - transformedPos.z * port1->normal;
	cam->direction = -transformedDir.x * port1->right + transformedDir.y * port1->up - transformedDir.z * port1->normal;
	cam->up = -transformedUp.x * port1->right + transformedUp.y * port1->up - transformedUp.z * port1->normal;

	view = glm::lookAt(cam->position, cam->position + cam->direction, cam->up);
	shader->setUniformMat4("view", view);

	glStencilFunc(GL_EQUAL, i, 0xFF);

	glStencilMask(0xFF);

	glDepthFunc(GL_NEVER);
	port2->draw(shader);
	glDepthFunc(GL_LESS);

	glEnable(GL_CLIP_DISTANCE0 + clip);

	drawPortalOutline(port2, shader);

	glStencilMask(0x00);

	for (PortalSurface wall : portalSurfaces)
		wall.plane->draw(shader);

	box->draw(shader);
	tower->draw(shader);

	glStencilFunc(GL_EQUAL, i + 1, 0xFF);
	box->draw(shader);
	tower->draw(shader);

	glDisable(GL_CLIP_DISTANCE0 + clip);
}

void Portals::drawPortalOutline(Plane* p, Shader* s)
{
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	p->scale += glm::vec3(0.2f, 0.2f, 0);
	p->draw(s);
	p->scale -= glm::vec3(0.2f, 0.2f, 0);
	glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
}

Portals::~Portals()
{
	delete shader;
	delete camera;
	delete cam1;
	delete cam2;
	delete portal1;
	delete portal2;

	for (PortalSurface wall : portalSurfaces)
		delete wall.plane;

	delete box;
	delete tower;
	delete player;
}
