#pragma once
#include <string>

struct MaterialMeta
{
	std::string materialName;
	std::string pipelineName;

	std::string ambientTextureName;
	std::string diffuseTextureName;
	std::string specularTextureName;
	std::string specularHighlightTextureName;
	std::string bumpTextureName;
	std::string displacementTextureName;
	std::string alphaTextureName;
	std::string reflectionTextureName;
};