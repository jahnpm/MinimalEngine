#include "WorldObject.h"


WorldObject::WorldObject()
{
	position = glm::vec3(0);
	color = glm::vec4(1);
	rotation = glm::vec3(0);
	scale = glm::vec3(1);
	textureRepeatCountS = 1.0f;
	textureRepeatCountT = 1.0f;
	textured = true;
	texCoordsPerVertex = 2;
	instanceCount = 0;

	glGenVertexArrays(1, &vertexArray);
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &textureBuffer);
	glGenBuffers(1, &instanceBuffer);
}


WorldObject::~WorldObject()
{
	glDeleteVertexArrays(1, &vertexArray);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &textureBuffer);
	glDeleteBuffers(1, &normalBuffer);

	delete mesh;
	delete textureCoordinates;
	delete normals;
	delete texture;
}

void WorldObject::fillVertexBuffer()
{
	glBindVertexArray(vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, 3 * vertexCount * sizeof(float), mesh, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void WorldObject::fillTextureBuffer()
{
	glBindVertexArray(vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);

	glBufferData(GL_ARRAY_BUFFER, texCoordsPerVertex * vertexCount * sizeof(float), textureCoordinates, GL_STATIC_DRAW);

	glVertexAttribPointer(1, texCoordsPerVertex, GL_FLOAT, GL_FALSE, texCoordsPerVertex * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void WorldObject::fillInstanceBuffer()
{
	glBindVertexArray(vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * instanceCount, instances, GL_STATIC_DRAW);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1);
}

void WorldObject::draw(Shader *shader)
{
	glm::mat4 model;
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	model = model * glm::mat4_cast(rotationQuaternion);
	model = glm::scale(model, scale);

	shader->setUniformMat4("model", model);
	shader->setUniformVec4("color", color);
	shader->setUniformBool("textured", textured);

	if (textured)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->id);
	}

	glBindVertexArray(vertexArray);
	
	if (instanceCount == 0)
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	else
		glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, instanceCount);
}
