#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_map>

#include "Architecture/Ref.h"
#include "Rendering/Entity/Texture.h"

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