
#include "ImguiVulkan.h"

ImguiVulkan::ImguiVulkan(AVulkan::VulkanGraphicsApi& vulkanApi) : vulkanApi(vulkanApi)
{
    rollback = CreateRef<Rollback>();

    descriptorPool = vulkanApi.descriptors->CreateDescriptorPool(vulkanApi.logicalDevice);
    queueFamilies = AVulkan::APhysicalDevice().GetQueueFamilies(vulkanApi.physicalDevice, vulkanApi.windowSurface);
    graphicsQueueFamily = queueFamilies.graphicsFamily.value();

    CreateRenderPass(vulkanApi.logicalDevice, vulkanApi.swapChainData, &renderPass);

    // Create Framebuffer
    // todo: not sure if I need specific framebuffers for imgui or ok to use engine buffers?
    //VkImageView attachment[1];
    //VkFramebufferCreateInfo framebuffereInfo = {};
    //framebuffereInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    //framebuffereInfo.renderPass = wd->RenderPass;
    //framebuffereInfo.attachmentCount = 1;
    //framebuffereInfo.pAttachments = attachment;
    //framebuffereInfo.width = wd->Width;
    //framebuffereInfo.height = wd->Height;
    //framebuffereInfo.layers = 1;
    //for (uint32_t i = 0; i < wd->ImageCount; i++)
    //{
    //    ImGui_ImplVulkanH_Frame* fd = &wd->Frames[i];
    //    attachment[0] = fd->BackbufferView;
    //    auto framebufferStatus = vkCreateFramebuffer(logicalDevice, &framebuffereInfo, nullptr, &fd->Framebuffer);
    //    CAssert::Check(framebufferStatus == VK_SUCCESS, "Failed to create frame buffer");
    //}

    //todo: replace to function create command buffers
    commandPool = AVulkan::ACommandPool().Create(vulkanApi.logicalDevice, vulkanApi.physicalDevice, vulkanApi.windowSurface);
    AVulkan::ACommandBuffer().Allocate(vulkanApi.logicalDevice, commandPool, commandBuffers, vulkanApi.swapChainData->imagesCount);

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
    ImGui_ImplGlfw_InitForVulkan(vulkanApi.window, true);
    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = vulkanApi.instance;
    initInfo.PhysicalDevice = vulkanApi.physicalDevice;
    initInfo.Device = vulkanApi.logicalDevice;
    initInfo.QueueFamily = graphicsQueueFamily;
    initInfo.Queue = vulkanApi.graphicsQueue;
    initInfo.PipelineCache = VK_NULL_HANDLE;
    initInfo.DescriptorPool = descriptorPool;
    initInfo.RenderPass = renderPass;
    initInfo.Subpass = 0;
    initInfo.MinImageCount = 2;
    initInfo.ImageCount = vulkanApi.swapChainData->imagesCount;
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

}

ImguiVulkan::~ImguiVulkan()
{
    rollback->Dispose();

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    vkDestroyDescriptorPool(vulkanApi.logicalDevice, descriptorPool, nullptr);
}

void ImguiVulkan::Update()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    bool show = true;
    ImGui::Begin("Another Window", &show);
    ImGui::Text("Hello from another window!");
    ImGui::End();

    ImGui::Render();

    auto drawData = ImGui::GetDrawData();
    FrameRender(drawData);
    FramePresent();
}

void ImguiVulkan::FrameRender(ImDrawData* draw_data)
{
    auto frame = vulkanApi.GetFrame();
    auto imageIndex = vulkanApi.GetImageIndex();

    VkSemaphore image_acquired_semaphore = vulkanApi.imageAvailableSemaphores[frame];
    VkSemaphore render_complete_semaphore = vulkanApi.renderFinishedSemaphores[frame];

    auto waitFenceStatus = vkWaitForFences(vulkanApi.logicalDevice, 1, &vulkanApi.drawFences[frame], VK_TRUE, UINT64_MAX);
    CAssert::Check(waitFenceStatus == VK_SUCCESS, "Wait fences failed");

    auto resetFenceStatus = vkResetFences(vulkanApi.logicalDevice, 1, &vulkanApi.drawFences[frame]);
    CAssert::Check(resetFenceStatus == VK_SUCCESS, "Fence reset failed");

    {
        auto resetStatus = vkResetCommandPool(vulkanApi.logicalDevice, commandPool, 0);
        CAssert::Check(resetStatus == VK_SUCCESS, "Can't reset vk command pool");
        VkCommandBufferBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        auto beginStatus = vkBeginCommandBuffer(commandBuffers[imageIndex], &info);
        CAssert::Check(beginStatus == VK_SUCCESS, "Can't begin vk command buffer");
    }
    {
        std::array<VkClearValue, 2> clearColors{};
        clearColors[0].color = { 0.015f, 0.015f, 0.04f, 1.0f };
        clearColors[1].depthStencil.depth = 1.0f;

        VkRenderPassBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        info.renderPass = renderPass;
        info.framebuffer = vulkanApi.swapChainData->frameBuffers[imageIndex];
        info.renderArea.extent.width = vulkanApi.swapChainData->extent.width;
        info.renderArea.extent.height = vulkanApi.swapChainData->extent.height;
        info.clearValueCount = 1;
        info.pClearValues = clearColors.data();
        vkCmdBeginRenderPass(commandBuffers[imageIndex], &info, VK_SUBPASS_CONTENTS_INLINE);
    }

    // Record dear imgui primitives into command buffer
    ImGui_ImplVulkan_RenderDrawData(draw_data, commandBuffers[imageIndex]);

    // Submit command buffer
    vkCmdEndRenderPass(commandBuffers[imageIndex]);
    {
        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &image_acquired_semaphore;
        info.pWaitDstStageMask = &wait_stage;
        info.commandBufferCount = 1;
        info.pCommandBuffers = &commandBuffers[imageIndex];
        info.signalSemaphoreCount = 1;
        info.pSignalSemaphores = &render_complete_semaphore;

        auto endCommandStatus = vkEndCommandBuffer(commandBuffers[imageIndex]);
        CAssert::Check(endCommandStatus == VK_SUCCESS, "End command buffer failed");
        auto submitStatus = vkQueueSubmit(vulkanApi.graphicsQueue, 1, &info, vulkanApi.drawFences[frame]);
        CAssert::Check(submitStatus == VK_SUCCESS, "Failed to vkQueueSubmit");
    }
}

void ImguiVulkan::FramePresent()
{
    auto frame = vulkanApi.GetFrame();
    auto imageIndex = vulkanApi.GetImageIndex();

    VkSemaphore render_complete_semaphore = vulkanApi.renderFinishedSemaphores[frame];
    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &render_complete_semaphore;
    info.swapchainCount = 1;
    info.pSwapchains = &vulkanApi.swapChainData->swapChain;
    info.pImageIndices = &imageIndex;
    VkResult presentStatus = vkQueuePresentKHR(vulkanApi.graphicsQueue, &info);
    if (presentStatus == VK_ERROR_OUT_OF_DATE_KHR || presentStatus == VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("Swapchain is out of date");
        return;
    }
    CAssert::Check(presentStatus == VK_SUCCESS, "Failed to vkQueuePresentKHR");
}

void ImguiVulkan::CreateRenderPass(VkDevice& logicalDevice, Ref<AVulkan::SwapChainData> swapChainData, VkRenderPass* renderPass)
{
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

    auto createStatus = vkCreateRenderPass(logicalDevice, &info, nullptr, renderPass);
    CAssert::Check(createStatus == VK_SUCCESS, "Failed to create render pass, status: " + createStatus);
}