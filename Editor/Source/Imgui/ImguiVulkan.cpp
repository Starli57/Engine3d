
#include <spdlog/spdlog.h>
#include "ImguiVulkan.h"

ImguiVulkan::ImguiVulkan(AVulkan::VulkanGraphicsApi& vulkanApi) : vulkanApi(vulkanApi)
{
    spdlog::info("Start editor imgui initialization");

    rollback = CreateRef<Rollback>("Editor");

    descriptorPool = vulkanApi.descriptors->CreateDescriptorPool(vulkanApi.logicalDevice);
    queueFamilies = VkUtils::GetQueueFamilies(vulkanApi.physicalDevice, vulkanApi.windowSurface);
    graphicsQueueFamily = queueFamilies.graphicsFamily.value();

    auto commandPool = vulkanApi.commandPool;

    VkRenderPass renderPass;
    CreateRenderPass(renderPass);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;        
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

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
    initInfo.SurfaceFormat = vulkanApi.swapChainData->surfaceFormat;

    ImGui_ImplVulkan_Init(&initInfo);
    ImGui_ImplVulkan_CreateFontsTexture();

    spdlog::info("Editor imgui initialization completed");
}

ImguiVulkan::~ImguiVulkan()
{
    rollback->Dispose();

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    spdlog::info("Finished ImguiVulkan dispose");
}

void ImguiVulkan::Update()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    auto show_another_window = true;
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    ImGui::Render();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
}

void ImguiVulkan::CreateRenderPass(VkRenderPass& renderPass)
{
    VkAttachmentDescription renderPassAttachment = {};
    renderPassAttachment.format = vulkanApi.swapChainData->imageFormat;
    renderPassAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    renderPassAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    renderPassAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    renderPassAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    renderPassAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    renderPassAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    renderPassAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment = {};
    color_attachment.attachment = 0;
    color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment;

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
    auto renderPassStatus = vkCreateRenderPass(vulkanApi.logicalDevice, &renderPassInfo, nullptr, &renderPass);

    CAssert::Check(renderPassStatus == VK_SUCCESS, "Failed to create imgui render pass");
    rollback->Add([this, renderPass] { vkDestroyRenderPass(vulkanApi.logicalDevice, renderPass, nullptr); });
}
