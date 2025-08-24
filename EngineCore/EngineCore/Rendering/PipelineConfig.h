#pragma once

#include <string>

namespace Engine
{
	struct PipelineConfig
	{
		std::string pipelineName;

		std::string vertShaderPath;
		std::string fragShaderPath;

		bool transparent;
	};
}