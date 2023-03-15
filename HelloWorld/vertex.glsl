#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoordIn;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;

void main()
{
	texCoord = texCoordIn;
	gl_Position = projection * view * model * vec4(position, 1);
}