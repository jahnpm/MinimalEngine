#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoordIn;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 portal1Pos;
uniform vec3 portal2Pos;
uniform vec3 portal1Norm;
uniform vec3 portal2Norm;

out vec3 norm;
out vec3 fragPos;
out vec2 texCoord;

void main()
{
	fragPos = (model * vec4(pos, 1)).xyz;
	texCoord = texCoordIn;
	gl_ClipDistance[0] = dot(portal1Norm, fragPos - (portal1Pos + 0.01f * portal1Norm));
	gl_ClipDistance[1] = dot(portal2Norm, fragPos - (portal2Pos + 0.01f * portal2Norm));
	gl_Position = projection * view * model * vec4(pos, 1);
}