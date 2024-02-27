#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "Architecture/CustomAssert.h"
#include "Architecture/Ref.h"
#include "Rendering/Vulkan/Models/DepthBufferModel.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "Rendering/Vulkan/GraphicsPipeline.h"

namespace AVulkan
{
	//todo: simplify, reduce code duplications or automate generation of getters and setters
	struct VulkanContext
	{
	public:
		Ref<VkInstance> GetVkInstance()
		{
			CAssert::Check(!vkInstance, "VkInstance is null");
			return vkInstance;
		};

		Ref<VkPhysicalDevice> GetPhysicalDevice()
		{
			CAssert::Check(!vkPhysicalDevice, "VkPhysicalDevice is null");
			return vkPhysicalDevice;
		};

		Ref<VkDevice> GetVkLogicalDevice()
		{
			CAssert::Check(!vkLogicalDevice, "VkDevice is null");
			return vkLogicalDevice;
		};
		
		Ref<VkSurfaceKHR> GetVkWindowSurface()
		{
			CAssert::Check(!vkWindowSurface, "VkSurfaceKHR is null");
			return vkWindowSurface;
		};
		
		Ref<VkQueue> GetVkGraphicsQueue()
		{
			CAssert::Check(!vkGraphicsQueue, "VkGraphicsQueue is null");
			return vkGraphicsQueue;
		};
		
		Ref<VkQueue> GetVkPresentationQueue()
		{
			CAssert::Check(!vkPresentationQueue, "VkPresentationQueue is null");
			return vkPresentationQueue;
		};
		
		Ref<VkRenderPass> GetVkRenderPass()
		{
			CAssert::Check(!vkRenderPass, "VkRenderPass is null");
			return vkRenderPass;
		};
		
		Ref<VkCommandPool> GetVkCommandPool()
		{
			CAssert::Check(!vkCommandPool, "VkCommandPool is null");
			return vkCommandPool;
		};
		
		Ref<VkDescriptorSetLayout> GetVkDescriptorSetLayout()
		{
			CAssert::Check(!vkDescriptorSetLayout, "VkDescriptorSetLayout is null");
			return vkDescriptorSetLayout;
		};
		
		Ref<VkDescriptorPool> GetVkDescriptorPool()
		{
			CAssert::Check(!vkDescriptorPool, "VkDescriptorPool is null");
			return vkDescriptorPool;
		};

		Ref<SwapChainData> GetSwapChainData()
		{
			CAssert::Check(!swapChainData, "SwapchainData is null");
			return swapChainData;
		};

		Ref<DepthBufferModel> GetDepthBufferModel()
		{
			CAssert::Check(!depthBufferModel, "DepthBufferModel is null");
			return depthBufferModel;
		};

		Ref<GraphicsPipeline> GetGraphicsPipeline()
		{
			CAssert::Check(!graphicsPipeline, "GraphicsPipeline is null");
			return graphicsPipeline;
		};
		
		void SetVkInstance(Ref<VkInstance> vkInstance)
		{
			CAssert::Check(this->vkInstance != nullptr, "VkInstance has value");
			this->vkInstance = vkInstance;
		};
		
		void SetVkPhysicalDevice(Ref<VkPhysicalDevice> vkPhysicalDevice)
		{
			CAssert::Check(this->vkPhysicalDevice != nullptr, "VkPhysicalDevice has value");
			this->vkPhysicalDevice = vkPhysicalDevice;
		};
		
		void SetVkLogicalDevice(Ref<VkDevice> vkLogicalDevice)
		{
			CAssert::Check(this->vkLogicalDevice != nullptr, "VkLogicalDevice has value");
			this->vkLogicalDevice = vkLogicalDevice;
		};
		
		void SetVkWindowSurface(Ref<VkSurfaceKHR> vkWindowSurface)
		{
			CAssert::Check(this->vkWindowSurface != nullptr, "VkWindowSurface has value");
			this->vkWindowSurface = vkWindowSurface;
		};
		
		void SetVkGraphicsQueue(Ref<VkQueue> vkGraphicsQueue)
		{
			CAssert::Check(this->vkGraphicsQueue != nullptr, "VkGraphicsQueue has value");
			this->vkGraphicsQueue = vkGraphicsQueue;
		};
		
		void SetVkPresentationQueue(Ref<VkQueue> vkPresentationQueue)
		{
			CAssert::Check(this->vkPresentationQueue != nullptr, "VkPresentationQueue has value");
			this->vkPresentationQueue = vkPresentationQueue;
		};
		
		void SetVkRenderPass(Ref<VkRenderPass> vkRenderPass)
		{
			CAssert::Check(this->vkRenderPass != nullptr, "VkRenderPass has value");
			this->vkRenderPass = vkRenderPass;
		};
		
		void SetVkCommandPool(Ref<VkCommandPool> vkCommandPool)
		{
			CAssert::Check(this->vkCommandPool != nullptr, "VkCommandPool has value");
			this->vkCommandPool = vkCommandPool;
		};
		
		void SetVkDescriptorSetLayout(Ref<VkDescriptorSetLayout> vkDescriptorSetLayout)
		{
			CAssert::Check(this->vkDescriptorSetLayout != nullptr, "VkDescriptorSetLayout has value");
			this->vkDescriptorSetLayout = vkDescriptorSetLayout;
		};
		
		void SetVkDescriptorPool(Ref<VkDescriptorPool> vkDescriptorPool)
		{
			CAssert::Check(this->vkDescriptorPool != nullptr, "VkDescriptorPool has value");
			this->vkDescriptorPool = vkDescriptorPool;
		};

		void SetSwapChainData(Ref<SwapChainData> swapChainData)
		{
			CAssert::Check(this->swapChainData != nullptr, "SwapchainData has value");
			this->swapChainData = swapChainData;
		};

		void SetDepthBufferModel(Ref<DepthBufferModel> depthBufferModel)
		{
			CAssert::Check(this->depthBufferModel != nullptr, "DepthBufferModel has value");
			this->depthBufferModel = depthBufferModel;
		};

		void SetGraphicsPipeline(Ref<GraphicsPipeline> graphicsPipeline)
		{
			CAssert::Check(this->graphicsPipeline != nullptr, "GraphicsPipeline has value");
			this->graphicsPipeline = graphicsPipeline;
		};

		uint16_t GetFrame() 
		{
			return frame;
		}

		void IncrementFrame()
		{
			frame = (frame + 1) % maxFramesDraws;
		}

		bool GetNeedToResizeValue() 
		{
			return needResizeWindow;
		}
		
		void SetWindowResizeFlag()
		{
			needResizeWindow = true;
		}

		void ResetWindowResizeFlag() 
		{
			needResizeWindow = false;
		}

		//todo: replace
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> drawFences;

		//todo: replace
		uint16_t const maxFramesDraws = 2;
		uint64_t const frameSyncTimeout = UINT64_MAX;//todo: setup real timeout

	private:
		Ref<VkInstance> vkInstance;
		Ref<VkPhysicalDevice> vkPhysicalDevice;
		Ref<VkDevice> vkLogicalDevice;
		Ref<VkSurfaceKHR> vkWindowSurface;
		Ref<VkQueue> vkGraphicsQueue;
		Ref<VkQueue> vkPresentationQueue;
		Ref<VkRenderPass> vkRenderPass;
		Ref<VkCommandPool> vkCommandPool;
		Ref<VkDescriptorSetLayout> vkDescriptorSetLayout;
		Ref<VkDescriptorPool> vkDescriptorPool;

		Ref<SwapChainData> swapChainData;
		Ref<DepthBufferModel> depthBufferModel;
		Ref<GraphicsPipeline> graphicsPipeline;

		//todo: replace
		uint16_t frame = 0;

		//todo: replace
		bool needResizeWindow = false;
	};
}