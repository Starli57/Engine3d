#pragma once

#include <optional>

namespace VulkanApi
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