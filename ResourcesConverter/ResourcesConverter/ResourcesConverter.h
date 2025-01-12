#include <filesystem>
#include <yaml-cpp/yaml.h>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vertex.h"
#include "EngineCore/Assets/Meta/MeshMeta.h"

class ResourcesConverter
{
public:
	virtual ~ResourcesConverter() = default;
	virtual void Convert(const std::string& inFolder, const std::string& outFolder) = 0;
};