#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

/* an abstract class providing a skeleton for a game */
class Game
{
public:

	Game();

	void run();

	/* executed once before entering the game loop */
	virtual void init();

	/**
	 * \brief	called once per frame, should be overridden in a child class
	 * 			
	 * 			This method is executed once per frame before the draw method is called.\n
	 * 			It is meant to be overridden in a child class to control the game's flow by\n
	 * 			updating transformations, performing collision detection and processing user input.
	 */
	virtual void update();

	/* executed once per frame after update(), draws stuff */
	virtual void draw();

	~Game();

	int glVersionMajor, glVersionMinor;
	float screenWidth, screenHeight;
	bool fullScreen;
	int vsync;
	std::string windowTitle;	/**< The window title */
	GLFWwindow *window;
	//bool catchCursor;

private:

	bool initGLFW();
};