#pragma once

#include <fstream>

#include "EngineShared/CustomAssert.h"
#include "EngineShared/Ref.h"

class IOUtility
{
public:
    std::vector<char> ReadFile(const std::string& filepath) const;
};

