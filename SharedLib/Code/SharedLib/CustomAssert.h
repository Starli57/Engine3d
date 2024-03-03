#pragma once

#include <iostream>
#include <string>

class CAssert
{
public:
    static void Check(const bool condition, const std::string& message)
    {
        if (!condition)
        {
            throw std::runtime_error(message);
        }
    }
};