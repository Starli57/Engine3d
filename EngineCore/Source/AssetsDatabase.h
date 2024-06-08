#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_map>

#include "EngineShared/Ref.h"
#include "Entities/Texture.h"

class AssetsDatabase
{
public:
	AssetsDatabase();
	~AssetsDatabase();

	bool HasTexture(TextureId textureId);
	Ref<Texture> GetTexture(TextureId textureId);
	void AddTexture(Ref<Texture> texture);
	void RemoveTexture(TextureId textureId);

private:
	std::unordered_map<TextureId, Ref<Texture>> textures;
};