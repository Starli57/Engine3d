#pragma once

class Texture
{
public:
	Texture(const std::string& path, int& width, int& height);
	virtual ~Texture();
};

