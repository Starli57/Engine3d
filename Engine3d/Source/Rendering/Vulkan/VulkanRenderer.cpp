#include "Pch.h"
#include "VulkanRenderer.h"
#include "Utilities/IOUtility.h"

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
	InstanceInterface().CreateInstance(instance);
	rollback->Add([this]() { DisposeInstance(); });
}

void VulkanRenderer::CreateWindowSurface()
{
	windowSurface = WindowsSurfaceInterface().CreateSurface(instance, *window);
	rollback->Add([this]() { DisposeSurface(); });
}

void VulkanRenderer::SelectPhysicalRenderingDevice()
{
	PhysicalDeviceInterface pdInterface;
	physicalDevice = pdInterface.GetBestRenderingDevice(instance, windowSurface);
	pdInterface.PrintDebugInformation(physicalDevice, windowSurface);
}

void VulkanRenderer::CreateLogicalDevice()
{
	logicalDevice = LogicalDeviceInterface().Create(physicalDevice, windowSurface, graphicsQueue, presentationQueue);
	rollback->Add([this]() { DisposeLogicalDevice(); });
}

void VulkanRenderer::CreateSwapChain()
{
	auto queueIndices = PhysicalDeviceInterface().GetQueueFamilies(physicalDevice, windowSurface);
	swapChainData = SwapChainInterface().CreateSwapChain(*window, physicalDevice, logicalDevice, windowSurface, queueIndices);
	rollback->Add([this]() { DisposeSwapChain(); });
}

void VulkanRenderer::CreateSwapChainImageViews()
{
	ImageViewInterface().CreateImageViews(logicalDevice, swapChainData);
	rollback->Add([this]() { DisposeSwapChainImageViews(); });
}

void VulkanRenderer::DisposeInstance()
{
	InstanceInterface().DestroyInstance(instance);
}

void VulkanRenderer::DisposeSurface()
{
	WindowsSurfaceInterface().DestroySurface(instance, windowSurface);
}

void VulkanRenderer::DisposeLogicalDevice()
{
	LogicalDeviceInterface().DestroyDevice(logicalDevice);
}

void VulkanRenderer::DisposeSwapChain()
{
	SwapChainInterface().DestroySwapChain(logicalDevice, swapChainData);
}

void VulkanRenderer::DisposeSwapChainImageViews()
{
	ImageViewInterface().Dispose(logicalDevice, swapChainData);
}

void VulkanRenderer::CreateGraphicsPipeline()
{
	CreateShadersModules();
	rollback->Add([this]() { DisposeShadersModules(); });
}

void VulkanRenderer::CreateShadersModules()
{
	IOUtility utilities;
	ShaderModuleInterface shaderModule;

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
	ShaderModuleInterface shaderModule;
	shaderModule.Destroy(logicalDevice, vertShaderModule);
	shaderModule.Destroy(logicalDevice, fragShaderModule);
}
