
#include <spdlog/spdlog.h>

#include "ImguiVulkan.h"

#include "EngineCore/CustomAssert.h"

ImguiVulkan::ImguiVulkan(const Ref<ProjectSettings>& projectSettings, AVulkan::GraphicsApiVulkan& vulkanApi) :
    projectSettings(projectSettings), vulkanApi(vulkanApi)
{
    spdlog::info("Start editor imgui initialization");

    rollback = CreateRef<Engine::Rollback>("Editor");

    queueFamilies = VulkanApi::GetQueueFamilies(vulkanApi.context->physicalDevice, vulkanApi.context->windowSurface);
    graphicsQueueFamily = queueFamilies.graphicsFamily.value();

    CreateRenderPass(renderPass);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiWindowFlags_MenuBar;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;        
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    auto fontPath = projectSettings->fontsPath + "/Roboto-Medium.ttf";
    io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 20);

    DefaultEditorColors(nullptr);

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;

        io.ConfigViewportsNoAutoMerge = true;
    }

    ImGui_ImplGlfw_InitForVulkan(vulkanApi.context->window, true);
    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = vulkanApi.context->instance;
    initInfo.PhysicalDevice = vulkanApi.context->physicalDevice;
    initInfo.Device = vulkanApi.context->logicalDevice;
    initInfo.QueueFamily = graphicsQueueFamily;
    initInfo.Queue = vulkanApi.context->graphicsQueue;
    initInfo.PipelineCache = VK_NULL_HANDLE;
    initInfo.DescriptorPoolSize = 1024;
    initInfo.RenderPass = renderPass;
    initInfo.Subpass = 0;
    initInfo.MinImageCount = 2;
    initInfo.ImageCount = vulkanApi.swapChainData->imagesCount;
    initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

    ImGui_ImplVulkan_Init(&initInfo);
    ImGui_ImplVulkan_CreateFontsTexture();

    spdlog::info("Editor imgui initialization completed");
}

ImguiVulkan::~ImguiVulkan()
{
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    vkDestroyRenderPass(vulkanApi.context->logicalDevice, renderPass, nullptr);

    rollback->Dispose();

    spdlog::info("Finished ImguiVulkan dispose");
}

void ImguiVulkan::Update()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    for (auto window : windows)
        window->Update();

    ImGui::Render();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
}

void ImguiVulkan::CreateRenderPass(VkRenderPass& renderPass) const
{
    VkAttachmentDescription renderPassAttachment = {};
    renderPassAttachment.format = vulkanApi.context->imageFormat;
    renderPassAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    renderPassAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    renderPassAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    renderPassAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    renderPassAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    renderPassAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    renderPassAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

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

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &renderPassAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;
    auto renderPassStatus = vkCreateRenderPass(vulkanApi.context->logicalDevice, &renderPassInfo, nullptr, &renderPass);

    Engine::CAssert::Check(renderPassStatus == VK_SUCCESS, "Failed to create imgui render pass");
}

void ImguiVulkan::DefaultEditorColors(ImGuiStyle* dst) const
{
    ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text]                  = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
    colors[ImGuiCol_TextDisabled]          = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
    colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
    colors[ImGuiCol_Border]                = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
    colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.82f, 0.28f, 0.29f, 0.78f);
    colors[ImGuiCol_FrameBgActive]         = ImVec4(0.82f, 0.28f, 0.29f, 1.00f);
    colors[ImGuiCol_TitleBg]               = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
    colors[ImGuiCol_TitleBgActive]         = ImVec4(0.82f, 0.28f, 0.29f, 1.00f);
    colors[ImGuiCol_MenuBarBg]             = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
    colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.82f, 0.28f, 0.29f, 0.78f);
    colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.82f, 0.28f, 0.29f, 1.00f);
    colors[ImGuiCol_CheckMark]             = ImVec4(0.82f, 0.22f, 0.27f, 1.00f);
    colors[ImGuiCol_SliderGrab]            = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.82f, 0.28f, 0.29f, 1.00f);
    colors[ImGuiCol_Button]                = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    colors[ImGuiCol_ButtonHovered]         = ImVec4(0.82f, 0.28f, 0.29f, 0.86f);
    colors[ImGuiCol_ButtonActive]          = ImVec4(0.82f, 0.28f, 0.29f, 1.00f);
    colors[ImGuiCol_Header]                = ImVec4(0.82f, 0.28f, 0.29f, 0.76f);
    colors[ImGuiCol_HeaderHovered]         = ImVec4(0.82f, 0.28f, 0.29f, 0.86f);
    colors[ImGuiCol_HeaderActive]          = ImVec4(0.82f, 0.28f, 0.29f, 1.00f);
    colors[ImGuiCol_Separator]             = ImVec4(0.20f, 0.22f, 0.25f, 1.00f);
    colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.82f, 0.28f, 0.29f, 0.78f);
    colors[ImGuiCol_SeparatorActive]       = ImVec4(0.82f, 0.28f, 0.29f, 1.00f);
    colors[ImGuiCol_ResizeGrip]            = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
    colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.82f, 0.28f, 0.29f, 0.78f);
    colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.82f, 0.28f, 0.29f, 1.00f);
    colors[ImGuiCol_PlotLines]             = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
    colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.82f, 0.28f, 0.29f, 1.00f);
    colors[ImGuiCol_PlotHistogram]         = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
    colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.82f, 0.28f, 0.29f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.82f, 0.28f, 0.29f, 0.43f);
    colors[ImGuiCol_PopupBg]               = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);
}
