#include "TerrainGeneration.h"
#include <iostream>
#include <random>

TerrainGeneration::TerrainGeneration()
{
	screenWidth = 2560;
	screenHeight = 1440;
	fullScreen = true;
}

void TerrainGeneration::init()
{
	glEnable(GL_CULL_FACE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	shader = new Shader("vertexshader.glsl", "fragshader.glsl");
	shader->setUniformTex("texture0", 0);
	shader->setUniformTex("texture1", 1);
	shader->setUniformTex("texture2", 2);
	glUseProgram(shader->id);

	seed = 53654754;
	mapSize = 256;

	std::vector<glm::vec3> positions;
	int highestPos = INT32_MIN;

	for (int x = 0; x < mapSize; x++)
	{
		for (int z = 0; z < mapSize; z++)
		{
			float noise1 = 64.0f * (perlinNoise(glm::vec2(x / 128.0f, z / 128.0f)) + 0.5f);
			float noise2 = 16.0f * (perlinNoise(glm::vec2(x / 32.0f, z / 32.0f)) + 0.5f);
			float noise3 = 8.0f * (perlinNoise(glm::vec2(x / 16.0f, z / 16.0f)) + 0.5f);
			
			int y = noise1 + noise2 + noise3;

			positions.push_back(glm::vec3(x, y, z));
			positions.push_back(glm::vec3(x, y - 1, z));
			positions.push_back(glm::vec3(x, y - 2, z));

			if (y > highestPos)
				highestPos = y;
		}
	}

	camera = new Camera(screenWidth, screenHeight);
	camera->position = glm::vec3(mapSize / 2, highestPos + 1, mapSize / 2);
	camera->direction = glm::vec3(0, 0, -1);
	camera->nearClip = 0.1f;
	camera->farClip = 200.0f;

	cube = new Cube("grassblock.png", "grassblock_top.png", "grassblock_bottom.png");
	cube->instanceCount = positions.size();
	cube->instances = positions.data();
	cube->fillInstanceBuffer();

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	previousMouse = glm::vec2((float)mouseX, (float)mouseY);

	previousTime = glfwGetTime();
}

void TerrainGeneration::update()
{
	float currentTime = glfwGetTime();
	float deltaTime = currentTime - previousTime;
	previousTime = currentTime;

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	glm::vec2 currentMouse = glm::vec2((float)mouseX, (float)mouseY);
	glm::vec2 mouseOffset = currentMouse - previousMouse;
	previousMouse = currentMouse;

	camera->updateFlying(window, deltaTime, mouseOffset);
	camera->updateProjectionView(shader);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void TerrainGeneration::draw()
{
	cube->drawTripleTexture(shader);
}

float mix(float a, float b, float t)
{
	return (1 - t) * a + t * b;
}

float fade(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float TerrainGeneration::perlinNoise(glm::vec2 position)
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

TerrainGeneration::~TerrainGeneration()
{
	delete shader;
	delete cube;
	delete camera;
}
