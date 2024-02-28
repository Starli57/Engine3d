#include "Pch.h"
#include "Material.h"

Material::Material(Ref<AssetsDatabase> assetsDatabase, const std::string& mainTexturePath)
{
	if (assetsDatabase->HasTexture(mainTexturePath))
	{
		mainTexture = assetsDatabase->GetTexture(mainTexturePath);
	}
	else
	{
		mainTexture = CreateRef<Texture>(mainTexturePath);
		assetsDatabase->AddTexture(mainTexture);
	}
}

Material::~Material()
{
	mainTexture.reset();
}
