#pragma once
#include <string>

struct ProjectSettigns
{
	const std::string projectPath = "";
	const std::string resourcesPath = "";

	ProjectSettigns(const std::string& projectPath) : projectPath(projectPath), resourcesPath(projectPath + "Resources")
	{
	};
};