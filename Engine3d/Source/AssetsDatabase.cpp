#include "Pch.h"
#include "AssetsDatabase.h"
#include "SharedLib/CustomAssert.h"

//todo: calculate texture links count to be able to dispose if not used

AssetsDatabase::AssetsDatabase()
{
	textures = std::unordered_map<TextureId, Ref<Texture>>();
}
	
AssetsDatabase::~AssetsDatabase()
{
	//todo: check if vk objects are disposed correctly
	for (auto& pair : textures) {
		Ref<Texture>& textureRef = pair.second;
		textureRef.reset();
	}

	textures.clear();
}

bool AssetsDatabase::HasTexture(TextureId textureId)
{
	return textures.find(textureId) != textures.end();
}

Ref<Texture> AssetsDatabase::GetTexture(TextureId textureId)
{
	auto it = textures.find(textureId);
	CAssert::Check(it != textures.end(), "Assets database doesn't have a texture with path: " + (int)textureId);
	return it->second;
}

void AssetsDatabase::AddTexture(Ref<Texture> texture)
{
	if (!HasTexture(texture->textureId)) textures.insert({ texture->textureId, texture });
}

void AssetsDatabase::RemoveTexture(TextureId textureId)
{
	auto it = textures.find(textureId);
	CAssert::Check(it != textures.end(), "Assets database doesn't have a texture with path: " + (int)textureId);
	textures.erase(it->first);
}
