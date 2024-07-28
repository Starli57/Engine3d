#include "Pch.h"
#include "MaterialVulkan.h"

namespace AVulkan
{
    MaterialVulkan::MaterialVulkan(std::string pipelineId, Ref<AssetsDatabase> assetDatabase, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, 
        Ref<Descriptors> descriptors, VkSampler& textureSampler, VkDescriptorSetLayout& descriptorSetLayout, Ref<Rollback> rollback) :
        Material(pipelineId), assetDatabase(assetDatabase), logicalDevice(logicalDevice), descriptors(descriptors), textureSampler(textureSampler)
    {
        auto uniformBufferBuilder = AUniformBufferVulkan();

        descriptorSets = std::vector<VkDescriptorSet>();
        uboViewProjection = std::vector<Ref<BufferModel>>();
        uboLights = std::vector<Ref<BufferModel>>();

        //todo: replace ubo data to independent component
        for (uint16_t i = 0; i < GraphicsApiVulkan::maxFramesInFlight; i++)
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

    MaterialVulkan::~MaterialVulkan()
    {
        uboLights.clear();
        uboViewProjection.clear();
        descriptorSets.clear();
    }
    
    void MaterialVulkan::UpdateDescriptors(uint16_t frame)
    {
        VkImageView albedoImageView = nullptr;

        if (albedoTexture >= 0)
        {
            auto albedo = static_pointer_cast<AVulkan::TextureVulkan>(assetDatabase->GetTexture(albedoTexture));
            albedoImageView = albedo->imageModel->imageView;
        }

        descriptors->UpdateDescriptorSet(
            logicalDevice, descriptorSets[frame],
            uboViewProjection[frame]->buffer, sizeof(UboViewProjectionComponent),
            uboLights[frame]->buffer, sizeof(UboDiffuseLightComponent),
            albedoImageView, textureSampler);
    }
}