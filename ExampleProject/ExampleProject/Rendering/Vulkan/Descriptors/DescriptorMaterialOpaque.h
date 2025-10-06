#pragma once
#include "EngineCore/EngineContext.h"
#include "EngineCore/Core/ResourcesStorageVulkan.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"

namespace ClientVulkanApi
{
    class DescriptorMaterialOpaque
    {
    public:
        DescriptorMaterialOpaque(Engine::EngineContext* engineContext);
        ~DescriptorMaterialOpaque();
        
        void CreateLayout();
        void CreateDescriptorSets();
        void UpdateDescriptorSets(uint16_t frame) const;

        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorSet& GetDescriptorSet(const uint32_t frame, const uint32_t materialIndex) { return descriptorSets.at(frame).at(materialIndex); }

    private:
        Engine::EngineContext* engineContext;
        VulkanApi::VulkanContext* vulkanContext;
        
        std::vector<VkDescriptorPool> descriptorPools;
        std::vector<std::vector<VkDescriptorSet>> descriptorSets;
        std::vector<Ref<VulkanApi::BufferModel>> materialsUniformBuffers;
    };
}
