#include "LTile.h"



LTile::LTile()
{
	vertexCount = 18;

	mesh = new float[3 * vertexCount]
	{
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	};

	glGenVertexArrays(1, &vertexArray);
	glGenBuffers(1, &vertexBuffer);

	glBindVertexArray(vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * vertexCount * sizeof(float), mesh, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}


LTile::~LTile()
{
}
