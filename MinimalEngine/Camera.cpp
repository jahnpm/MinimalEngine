#include "Camera.h"

Camera::Camera(float screenWidth, float screenHeight)
{
	position = glm::vec3(0);
	direction = glm::vec3(0, 0, -1);
	up = glm::vec3(0, 1, 0);
	right = glm::vec3(1, 0, 0);
	rotation = glm::vec3(0);

	fov = 45.0f;
	aspectRatio = screenWidth / screenHeight;
	nearClip = 0.1f;
	farClip = 100.0f;

	speed = 7.5f;
}

void Camera::updateFlying(GLFWwindow *window, float deltaTime, glm::vec2 deltaMouse)
{
	float xAngle = 0.1f * deltaMouse.y * deltaTime;
	float yAngle = 0.1f * deltaMouse.x * deltaTime;

	if (rotation.x + xAngle > glm::radians(89.0f))
		//xAngle = glm::radians(89.0f) - rotation.x;

	if (rotation.x + xAngle < glm::radians(-89.0f))
		//xAngle = glm::radians(-89.0f) - rotation.x;

	rotation.x += xAngle;
	rotation.y += yAngle;

	if (rotation.y > glm::two_pi<float>())
		rotation.y -= glm::two_pi<float>();

	if (rotation.y < 0)
		rotation.y += glm::two_pi<float>();

	glm::quat rotQuatX = glm::angleAxis(-xAngle, right);
	glm::quat rotQuatY = glm::angleAxis(-yAngle, up);

	direction = rotQuatY * rotQuatX * direction;
	right = rotQuatY * right;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		position += direction * speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		position -= right * speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		position -= direction * speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		position += right * speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		position += up * speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		position -= up * speed * deltaTime;
}

void Camera::updateProjectionView(Shader* shader)
{
	glm::mat4 projection = glm::perspective(fov, aspectRatio, nearClip, farClip);
	shader->setUniformMat4("projection", projection);

	glm::mat4 view = glm::lookAt(position, position + direction, up);
	shader->setUniformMat4("view", view);
}

Camera::~Camera()
{
}
