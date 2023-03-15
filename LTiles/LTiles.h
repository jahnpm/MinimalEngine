#pragma once

#include "Game.h"
#include "Shader.h"
#include "LTile.h"
#include <vector>

class LTiles :
	public Game
{
public:
	LTiles();
	~LTiles();

	/* executed once before entering the game loop */
	void init();
	void update();
	void draw();

	Shader *shader;
	LTile *lTile;

private:

	void handleUserInput(GLFWwindow *window);
	std::vector<glm::vec3> getLTiles(int k, glm::vec2 offset, float rot);

	std::vector<glm::vec3> tileData;

	int k;
	int iterations;
};

