#pragma once

#include <stb_image.h>
#include <fstream>

#include <Architecture/Ref.h>

class IOUtility
{
public:
    std::vector<char> ReadFile(const std::string& filepath) const;
    Ref<stbi_uc> LoadTexture(const std::string& filepath, int* width, int* height, int* channels, int channelsFormat);
};

