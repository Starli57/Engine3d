#pragma once

#include <array>
#include <string>

enum class MeshId
{
    viking_room = 0,

    Count //always the last, represents the number of meshes in the list
};

std::array<std::string, static_cast<size_t>(MeshId::Count)> meshes =
{
    "Meshes/viking_room.obj"
};