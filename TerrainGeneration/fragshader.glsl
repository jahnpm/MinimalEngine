#version 330 core

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

in vec2 texCoord;
flat in float texID;

out vec4 FragColor;

void main()
{
	vec4 color;

	if (texID > -0.5 && texID < 0.5)
	{
		color = texture(texture0, texCoord);
	}
	else if (texID > 0.5 && texID < 1.5)
	{
		color = texture(texture1, texCoord);
	}
	else if (texID > 1.5 && texID < 2.5)
	{
		color = texture(texture2, texCoord);
	}

	FragColor = color;
}