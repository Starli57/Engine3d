#pragma once

#include <array>
#include <string>

enum class TextureId 
{
    GroundDirtWeedsPatchy004_COL_2K = 0,
    GroundGrassGreen002_COL_1K = 1,
    viking_room = 2,

    Count //always the last, represents the number of textures in the list
};

std::array<std::string, static_cast<size_t>(TextureId::Count)> textures =
{
    "Textures/GroundDirtWeedsPatchy004_COL_2K.jpg",
    "Textures/GroundGrassGreen002_COL_1K.jpg",
    "Textures/viking_room.png"
};