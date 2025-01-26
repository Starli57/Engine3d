#include "VulkanTextureView.h"

VulkanTextureView::VulkanTextureView(const Ref<Engine>& engine, AVulkan::GraphicsApiVulkan& vulkanApi)
	: engine(engine), vulkanApi(vulkanApi)
{
    const auto shadowMap = vulkanApi.GetRenderPassShadowMap();
    const auto imageBuffer = shadowMap->GetImageBuffer();
    textureDescriptor = ImGui_ImplVulkan_AddTexture(shadowMap->GetSampler(), imageBuffer->imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void VulkanTextureView::Update()
{
    ImGui::Begin("Shadow Map Preview");

    auto shadowMap = vulkanApi.GetRenderPassShadowMap();
    auto imageBuffer = shadowMap->GetImageBuffer();

    VkDescriptorImageInfo descImage[1] = {};
    descImage[0].sampler = shadowMap->GetSampler();
    descImage[0].imageView = imageBuffer->imageView;
    descImage[0].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkWriteDescriptorSet writeDesc[1] = {};
    writeDesc[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDesc[0].dstSet = textureDescriptor;
    writeDesc[0].descriptorCount = 1;
    writeDesc[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDesc[0].pImageInfo = descImage;
    vkUpdateDescriptorSets(vulkanApi.logicalDevice, 1, writeDesc, 0, nullptr);

    float shadowMapSize = 256.0f;
    ImGui::Image(textureDescriptor, ImVec2(shadowMapSize, shadowMapSize));

    ImGui::End();
}
