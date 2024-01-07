#include "Pch.h"
#include "VulkanRenderer.h"
#include "Utilities/IOUtility.h"

namespace AVulkan
{
	VulkanRenderer::VulkanRenderer(GLFWwindow* glfwWindow, Rollback* vulkanRollback)
	{
		rollback = vulkanRollback;
		window = glfwWindow;
	}

	VulkanRenderer::~VulkanRenderer()
	{
		rollback->Dispose();
	}

	void VulkanRenderer::Initialize()
	{
		try
		{
			CreateInstance();
			CreateWindowSurface();
			SelectPhysicalRenderingDevice();
			CreateLogicalDevice();
			CreateSwapChain();
			CreateSwapChainImageViews();
			CreateGraphicsPipeline();
		}
		catch (const std::exception& e)
		{
			spdlog::critical(e.what());
			rollback->Dispose();
			throw e;
		}
	}

	void VulkanRenderer::CreateInstance()
	{
		AInstance().Create(instance);
		rollback->Add([this]() { DisposeInstance(); });
	}

	void VulkanRenderer::CreateWindowSurface()
	{
		windowSurface = AWindowsSurface().Create(instance, *window);
		rollback->Add([this]() { DisposeSurface(); });
	}

	void VulkanRenderer::SelectPhysicalRenderingDevice()
	{
		APhysicalDevice pdInterface;
		physicalDevice = pdInterface.GetBestRenderingDevice(instance, windowSurface);
		pdInterface.PrintDebugInformation(physicalDevice, windowSurface);
	}

	void VulkanRenderer::CreateLogicalDevice()
	{
		logicalDevice = ALogicalDevice().Create(physicalDevice, windowSurface, graphicsQueue, presentationQueue);
		rollback->Add([this]() { DisposeLogicalDevice(); });
	}

	void VulkanRenderer::CreateSwapChain()
	{
		auto queueIndices = APhysicalDevice().GetQueueFamilies(physicalDevice, windowSurface);
		swapChainData = ASwapChain().Create(*window, physicalDevice, logicalDevice, windowSurface, queueIndices);
		rollback->Add([this]() { DisposeSwapChain(); });
	}

	void VulkanRenderer::CreateSwapChainImageViews()
	{
		AImageView().Create(logicalDevice, swapChainData);
		rollback->Add([this]() { DisposeSwapChainImageViews(); });
	}

	void VulkanRenderer::DisposeInstance()
	{
		AInstance().Dispose(instance);
	}

	void VulkanRenderer::DisposeSurface()
	{
		AWindowsSurface().Dispose(instance, windowSurface);
	}

	void VulkanRenderer::DisposeLogicalDevice()
	{
		ALogicalDevice().Dispose(logicalDevice);
	}

	void VulkanRenderer::DisposeSwapChain()
	{
		ASwapChain().Dispose(logicalDevice, swapChainData);
	}

	void VulkanRenderer::DisposeSwapChainImageViews()
	{
		AImageView().Dispose(logicalDevice, swapChainData);
	}

	void VulkanRenderer::CreateGraphicsPipeline()
	{
		CreateShadersModules();
		rollback->Add([this]() { DisposeShadersModules(); });
	}

	void VulkanRenderer::CreateShadersModules()
	{
		IOUtility utilities;
		AShaderModule shaderModule;

		spdlog::info("Create vert shader module");
		const std::vector<char>& vertShaderCode = utilities.ReadFile("../Engine3d/Source/Rendering/Shaders/vert.spv");
		vertShaderModule = shaderModule.Create(logicalDevice, vertShaderCode);

		spdlog::info("Create frag shader module");
		const std::vector<char>& fragShaderCode = utilities.ReadFile("../Engine3d/Source/Rendering/Shaders/frag.spv");
		fragShaderModule = shaderModule.Create(logicalDevice, fragShaderCode);

		VkPipelineShaderStageCreateInfo vertInfo{};
		vertInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertInfo.module = vertShaderModule;
		vertInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragInfo{};
		fragInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragInfo.module = fragShaderModule;
		fragInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertInfo, fragInfo };

	}

	void VulkanRenderer::DisposeShadersModules()
	{
		AShaderModule shaderModule;
		shaderModule.Dispose(logicalDevice, vertShaderModule);
		shaderModule.Dispose(logicalDevice, fragShaderModule);
	}
}