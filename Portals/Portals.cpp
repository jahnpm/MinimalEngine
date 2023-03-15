#include "Portals.h"


Portals::Portals()
{
	screenWidth = 1920;
	screenHeight = 1080;
	fullScreen = false;
}

void Portals::init()
{
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

	shader = new Shader("portalsvs.glsl", "portalsfs.glsl");
	glUseProgram(shader->id);

	portalDepth = 16;

	camera = new Camera(screenWidth, screenHeight);
	camera->position = glm::vec3(0, 0, 0);
	camera->direction = glm::normalize(glm::vec3(0, 0, -1));

	projection = glm::perspective(camera->fov, camera->aspectRatio, camera->nearClip, camera->farClip);
	shader->setUniformMat4("projection", projection);

	portal1 = new Plane(NULL);
	portal1->textured = false;
	portal1->normal = glm::vec3(0, 0, 1);
	portal1->position = glm::vec3(0, 0, -5);
	portal1->scale = glm::vec3(2, 3, 0);
	portal1->up = glm::vec3(0, 1, 0);
	portal1->right = glm::vec3(1, 0, 0);
	portal1->rotationQuaternion = glm::mat4_cast(
		getRotationQuat(portal1->normal, glm::vec3(0, 0, 1), portal1->up));

	portal2 = new Plane(NULL);
	portal2->textured = false;
	portal2->normal = glm::vec3(-1, 0, 0);
	portal2->position = glm::vec3(7.5f, 0, 0);
	portal2->scale = glm::vec3(2, 3, 0);
	portal2->up = glm::vec3(0, 1, 0);
	portal2->right = glm::vec3(0, 0, 1);
	portal2->rotationQuaternion = glm::mat4_cast(
		getRotationQuat(portal2->normal, glm::vec3(0, 0, 1), portal2->up));

	wallNorth = new Plane("wallTexture.jpg");
	wallNorth->texture->setWrapS(GL_MIRRORED_REPEAT);
	wallNorth->textureRepeatCountS = 3;
	wallNorth->updateTextureCoordinates();
	wallNorth->position = glm::vec3(0, 0, -5);
	wallNorth->scale = glm::vec3(15, 5, 1);

	wallEast = new Plane("wallTexture.jpg");
	wallEast->texture->setWrapS(GL_MIRRORED_REPEAT);
	wallEast->textureRepeatCountS = 3;
	wallEast->updateTextureCoordinates();
	wallEast->position = glm::vec3(7.5f, 0, 0);
	wallEast->rotation.y = 90;
	wallEast->scale = glm::vec3(10, 5, 1);

	wallSouth = new Plane("wallTexture.jpg");
	wallSouth->texture->setWrapS(GL_MIRRORED_REPEAT);
	wallSouth->textureRepeatCountS = 3;
	wallSouth->updateTextureCoordinates();
	wallSouth->position = glm::vec3(0, 0, 5);
	wallSouth->scale = glm::vec3(15, 5, 1);

	wallWest = new Plane("wallTexture.jpg");
	wallWest->texture->setWrapS(GL_MIRRORED_REPEAT);
	wallWest->textureRepeatCountS = 3;
	wallWest->updateTextureCoordinates();
	wallWest->position = glm::vec3(-7.5f, 0, 0);
	wallWest->rotation.y = 90;
	wallWest->scale = glm::vec3(10, 5, 1);

	floor = new Plane("floorTexture.jpg");
	floor->texture->setWrapS(GL_MIRRORED_REPEAT);
	floor->texture->setWrapT(GL_MIRRORED_REPEAT);
	floor->textureRepeatCountS = 3;
	floor->textureRepeatCountT = 3;
	floor->updateTextureCoordinates();
	floor->position = glm::vec3(0, -2.5f, 0);
	floor->rotation.x = 90;
	floor->scale = glm::vec3(15, 10, 1);

	ceiling = new Plane("ceilingTexture.jpg");
	ceiling->texture->setWrapS(GL_MIRRORED_REPEAT);
	ceiling->texture->setWrapT(GL_MIRRORED_REPEAT);
	ceiling->textureRepeatCountS = 3;
	ceiling->textureRepeatCountT = 3;
	ceiling->updateTextureCoordinates();
	ceiling->position = glm::vec3(0, 2.5f, 0);
	ceiling->rotation.x = 90;
	ceiling->scale = glm::vec3(15, 10, 1);

	box = new Cube("objectTexture.jpg");
	box->position = glm::vec3(-3, -2, -2);
	box->rotation.y = 45;

	tower = new Cube("objectTexture.jpg");
	tower->position = glm::vec3(4.5f, -1, 2);
	tower->scale = glm::vec3(0.5f, 3, 0.5f);

	player = new Cube(NULL);
	player->position = camera->position;
	player->scale = glm::vec3(0.5f, 0.5f, 0.5f);

	fpa = portal1->position + 0.5f * portal1->scale.y * portal1->up - 0.5f * portal1->scale.x * portal1->right;
	fpb = portal1->position + 0.5f * portal1->scale.y * portal1->up + 0.5f * portal1->scale.x * portal1->right;
	fpc = portal1->position - 0.5f * portal1->scale.y * portal1->up + 0.5f * portal1->scale.x * portal1->right;

	spa = portal2->position + 0.5f * portal2->scale.y * portal2->up - 0.5f * portal2->scale.x * portal2->right;
	spb = portal2->position + 0.5f * portal2->scale.y * portal2->up + 0.5f * portal2->scale.x * portal2->right;
	spc = portal2->position - 0.5f * portal2->scale.y * portal2->up + 0.5f * portal2->scale.x * portal2->right;

	shader->setUniformVec3("portal1Pos", portal1->position);
	shader->setUniformVec3("portal2Pos", portal2->position);
	shader->setUniformVec3("portal1Norm", portal1->normal);
	shader->setUniformVec3("portal2Norm", portal2->normal);
	shader->setUniformVec3("fpa", fpa);
	shader->setUniformVec3("fpb", fpb);
	shader->setUniformVec3("fpc", fpc);
	shader->setUniformVec3("spa", spa);
	shader->setUniformVec3("spb", spb);
	shader->setUniformVec3("spc", spc);

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

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	glm::vec3 camToFirstPort = camera->position - portal1->position;
	float firstAngle = glm::dot(camToFirstPort, portal1->normal);

	glm::vec3 camToSecPort = camera->position - portal2->position;
	float secondAngle = glm::dot(camToSecPort, portal2->normal);

	if (glm::length(camToFirstPort) > 1 && glm::length(camToSecPort) > 1)
	{
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

	bool updatePortal1 = false, updatePortal2 = false;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		updatePortal1 = true;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		updatePortal2 = true;

	if (updatePortal1 || updatePortal2)
	{
		glm::vec3 n[6];
		n[0] = glm::vec3(1, 0, 0);
		n[1] = glm::vec3(-1, 0, 0);
		n[2] = glm::vec3(0, 1, 0);
		n[3] = glm::vec3(0, -1, 0);
		n[4] = glm::vec3(0, 0, 1);
		n[5] = glm::vec3(0, 0, -1);

		glm::vec3 p[6];
		p[0] = glm::vec3(-7.5f, 0, 0);
		p[1] = glm::vec3(7.5f, 0, 0);
		p[2] = glm::vec3(0, -2.5f, 0);
		p[3] = glm::vec3(0, 2.5f, 0);
		p[4] = glm::vec3(0, 0, -5);
		p[5] = glm::vec3(0, 0, 5);

		glm::vec3 q[6];
		q[0] = glm::vec3(-7.5f, 1, 0);
		q[1] = glm::vec3(7.5f, 1, 0);
		q[2] = glm::vec3(0, -2.5f, -1);
		q[3] = glm::vec3(0, 2.5f, 1);
		q[4] = glm::vec3(0, 1, -5);
		q[5] = glm::vec3(0, 1, 5);

		for (int i = 0; i < 6; i++)
		{
			float nl = glm::dot(n[i], camera->direction);

			if (nl != 0)
			{
				float d = glm::dot(p[i] - camera->position, n[i]) / nl;
				glm::vec3 x = d * camera->direction + camera->position;

				if (x.x >= -7.5f && x.x <= 7.5f
					&& x.y >= -2.5f && x.y <= 2.5f
					&& x.z >= -5 && x.z <= 5
					&& d >= 0)
				{
					if (updatePortal1)
					{
						portal1->position = x;
						portal1->normal = n[i];
						portal1->up = q[i] - p[i];
						portal1->rotationQuaternion = glm::mat4_cast(
							getRotationQuat(portal1->normal, glm::vec3(0, 0, 1), portal1->up));

						portal1->right = -glm::normalize(glm::cross(portal1->normal, portal1->up));

						fpa = portal1->position + 0.5f * portal1->scale.y * portal1->up - 0.5f * portal1->scale.x * portal1->right;
						fpb = portal1->position + 0.5f * portal1->scale.y * portal1->up + 0.5f * portal1->scale.x * portal1->right;
						fpc = portal1->position - 0.5f * portal1->scale.y * portal1->up + 0.5f * portal1->scale.x * portal1->right;

						shader->setUniformVec3("portal1Pos", portal1->position);
						shader->setUniformVec3("portal1Norm", portal1->normal);
						shader->setUniformVec3("fpa", fpa);
						shader->setUniformVec3("fpb", fpb);
						shader->setUniformVec3("fpc", fpc);

						break;
					}
					else if (updatePortal2)
					{
						portal2->position = x;
						portal2->normal = n[i];
						portal2->up = q[i] - p[i];
						portal2->rotationQuaternion = glm::mat4_cast(
							getRotationQuat(portal2->normal, glm::vec3(0, 0, 1), portal2->up));

						portal2->right = -glm::normalize(glm::cross(portal2->normal, portal2->up));

						spa = portal2->position + 0.5f * portal2->scale.y * portal2->up - 0.5f * portal2->scale.x * portal2->right;
						spb = portal2->position + 0.5f * portal2->scale.y * portal2->up + 0.5f * portal2->scale.x * portal2->right;
						spc = portal2->position - 0.5f * portal2->scale.y * portal2->up + 0.5f * portal2->scale.x * portal2->right;

						shader->setUniformVec3("portal2Pos", portal2->position);
						shader->setUniformVec3("portal2Norm", portal2->normal);
						shader->setUniformVec3("spa", spa);
						shader->setUniformVec3("spb", spb);
						shader->setUniformVec3("spc", spc);

						break;
					}
				}
			}
		}
	}
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
	//save camera
	glm::vec3 camPos1 = camera->position;
	glm::vec3 camDir1 = camera->direction;
	glm::vec3 camUp1 = camera->up;

	glm::vec3 camPos2 = camera->position;
	glm::vec3 camDir2 = camera->direction;
	glm::vec3 camUp2 = camera->up;

	view = glm::lookAt(camPos2, camPos2 + camDir2, camUp2);
	shader->setUniformMat4("view", view);

	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilMask(0x00);
	glStencilFunc(GL_EQUAL, 0, 0xFF);

	wallNorth->draw(shader);
	wallEast->draw(shader);
	wallSouth->draw(shader);
	wallWest->draw(shader);
	floor->draw(shader);
	ceiling->draw(shader);
	box->draw(shader);
	tower->draw(shader);

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glDepthMask(GL_FALSE);

	portal2->draw(shader);

	glDepthMask(GL_TRUE);

	for (int i = 1; i <= portalDepth; i++)
	{
		glm::vec3 transformedPos2 = portal2->rotationQuaternion * glm::vec4(camPos2 - portal2->position, 0);
		camPos2 = portal1->position - transformedPos2.x * portal1->right + transformedPos2.y * portal1->up - transformedPos2.z * portal1->normal;
		glm::vec3 transformedDir2 = portal2->rotationQuaternion * glm::vec4(camDir2, 0);
		camDir2 = -transformedDir2.x * portal1->right + transformedDir2.y * portal1->up - transformedDir2.z * portal1->normal;
		glm::vec3 transformedUp2 = portal2->rotationQuaternion * glm::vec4(camUp2, 0);
		camUp2 = -transformedUp2.x * portal1->right + transformedUp2.y * portal1->up - transformedUp2.z * portal1->normal;

		view = glm::lookAt(camPos2, camPos2 + camDir2, camUp2);
		shader->setUniformMat4("view", view);

		glStencilMask(0x00);
		glStencilFunc(GL_EQUAL, i, 0xFF);

		glEnable(GL_CLIP_DISTANCE0);

		wallNorth->draw(shader);
		wallEast->draw(shader);
		wallSouth->draw(shader);
		wallWest->draw(shader);
		floor->draw(shader);
		ceiling->draw(shader);
		box->draw(shader);
		tower->draw(shader);

		glDisable(GL_CLIP_DISTANCE0);

		glStencilMask(0xFF);
		glDepthMask(GL_FALSE);
		glStencilFunc(GL_EQUAL, i, 0xFF);

		portal2->draw(shader);

		glDepthMask(GL_TRUE);
	}

	view = glm::lookAt(camPos1, camPos1 + camDir1, camUp1);
	shader->setUniformMat4("view", view);

	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glDepthMask(GL_FALSE);

	portal1->draw(shader);

	glDepthMask(GL_TRUE);

	for (int i = 1; i <= portalDepth; i++)
	{
		//transform camera
		glm::vec3 transformedPos1 = portal1->rotationQuaternion * glm::vec4(camPos1 - portal1->position, 0);
		camPos1 = portal2->position - transformedPos1.x * portal2->right + transformedPos1.y * portal2->up - transformedPos1.z * portal2->normal;
		glm::vec3 transformedDir1 = portal1->rotationQuaternion * glm::vec4(camDir1, 0);
		camDir1 = -transformedDir1.x * portal2->right + transformedDir1.y * portal2->up - transformedDir1.z * portal2->normal;
		glm::vec3 transformedUp1 = portal1->rotationQuaternion * glm::vec4(camUp1, 0);
		camUp1 = -transformedUp1.x * portal2->right + transformedUp1.y * portal2->up - transformedUp1.z * portal2->normal;

		view = glm::lookAt(camPos1, camPos1 + camDir1, camUp1);
		shader->setUniformMat4("view", view);

		glStencilMask(0x00);
		glStencilFunc(GL_EQUAL, i, 0xFF);

		glEnable(GL_CLIP_DISTANCE1);

		wallNorth->draw(shader);
		wallEast->draw(shader);
		wallSouth->draw(shader);
		wallWest->draw(shader);
		floor->draw(shader);
		ceiling->draw(shader);
		box->draw(shader);
		tower->draw(shader);

		glDisable(GL_CLIP_DISTANCE1);

		glStencilMask(0xFF);
		glDepthMask(GL_FALSE);
		glStencilFunc(GL_EQUAL, i, 0xFF);

		portal1->draw(shader);

		glDepthMask(GL_TRUE);
	}
}

Portals::~Portals()
{
	delete shader;
	delete camera;
}
