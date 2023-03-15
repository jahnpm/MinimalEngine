#include "Game.h"
#include "Camera.h"
#include "Cube.h"

class HelloWorld : public Game
{
public:
	~HelloWorld();

private:
	void init();
	void update();
	void draw();

	Shader* shader;
	Cube* cube;
	Camera* camera;
};

void HelloWorld::init()
{
	shader = new Shader("vertex.glsl", "fragment.glsl");
	glUseProgram(shader->id);

	cube = new Cube("texture.png");

	camera = new Camera(screenWidth, screenHeight);
	camera->position = glm::vec3(0, 2.5f, 2.5f);
	camera->direction = glm::vec3(0, -1, -1);
	camera->updateProjectionView(shader);
}

void HelloWorld::update()
{
	cube->rotation += glm::vec3(0, 0.2f, 0);
}

void HelloWorld::draw()
{
	cube->draw(shader);
}

HelloWorld::~HelloWorld()
{
	delete shader;
	delete cube;
	delete camera;
}

int main()
{
	HelloWorld* helloWorld = new HelloWorld();
	helloWorld->run();

	delete helloWorld;

	return 0;
}