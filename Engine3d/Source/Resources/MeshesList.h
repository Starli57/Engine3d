#pragma once

#include <array>
#include <string>

enum class MeshId
{
    viking_room = 0,
    formula1 = 1,

    Count //always the last, represents the number of meshes in the list
};

std::array<std::string, static_cast<size_t>(MeshId::Count)> meshes =
{
    "Resources/Meshes/viking_room.obj",
    "Resources/Meshes/Formula_1_mesh.obj"
};