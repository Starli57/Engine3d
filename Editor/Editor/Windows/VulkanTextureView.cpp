#include "VulkanTextureView.h"

VulkanTextureView::VulkanTextureView(Ref<Engine> engine, AVulkan::GraphicsApiVulkan& vulkanApi)
	: engine(engine), vulkanApi(vulkanApi)
{
    auto shadowMap = vulkanApi.GetRenderPassShadowMap();
    auto imageBuffer = shadowMap->GetImageBuffer();
    textureDescriptor = ImGui_ImplVulkan_AddTexture(shadowMap->GetSampler(), imageBuffer->imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void VulkanTextureView::Update()
{
    ImGui::Begin("Shadow Map Preview");

    auto shadowMap = vulkanApi.GetRenderPassShadowMap();
    auto imageBuffer = shadowMap->GetImageBuffer();

    VkDescriptorImageInfo desc_image[1] = {};
    desc_image[0].sampler = shadowMap->GetSampler();
    desc_image[0].imageView = imageBuffer->imageView;
    desc_image[0].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkWriteDescriptorSet write_desc[1] = {};
    write_desc[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write_desc[0].dstSet = textureDescriptor;
    write_desc[0].descriptorCount = 1;
    write_desc[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    write_desc[0].pImageInfo = desc_image;
    vkUpdateDescriptorSets(vulkanApi.logicalDevice, 1, write_desc, 0, nullptr);

    float shadowMapSize = 256.0f;
    ImGui::Image(textureDescriptor, ImVec2(shadowMapSize, shadowMapSize));

    ImGui::End();
}
