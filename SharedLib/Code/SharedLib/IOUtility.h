#pragma once

#include <fstream>

class IOUtility
{
public:
    std::vector<char> ReadFile(const std::string& filepath) const;
};

