#version 330 core

uniform vec4 color;
uniform vec3 portal1Pos;
uniform vec3 portal2Pos;
uniform vec3 portal1Norm;
uniform vec3 portal2Norm;
uniform vec3 fpa;
uniform vec3 fpb;
uniform vec3 fpc;
uniform vec3 spa;
uniform vec3 spb;
uniform vec3 spc;
uniform sampler2D tex;
uniform bool textured;

in vec3 norm;
in vec3 fragPos;
in vec2 texCoord;

out vec4 FragColor;

void main()
{
	/*
	vec3 fpam = fragPos - fpa;

	if (abs(dot(fpam, portal1Norm)) <= 0.01f && textured)
	{
		vec3 fpab = fpb - fpa;
		vec3 fpbc = fpc - fpb;
		vec3 fpbm = fragPos - fpb;

		float dotabam1 = dot(fpab, fpam);
		float dotbcbm1 = dot(fpbc, fpbm);

		if (0 <= dotabam1 && dotabam1 <= dot(fpab, fpab)
			&& 0 <= dotbcbm1 && dotbcbm1 <= dot(fpbc, fpbc))
			discard;
	}

	vec3 spam = fragPos - spa;

	if (abs(dot(spam, portal2Norm)) <= 0.01f && textured)
	{
		vec3 spab = spb - spa;
		vec3 spbc = spc - spb;
		vec3 spbm = fragPos - spb;

		float dotabam2 = dot(spab, spam);
		float dotbcbm2 = dot(spbc, spbm);

		if (0 <= dotabam2 && dotabam2 <= dot(spab, spab)
			&& 0 <= dotbcbm2 && dotbcbm2 <= dot(spbc, spbc))
			discard;
	}
	*/

	if (textured)
		FragColor = texture(tex, texCoord);
	else
		FragColor = color;
}