#pragma once

#include <array>
#include <string>

enum class TextureId 
{
    GroundDirtWeedsPatchy004_COL_2K = 0,
    GroundGrassGreen002_COL_1K = 1,
    viking_room = 2,
    formula1_Diffuse = 3,

    Count //always the last, represents the number of textures in the list
};

extern std::array<std::string, static_cast<size_t>(TextureId::Count)> textures;