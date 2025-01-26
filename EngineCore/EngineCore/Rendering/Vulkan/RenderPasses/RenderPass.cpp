#include "EngineCore/Pch.h"
#include "IRenderPass.h"

namespace AVulkan
{
    void IRenderPass::WriteDescriptorSet(VkWriteDescriptorSet& writeSet, const VkDescriptorSet& descriptorSet, const uint32_t dstBinding, const uint32_t dstArrayElement,
            const uint32_t descriptorCount, const VkDescriptorType descriptorType, const VkDescriptorImageInfo* pImageInfo, const VkDescriptorBufferInfo* pBufferInfo) const
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