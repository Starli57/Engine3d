#pragma once

#include <string>

namespace EngineCore
{
	struct PipelineConfig
	{
		std::string pipelineName;

		std::string vertShaderPath;
		std::string fragShaderPath;

		bool transparent;
	};
}