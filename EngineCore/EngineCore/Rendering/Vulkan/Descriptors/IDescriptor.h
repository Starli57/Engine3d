#pragma once
#include "GLFW/glfw3.h"
#include "DescriptorsAllocator.h"
#include "EngineCore/Core/Ecs.h"

namespace AVulkan
{
    class IDescriptor
    {
    public:
        IDescriptor(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const Ref<Ecs>& ecs, const Ref<DescriptorsAllocator>& descriptorsManager)
            : physicalDevice(physicalDevice), logicalDevice(logicalDevice), ecs(ecs), descriptorsAllocator(descriptorsManager)
        {
        }

        virtual ~IDescriptor() = default;

        virtual void CreateLayout() = 0;
        virtual void CreateDescriptorSets() = 0;
        
        VkDescriptorSetLayout& DescriptorSetLayout() { return descriptorSetLayout; }
        
    protected:
        VkPhysicalDevice& physicalDevice;
        VkDevice& logicalDevice;
        Ref<Ecs> ecs;
        Ref<DescriptorsAllocator> descriptorsAllocator;
        VkDescriptorSetLayout descriptorSetLayout;
    };
}
