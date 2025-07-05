#pragma once
#include <vector>

#include "EngineCore/Rendering/Vertex.h"

struct ConvertingMeshData
{
    uint16_t meshIndex;
    uint16_t materialIndex;
    
    std::vector<EngineCore::Vertex> vertices;
    std::vector<uint32_t> indices;
    
    std::string materialPath;
    std::string materialName;
    std::string meshName;
    
};
