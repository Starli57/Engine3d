#pragma once

class Texture
{
public:
	const std::string& path;

	Texture(const std::string& path);
	virtual ~Texture();
};

