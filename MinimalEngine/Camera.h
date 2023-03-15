#pragma once

#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Camera
{
public:

	Camera(float screenWidth, float screenHeight);
	~Camera();

	void updateFlying(GLFWwindow *window, float deltaTime, glm::vec2 deltaMouse);

	void updateProjectionView(Shader* shader);

	glm::vec3 position; /**< the 3D position of the camera in the world */
	glm::vec3 direction;	/**< the direction in which the camera is looking */
	glm::vec3 up;   /**< a vector that points upwards from the camera */
	glm::vec3 right;	/**< a vector that points to the right of the camera */
	glm::vec3 rotation;


	float fov;  /**< field of view */
	float aspectRatio;
	float nearClip;
	float farClip;

	glm::vec3 viewSphereCenter;
	float viewSphereRadius;

	float speed;
};

