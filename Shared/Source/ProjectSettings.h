#pragma once
#include <string>

struct ProjectSettigns
{
	const std::string resourcesPath = "Resources/";
	const std::string shadersPath = "Shaders/";

	ProjectSettigns(const std::string& resources, const std::string& shaders)
		: resourcesPath(resources), shadersPath(shaders) 
	{
	};
};