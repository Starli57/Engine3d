#include "Pch.h"
#include "VulkanMaterial.h"

namespace AVulkan
{
    VulkanMaterial::VulkanMaterial(std::string pipelineId, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, Ref<Descriptors> descriptors,
        VkSampler& textureSampler, VkDescriptorSetLayout& descriptorSetLayout, Ref<Rollback> rollback) :
        Material(pipelineId), logicalDevice(logicalDevice), descriptors(descriptors), textureSampler(textureSampler)
    {
        auto uniformBufferBuilder = AUniformBufferVulkan();

        descriptorSets = std::vector<VkDescriptorSet>();
        uboViewProjection = std::vector<Ref<BufferModel>>();
        uboLights = std::vector<Ref<BufferModel>>();

        //todo: replace ubo data to independent component
        for (uint16_t i = 0; i < VulkanGraphicsApi::maxFramesInFlight; i++)
        {
            auto viewProjectionDescriptorSet = descriptors->AllocateDescriptorSet(logicalDevice, descriptorSetLayout, rollback);
            descriptorSets.push_back(viewProjectionDescriptorSet);

            auto viewProjection = uniformBufferBuilder.Create(logicalDevice, physicalDevice, sizeof(UboViewProjectionComponent), rollback);
            uboViewProjection.push_back(viewProjection);

            auto lightningDescriptorSet = descriptors->AllocateDescriptorSet(logicalDevice, descriptorSetLayout, rollback);
            descriptorSets.push_back(lightningDescriptorSet);

            auto lightning = uniformBufferBuilder.Create(logicalDevice, physicalDevice, sizeof(UboDiffuseLightComponent), rollback);
            uboLights.push_back(lightning);
        }
    }

    VulkanMaterial::~VulkanMaterial()
    {
        uboLights.clear();
        uboViewProjection.clear();
        descriptorSets.clear();
    }
    
    void VulkanMaterial::UpdateDescriptors(uint16_t frame)
    {
        VkImageView albedoImageView = nullptr;

        if (albedoTexture.has_value())
        {
            auto albedo = static_pointer_cast<AVulkan::TextureVulkan>(albedoTexture.value());
            albedoImageView = albedo->imageModel->imageView;
        }

        descriptors->UpdateDescriptorSet(
            logicalDevice, descriptorSets[frame],
            uboViewProjection[frame]->buffer, sizeof(UboViewProjectionComponent),
            uboLights[frame]->buffer, sizeof(UboDiffuseLightComponent),
            albedoImageView, textureSampler);
    }
}