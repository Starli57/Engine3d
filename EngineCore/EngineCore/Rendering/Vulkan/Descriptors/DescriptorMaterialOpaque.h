#pragma once
#include "IDescriptor.h"
#include "EngineCore/Core/ResourcesStorageVulkan.h"
#include "EngineCore/Core/Ref.h"
#include "EngineCore/Rendering/Vulkan/Models/BufferModel.h"

namespace VulkanApi
{
    class DescriptorMaterialOpaque : public IDescriptor
    {
    public:
        DescriptorMaterialOpaque(VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, const Ref<Ecs>& ecs,
            const Ref<DescriptorsAllocator>& descriptorsAllocator, VkSampler& textureSampler, const Ref<Engine::ResourcesStorageVulkan>& resourcesStorage);

        ~DescriptorMaterialOpaque() override;
        void CreateLayout() override;
        void CreateDescriptorSets() override;
        void UpdateDescriptorSets(uint16_t frame) const;

        VkDescriptorSet& GetDescriptorSet(const uint32_t frame, const uint32_t materialIndex) { return descriptorSets.at(frame).at(materialIndex); }

    private:
        Ref<Engine::ResourcesStorageVulkan> resourcesStorage;
        
        std::vector<VkDescriptorPool> descriptorPools;
        std::vector<std::vector<VkDescriptorSet>> descriptorSets;
        std::vector<Ref<BufferModel>> materialsUniformBuffers;
        VkSampler& textureSampler;
    };
}
