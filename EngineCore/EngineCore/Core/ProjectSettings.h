#pragma once
#include <string>

struct ProjectSettigns
{
	const std::string projectPath = "";
	const std::string resourcesPath = "";
	const std::string prefabsPath = "Prefabs";

	ProjectSettigns(const std::string& projectPath) : projectPath(projectPath), 
		resourcesPath(projectPath + "Resources"), prefabsPath(projectPath + "Prefabs")
	{
	};
};