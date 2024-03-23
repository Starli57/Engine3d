#pragma once

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

class ImguiVulkan
{
public:

	void RunImgui(GLFWwindow* window, VkInstance& instance, VkPhysicalDevice& physicalDevice, VkDevice& logicalDevice, 
		uint32_t queueFamily, VkQueue& queue, VkPipelineCache& pipelineCache, VkDescriptorPool& descriptorsPool, VkRenderPass& renderPass,
		uint32_t minImageCount, uint32_t imageCount, VkAllocationCallbacks* allocator,
		std::function<void()> updateEditor)
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplGlfw_InitForVulkan(window, true);

		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = instance;
		init_info.PhysicalDevice = physicalDevice;
		init_info.Device = logicalDevice;
		init_info.QueueFamily = queueFamily;
		init_info.Queue = queue;
		init_info.PipelineCache = pipelineCache;
		init_info.DescriptorPool = descriptorsPool;
		init_info.RenderPass = renderPass;
		init_info.Subpass = 0;
		init_info.MinImageCount = minImageCount;
		init_info.ImageCount = imageCount;
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		init_info.Allocator = allocator;
		init_info.CheckVkResultFn = nullptr;
		ImGui_ImplVulkan_Init(&init_info);

		// Allocate a command buffer

		// Record our command buffer
		cb.begin({ vk::CommandBufferUsageFlagBits::eOneTimeSubmit });
		ImGui_ImplVulkan_CreateFontsTexture(cb);
		cb.end();

		// Submit to the GPU
		vk::SubmitInfo si;
		si.commandBufferCount = cb.size();
		si.pCommandBuffers = cb.data();
		vk::Result res = device->getGraphicsQueue().submit(1, &si, {});
		device->getGraphicsQueue().waitIdle(); // Wait for cb is complete
	}
};