#include "TestGame.h"
#include <iostream>
#include <random>

TestGame::TestGame()
{
	screenWidth = 1920;
	screenHeight = 1080;
	fullScreen = false;
}

void TestGame::init()
{
	shader = new Shader("vertexshader.glsl", "fragshader.glsl");
	shader->setUniformTex("texture0", 0);
	shader->setUniformTex("texture1", 1);
	shader->setUniformTex("texture2", 2);

	cube = new Cube("grassblock.png", "grassblock_top.png", "grassblock_bottom.png");

	camera = new Camera(screenWidth, screenHeight);
	camera->position = glm::vec3(0, 0, 0);
	camera->direction = glm::vec3(0, 0, -1);

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	previousMouse = glm::vec2((float)mouseX, (float)mouseY);

	previousTime = glfwGetTime();

	seed = 53654754;
	mapSize = 128;

	for (int x = 0; x < 2 * mapSize; x++)
	{
		heightMap.push_back(std::vector<int>());

		for (int z = 0; z < 2 * mapSize; z++)
		{
			float noise1 = 64.0f * (perlinNoise(glm::vec2(x / 128.0f, z / 128.0f)) + 0.5f);
			float noise2 = 16.0f * (perlinNoise(glm::vec2(x / 32.0f, z / 32.0f)) + 0.5f);
			float noise3 = 8.0f * (perlinNoise(glm::vec2(x / 16.0f, z / 16.0f)) + 0.5f);
			heightMap[x].push_back(noise1 + noise2 + noise3);
		}
	}

	glUseProgram(shader->id);

	projection = glm::perspective(camera->fov, camera->aspectRatio, camera->nearClip, camera->farClip);
	shader->setUniformMat4("projection", projection);

	farHeight = 2.0f * glm::tan(camera->fov / 2.0f) * camera->farClip;
	farWidth = farHeight * camera->aspectRatio;
	glm::vec3 fc = camera->position + (camera->direction * camera->farClip);
	glm::vec3 ftl = fc + (camera->up * farHeight * 0.5f) - (camera->right * farWidth * 0.5f);

	camera->viewSphereCenter = camera->position + camera->direction * (camera->nearClip + (camera->farClip - camera->nearClip) * 0.5f);
	camera->viewSphereRadius = (camera->farClip - camera->nearClip) * 0.5f;
}

void TestGame::update()
{
	float currentTime = glfwGetTime();
	float deltaTime = currentTime - previousTime;
	previousTime = currentTime;

	handleUserInput(window, deltaTime);
}

void TestGame::handleUserInput(GLFWwindow *window, float deltaTime)
{
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	glm::vec2 currentMouse = glm::vec2((float)mouseX, (float)mouseY);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->position -= camera->right * camera->speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->position += camera->right * camera->speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->position += camera->direction * camera->speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->position -= camera->direction * camera->speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->position.y += camera->speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera->position.y -= camera->speed * deltaTime;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glm::vec2 mouseOffset = currentMouse - previousMouse;

		camera->direction = glm::rotate(camera->direction, glm::radians(-mouseOffset.y) * camera->speed * 10.0f * deltaTime, camera->right);
		camera->direction = glm::rotate(camera->direction, glm::radians(-mouseOffset.x) * camera->speed * 10.0f * deltaTime, camera->up);

		camera->right = glm::normalize(glm::cross(camera->direction, camera->up));
	}

	previousMouse = currentMouse;
}

void TestGame::draw()
{
	view = glm::lookAt(camera->position, camera->position + camera->direction, camera->up);
	shader->setUniformMat4("view", view);

	camera->viewSphereCenter = camera->position + camera->direction * (camera->nearClip + (camera->farClip - camera->nearClip) * 0.5f);

	for (int x = -mapSize; x < mapSize; x++)
		for (int z = -mapSize; z < mapSize; z++)
		{
			int y = heightMap[x + mapSize][z + mapSize];

			if (glm::length(glm::vec3(x, y, z) - camera->viewSphereCenter) > camera->viewSphereRadius)
				continue;

			cube->position = glm::vec3(x, y, z);
			cube->drawTripleTexture(shader);
		}
}

float mix(float a, float b, float t)
{
	return (1 - t)*a + t*b;
}

float fade(float t)
{
	return t*t*t*(t*(t * 6 - 15) + 10);
}

float TestGame::perlinNoise(glm::vec2 position)
{
	int x = (position.x > 0 ? (int)position.x : (int)position.x - 1);
	int y = (position.y > 0 ? (int)position.y : (int)position.y - 1);

	position.x -= x;
	position.y -= y;

	glm::vec2 gradients[4];

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			std::seed_seq seedSeq{ seed, x + i, y + j };
			std::vector<unsigned int> seeds(1);
			seedSeq.generate(seeds.begin(), seeds.end());
			std::default_random_engine randEng(seeds[0]);
			std::uniform_real_distribution<float> realDist(0.0f, 1.0f);

			float angle = glm::radians(realDist(randEng) * 360.0f);
			gradients[2 * i + j] = glm::vec2(glm::cos(angle), glm::sin(angle));
		}
	}

	float n00 = glm::dot(gradients[0], position);
	float n01 = glm::dot(gradients[1], position - glm::vec2(0, 1));
	float n10 = glm::dot(gradients[2], position - glm::vec2(1, 0));
	float n11 = glm::dot(gradients[3], position - glm::vec2(1, 1));

	float u = fade(position.x);
	float v = fade(position.y);

	float nx00 = mix(n00, n10, u);
	float nx01 = mix(n01, n11, u);

	return mix(nx00, nx01, v);
}

TestGame::~TestGame()
{
	delete shader;
	delete cube;
	delete camera;
}
