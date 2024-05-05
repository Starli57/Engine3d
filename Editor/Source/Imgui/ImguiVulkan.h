#pragma once

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "Rendering/Vulkan/VulkanGraphicsApi.h"
#include "Rendering/Vulkan/Descriptors.h"
#include "Rendering/Vulkan/Models/SwapChainData.h"
#include "Rendering/Vulkan/Builders/APhysicalDevice.h"
#include "Rendering/Vulkan/Builders/ACommandPool.h"
#include "Rendering/Vulkan/Builders/ACommandBuffer.h"

#include "SharedLib/Ref.h"
#include "SharedLib/Rollback/Rollback.h"

class ImguiVulkan
{
public:

    static void FrameRender(VkDevice& logicalDevice, VkQueue& graphicsQueue, uint32_t imageIndex, ImDrawData* draw_data)
    {
        VkSemaphore image_acquired_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
        VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
       
        ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
        {
            auto waitFenceStatus = vkWaitForFences(logicalDevice, 1, &fd->Fence, VK_TRUE, UINT64_MAX);
            CAssert::Check(waitFenceStatus == VK_SUCCESS, "Wait fences failed");

            auto resetFenceStatus = vkResetFences(logicalDevice, 1, &fd->Fence);
            CAssert::Check(resetFenceStatus == VK_SUCCESS, "Fence reset failed");
        }
        {
            auto resetStatus = vkResetCommandPool(logicalDevice, fd->CommandPool, 0);
            CAssert::Check(resetStatus == VK_SUCCESS, "Can't reset vk command pool");
            VkCommandBufferBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            auto beginStatus = vkBeginCommandBuffer(fd->CommandBuffer, &info);
            CAssert::Check(beginStatus == VK_SUCCESS, "Can't begin vk command buffer");
        }
        {
            VkRenderPassBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            info.renderPass = wd->RenderPass;
            info.framebuffer = fd->Framebuffer;
            info.renderArea.extent.width = wd->Width;
            info.renderArea.extent.height = wd->Height;
            info.clearValueCount = 1;
            info.pClearValues = &wd->ClearValue;
            vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
        }

        // Record dear imgui primitives into command buffer
        ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

        // Submit command buffer
        vkCmdEndRenderPass(fd->CommandBuffer);
        {
            VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            VkSubmitInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            info.waitSemaphoreCount = 1;
            info.pWaitSemaphores = &image_acquired_semaphore;
            info.pWaitDstStageMask = &wait_stage;
            info.commandBufferCount = 1;
            info.pCommandBuffers = &fd->CommandBuffer;
            info.signalSemaphoreCount = 1;
            info.pSignalSemaphores = &render_complete_semaphore;

            auto endCommandStatus = vkEndCommandBuffer(fd->CommandBuffer);
            CAssert::Check(endCommandStatus == VK_SUCCESS, "End command buffer failed");
            auto submitStatus = vkQueueSubmit(graphicsQueue, 1, &info, fd->Fence);
            CAssert::Check(submitStatus == VK_SUCCESS, "Failed to vkQueueSubmit");
        }
    }

    static void FramePresent(VkQueue& graphicsQueue)
    {
        VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
        VkPresentInfoKHR info = {};
        info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &render_complete_semaphore;
        info.swapchainCount = 1;
        info.pSwapchains = &wd->Swapchain;
        info.pImageIndices = &wd->FrameIndex;
        VkResult presentStatus = vkQueuePresentKHR(graphicsQueue, &info);
        if (presentStatus == VK_ERROR_OUT_OF_DATE_KHR || presentStatus == VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("Swapchain is out of date");
            return;
        }
        CAssert::Check(presentStatus == VK_SUCCESS, "Failed to vkQueuePresentKHR");
        wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->SemaphoreCount; // Now we can use the next set of semaphores
    }

	void RunImgui(AVulkan::VulkanGraphicsApi& vulkanApi)
	{
		Ref<Rollback> rollback = CreateRef<Rollback>();

        auto window = vulkanApi.GetWindow();
        auto instance = vulkanApi.GetInstance();
        auto surface = vulkanApi.GetWindowSurface();
        auto physicalDevice = vulkanApi.GetPhysicalDevice();
        auto logicalDevice = vulkanApi.GetLogicalDevice();
        auto graphicsQueue = vulkanApi.GetGraphicsQueue();
        auto graphicsPipeline = vulkanApi.GetGraphicsPipeline();
        auto swapChainData = vulkanApi.GetSwapChainData();
        auto descriptorPool = vulkanApi.GetDescriptors()->CreateDescriptorPool(logicalDevice);
        auto queueFamilies = AVulkan::APhysicalDevice().GetQueueFamilies(physicalDevice, surface);
        auto graphicsQueueFamily = queueFamilies.graphicsFamily.value();

        //todo: replace to function create render pass
        VkAttachmentDescription attachment = {};
        attachment.format = swapChainData->imageFormat;
        attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD; 
        attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; 
        attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; 
        attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; 

        VkAttachmentReference colorAttachment = {}; 
        colorAttachment.attachment = 0;
        colorAttachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {}; 
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; 
        subpass.colorAttachmentCount = 1; 
        subpass.pColorAttachments = &colorAttachment;

        VkSubpassDependency dependency = {}; 
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL; 
        dependency.dstSubpass = 0; 
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; 
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; 
        dependency.srcAccessMask = 0; 
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT; 

        VkRenderPassCreateInfo info = {}; 
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO; 
        info.attachmentCount = 1; 
        info.pAttachments = &attachment; 
        info.subpassCount = 1; 
        info.pSubpasses = &subpass; 
        info.dependencyCount = 1; 
        info.pDependencies = &dependency;
        VkRenderPass renderPass = nullptr; 
        auto createStatus = vkCreateRenderPass(logicalDevice, &info, nullptr, &renderPass);
        CAssert::Check(createStatus == VK_SUCCESS, "Failed to create render pass, status: " + createStatus);
        
        //todo: replace to function create command buffers
        std::vector<VkCommandBuffer> commandBuffers;
        VkCommandPool commandPool = AVulkan::ACommandPool().Create(logicalDevice, physicalDevice, surface);
        AVulkan::ACommandBuffer().Allocate(logicalDevice, commandPool, commandBuffers, swapChainData->imagesCount);

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance = instance;
        initInfo.PhysicalDevice = physicalDevice;
        initInfo.Device = logicalDevice;
        initInfo.QueueFamily = graphicsQueueFamily;
        initInfo.Queue = graphicsQueue;
        initInfo.PipelineCache = VK_NULL_HANDLE;
        initInfo.DescriptorPool = descriptorPool;
        initInfo.RenderPass = renderPass;
        initInfo.Subpass = 0;
        initInfo.MinImageCount = 2;
        initInfo.ImageCount = swapChainData->imagesCount;
        initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        ImGui_ImplVulkan_Init(&initInfo);

        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        //io.Fonts->AddFontDefault();
        //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
        //IM_ASSERT(font != nullptr);

        // Main loop
        while (!glfwWindowShouldClose(window))
        {
            // Start the Dear ImGui frame
            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            bool show = true;
            ImGui::Begin("Another Window", &show);
            ImGui::Text("Hello from another window!");
            ImGui::End();
            
            ImGui::Render();

            ImDrawData* main_draw_data = ImGui::GetDrawData();
            FrameRender(logicalDevice, graphicsQueue, 0, main_draw_data);
            FramePresent(graphicsQueue);
        }

        // Cleanup
        vkDeviceWaitIdle(logicalDevice);

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
	}
};