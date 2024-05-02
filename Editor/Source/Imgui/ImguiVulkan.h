#pragma once

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "Rendering/Vulkan/Models/SwapChainData.h"

#include "SharedLib/Ref.h"
#include "SharedLib/Rollback/Rollback.h"

class ImguiVulkan
{
public:

	void RunImgui(GLFWwindow* window, VkInstance& instance, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, 
		uint32_t queueFamily, VkQueue& queue, VkPipelineCache& pipelineCache, VkDescriptorPool& descriptorsPool, VkRenderPass& renderPass,
		uint32_t minImageCount, uint32_t imageCount, VkAllocationCallbacks* allocator, Ref<AVulkan::SwapChainData> swapChainData,
		std::function<void()> updateEditor)
	{
		Ref<Rollback> rollback = CreateRef<Rollback>();

		// 1: create descriptor pool for IMGUI
		// the size of the pool is very oversize, but it's copied from imgui demo itself.
		VkDescriptorPoolSize pool_sizes[] = { 
			{ VK_DESCRIPTOR_TYPE_SAMPLER,				1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,			1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,			1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,	1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,	1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,		1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,		1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,		1000 }};

		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000;
		pool_info.poolSizeCount = (uint32_t)std::size(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;

		VkDescriptorPool imguiPool;
		auto createStatus = vkCreateDescriptorPool(logicalDevice, &pool_info, nullptr, &imguiPool);

		// 2: initialize imgui library
		// this initializes the core structures of imgui
		ImGui::CreateContext();

		// this initializes imgui for Vulkan
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = instance;
		init_info.PhysicalDevice = physicalDevice;
		init_info.Device = logicalDevice;
		init_info.Queue = queue;
		init_info.DescriptorPool = imguiPool;
		init_info.MinImageCount = 3;
		init_info.ImageCount = 3;
		init_info.UseDynamicRendering = true;

		//dynamic rendering parameters for imgui to use
		init_info.PipelineRenderingCreateInfo = { .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO };
		init_info.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
		init_info.PipelineRenderingCreateInfo.pColorAttachmentFormats = &swapChainData->imageFormat;


		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

		ImGui_ImplVulkan_Init(&init_info);

		ImGui_ImplVulkan_CreateFontsTexture();

		rollback->Add([=]() 
		{
			ImGui_ImplVulkan_Shutdown();
			vkDestroyDescriptorPool(logicalDevice, imguiPool, nullptr);
		});
	}
};