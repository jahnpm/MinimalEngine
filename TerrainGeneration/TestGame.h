#pragma once

#include "Game.h"
#include "Cube.h"
#include "Shader.h"
#include "Camera.h"
#include <vector>

class TestGame :
	public Game
{
public:

	TestGame();

	/* executed once before entering the game loop */
	void init();

	void update();

	void draw();

	/**
	 * \brief	calculates 2D Perlin noise
	 * 			
	 * 			This method uses the classic Perlin noise algorithm to interpolate
	 * 			naturally between points on a two dimensional plane. Note that passing
	 * 			an integer vector always results in the same output.
	 *
	 * \param	position	noise will be calculated for this position
	 *
	 * \return	noise value at the given position between -0.5 and 0.5
	 */
	float perlinNoise(glm::vec2 position);

	~TestGame();

	Shader *shader;
	Cube *cube;
	Camera *camera;

private:

	void handleUserInput(GLFWwindow *window, float deltaTime);

	glm::vec2 previousMouse;

	float previousTime;
	int seed;   /**< needed to calculate random Perlin noise */
	int mapSize;
	std::vector<std::vector<int>> heightMap;

	glm::mat4 view;
	glm::mat4 projection;

	float farHeight;
	float farWidth;
};

