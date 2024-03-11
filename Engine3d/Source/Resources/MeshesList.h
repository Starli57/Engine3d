#pragma once

#include <array>
#include <string>

enum class MeshId
{
    viking_room = 0,
    formula1 = 1,

    Count //always the last, represents the number of meshes in the list
};

extern std::array<std::string, static_cast<size_t>(MeshId::Count)> meshes;