#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_map>

#include "SharedLib/Ref.h"
#include "Renderer/Entities/Texture.h"

class AssetsDatabase
{
public:
	AssetsDatabase();
	~AssetsDatabase();

	bool HasTexture(const std::string& path);
	Ref<Texture> GetTexture(const std::string& path);
	void AddTexture(Ref<Texture> texture);
	void RemoveTexture(Ref<Texture> texture);
	void RemoveTexture(const std::string& path);

private:
	std::unordered_map<std::string, Ref<Texture>> textures;
};