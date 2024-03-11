#pragma once

#include <fstream>

#include "SharedLib/CustomAssert.h"
#include "SharedLib/Ref.h"

class IOUtility
{
public:
    std::vector<char> ReadFile(const std::string& filepath) const;
};

