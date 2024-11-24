#include "EngineCore/Pch.h"
#include "IRenderPass.h"

namespace AVulkan
{
    void IRenderPass::WriteDescriptorSet(VkWriteDescriptorSet& writeSet, VkDescriptorSet& descriptorSet, uint32_t dstBinding, uint32_t dstArrayElement,
        uint32_t descriptorCount, VkDescriptorType descriptorType, VkDescriptorImageInfo* pImageInfo, VkDescriptorBufferInfo* pBufferInfo)
    {
        writeSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeSet.dstSet = descriptorSet;
        writeSet.dstBinding = dstBinding;
        writeSet.dstArrayElement = dstArrayElement;
        writeSet.descriptorType = descriptorType;
        writeSet.descriptorCount = descriptorCount;
        writeSet.pImageInfo = pImageInfo;
        writeSet.pBufferInfo = pBufferInfo;
    }
}