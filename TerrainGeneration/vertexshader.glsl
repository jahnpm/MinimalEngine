#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTexCoord;
layout (location = 2) in vec3 aOffset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;
flat out float texID;

void main()
{
   gl_Position = projection * view * model * vec4(aPos + aOffset, 1.0);
   texCoord = aTexCoord.xy;
   texID = aTexCoord.z;
}