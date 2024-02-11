#include "Pch.h"
#include "ADescriptorLayout.h"

namespace AVulkan 
{
    void ADescriptorLayout::Create(VkDevice& logicalDevice, VkDescriptorSetLayout& descriptorSetLayout) const
    {
        VkDescriptorSetLayoutBinding viewProjectionLayoutBinding{};
        viewProjectionLayoutBinding.binding = 0;
        viewProjectionLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        viewProjectionLayoutBinding.descriptorCount = 1;
        viewProjectionLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding modelLayoutBinding{};
        modelLayoutBinding.binding = 1;
        modelLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        modelLayoutBinding.descriptorCount = 1;
        modelLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        std::vector< VkDescriptorSetLayoutBinding> bindings = { viewProjectionLayoutBinding, modelLayoutBinding };

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        auto createStatus = vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &descriptorSetLayout);
        if (createStatus != VK_SUCCESS) 
        {
            throw std::runtime_error("Failed to create descriptor set layout, status: " + createStatus);
        }
    }

    void ADescriptorLayout::Dispose(VkDevice& logicalDevice, VkDescriptorSetLayout& descriptorSetLayout) const
    {
        vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);
    }
}