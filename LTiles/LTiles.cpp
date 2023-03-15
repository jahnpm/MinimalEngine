#include "LTiles.h"


LTiles::LTiles()
{
	screenWidth = 1920;
	screenHeight = 1080;
}


LTiles::~LTiles()
{
}

void LTiles::init()
{
	shader = new Shader("ltilesvs.glsl", "ltilesfs.glsl");

	lTile = new LTile();
	lTile->scale = glm::vec3(25);

	k = 1;

	glfwSetTime(0);
}

void LTiles::update()
{
	if (glfwGetTime() >= 1)
	{
		if (k < 5)
			k++;
		else
			k = 1;

		iterations = 0;

		tileData = getLTiles(k, glm::vec2(0), 0);

		std::cout << iterations << "\n";

		glfwSetTime(0);
	}

	handleUserInput(window);
}

void LTiles::handleUserInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		lTile->scale += glm::vec3(0.01f);

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		if (lTile->scale.x >= 0.01f)
			lTile->scale -= glm::vec3(0.01f);
}

void LTiles::draw()
{
	glm::mat4 projection = glm::ortho(-screenWidth / 2.0f, screenWidth / 2.0f, -screenHeight / 2.0f, screenHeight / 2.0f, -1.0f, 1.0f);

	glUseProgram(shader->id);
	shader->setUniformMat4("projection", projection);

	glBindVertexArray(lTile->vertexArray);

	for (int i = 0; i < tileData.size(); i++)
	{
		glm::mat4 model;
		model = glm::translate(model, lTile->scale.x * glm::vec3(tileData[i].x, tileData[i].y, 0));
		model = glm::rotate(model, glm::radians(tileData[i].z), glm::vec3(0, 0, -1));
		model = glm::scale(model, glm::vec3(lTile->scale.x, lTile->scale.y, lTile->scale.z));
		shader->setUniformMat4("model", model);

		glm::vec3 color;

		if (tileData[i].z == 0)
			color = glm::vec3(1.0f * tileData[i].x / 64.0f, 0.0f, 0.0f);
		if (tileData[i].z == 90)
			color = glm::vec3(0.0f, 1.0f * tileData[i].x / 64.0f, 0.0f);
		if (tileData[i].z == 180)
			color = glm::vec3(0.0f, 0.0f, 1.0f * tileData[i].x / 64.0f);
		if (tileData[i].z == 270)
			color = glm::vec3(1.0f * tileData[i].x / 64.0f, 1.0f * tileData[i].x / 64.0f, 0.0f);

		shader->setUniformVec3("color", color);

		glDrawArrays(GL_TRIANGLES, 0, lTile->vertexCount);
	}

	glBindVertexArray(0);
}

glm::vec2 rotate90(glm::vec2 v)
{
	if (v.x * v.y > 0)
		return v * glm::vec2(1, -1);

	return v * glm::vec2(-1, 1);
}

std::vector<glm::vec3> LTiles::getLTiles(int k, glm::vec2 offset, float rot)
{
	iterations++;

	if (rot >= 360)
		rot -= 360;

	std::vector<glm::vec3> result;

	if (k == 1)
		result.push_back(glm::vec3(offset.x, offset.y, rot));
	else
	{
		glm::vec2 first, second, third, fourth;

		if (rot == 90)
			first = glm::vec2(1, -1);
		else if (rot == 180)
			first = glm::vec2(-1, -1);
		else if (rot == 270)
			first = glm::vec2(-1, 1);
		else
			first = glm::vec2(1, 1);

		second = rotate90(first);
		third = rotate90(second);
		fourth = rotate90(third);

		std::vector<glm::vec3> tr = getLTiles(k - 1, offset + first * glm::vec2(glm::pow(2, k - 2), glm::pow(2, k - 2)), rot + 0);
		std::vector<glm::vec3> br = getLTiles(k - 1, offset + second * glm::vec2(glm::pow(2, k - 2), glm::pow(2, k - 2)), rot + 270);
		std::vector<glm::vec3> bl = getLTiles(k - 1, offset + third * glm::vec2(glm::pow(2, k - 2), glm::pow(2, k - 2)), rot + 0);
		std::vector<glm::vec3> tl = getLTiles(k - 1, offset + fourth * glm::vec2(glm::pow(2, k - 2), glm::pow(2, k - 2)), rot + 90);

		result.push_back(glm::vec3(offset.x, offset.y, rot));

		result.insert(result.end(), tr.begin(), tr.end());
		result.insert(result.end(), br.begin(), br.end());
		result.insert(result.end(), bl.begin(), bl.end());
		result.insert(result.end(), tl.begin(), tl.end());
	}

	return result;
}

