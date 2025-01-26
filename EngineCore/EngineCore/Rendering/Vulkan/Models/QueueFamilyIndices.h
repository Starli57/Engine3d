#pragma once

#include <GLFW/glfw3.h>
#include <optional>

namespace AVulkan
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentationFamily;

        bool IsComplete() const
        {
            return graphicsFamily.has_value() && presentationFamily.has_value();
        }
    };
}