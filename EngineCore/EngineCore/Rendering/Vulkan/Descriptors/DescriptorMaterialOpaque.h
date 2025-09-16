#pragma once
#include "EngineCore/Core/Ecs.h"
#include "EngineCore/Core/ResourcesStorageVulkan.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/VulkanContext.h"
#include "EngineCore/Rendering/Vulkan/Models/BufferModel.h"

namespace VulkanApi
{
    class DescriptorMaterialOpaque
    {
    public:
        DescriptorMaterialOpaque(VulkanContext* vulkanContext, const Ref<Ecs>& ecs, VkSampler& textureSampler, const Ref<Engine::ResourcesStorageVulkan>& resourcesStorage);

        ~DescriptorMaterialOpaque();
        void CreateLayout();
        void CreateDescriptorSets();
        void UpdateDescriptorSets(uint16_t frame) const;

        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorSet& GetDescriptorSet(const uint32_t frame, const uint32_t materialIndex) { return descriptorSets.at(frame).at(materialIndex); }

    private:
        Ref<Engine::ResourcesStorageVulkan> resourcesStorage;

        VulkanContext* vulkanContext;
        Ref<Ecs> ecs;
        
        std::vector<VkDescriptorPool> descriptorPools;
        std::vector<std::vector<VkDescriptorSet>> descriptorSets;
        std::vector<Ref<BufferModel>> materialsUniformBuffers;
        VkSampler& textureSampler;
    };
}
