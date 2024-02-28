#pragma once

#include <fstream>

#include <Architecture/Ref.h>

class IOUtility
{
public:
    std::vector<char> ReadFile(const std::string& filepath) const;
};

