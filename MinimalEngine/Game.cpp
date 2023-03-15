#include "Game.h"

Game::Game()
{
	glVersionMajor = 3;
	glVersionMinor = 3;
	screenWidth = 800;
	screenHeight = 600;
	fullScreen = false;
	vsync = 1;
	windowTitle = "Game";
	//catchCursor = true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void Game::run()
{
	initGLFW();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	glEnable(GL_DEPTH_TEST);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	init();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		update();

		// render
		// ------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		draw();
		// glBindVertexArray(0); // no need to unbind it every time 

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void Game::init()
{
	
}

void Game::update()
{
}

void Game::draw()
{
}

Game::~Game()
{
}

bool Game::initGLFW()
{
	// glfw: initialize and configure
	// ------------------------------
	if (!glfwInit())
		return false;

	std::cout << "GLFW version: " << glfwGetVersionString() << std::endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X

	// glfw window creation
	// --------------------
	window = glfwCreateWindow(screenWidth, screenHeight,
		windowTitle.data(), fullScreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSwapInterval(vsync);

	return true;
}
