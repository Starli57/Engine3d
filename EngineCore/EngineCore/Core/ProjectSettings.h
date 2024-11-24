#pragma once
#include <string>
#include "EngineCore/Defines/DllDefines.h"

struct ProjectSettigns
{
	const std::string projectName = "Ecosystem";
	const std::string projectPath = "";
	const std::string resourcesPath = "";
	const std::string prefabsPath = "Prefabs";
	const std::string worldsPath = "Worlds";

	ProjectSettigns(const std::string& projectPath) : projectPath(projectPath), 
		resourcesPath(projectPath + "Resources"), prefabsPath(projectPath + "Prefabs"), worldsPath(projectPath + "Worlds")
	{
	};
};