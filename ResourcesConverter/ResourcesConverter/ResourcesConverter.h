#pragma once
#include <filesystem>
#include <yaml-cpp/yaml.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vertex.h"

class ResourcesConverter
{
public:
	virtual ~ResourcesConverter() = default;
};