#pragma once

class Texture
{
public:

	Texture(const char *file);
	~Texture();

	void setWrapS(int param);
	void setWrapT(int param);
	void setMinFilter(int param);
	void setMagFilter(int param);

	unsigned int id;
};

