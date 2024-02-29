#pragma once

#include <array>
#include <string>

enum class TextureId 
{
    GroundDirtWeedsPatchy004_COL_2K = 0,
    GroundDirtWeedsPatchy004_NRM_2K = 1,

    Count //always the last, represent number of textures in the list
};

std::array<std::string, static_cast<size_t>(TextureId::Count)> textures =
{
    "Textures/GroundDirtWeedsPatchy004_COL_2K.jpg",
    "Textures/GroundDirtWeedsPatchy004_NRM_2K.jpg" 
};