
#include <spdlog/spdlog.h>
#include "ImguiVulkan.h"

ImguiVulkan::ImguiVulkan(AVulkan::VulkanGraphicsApi& vulkanApi) : vulkanApi(vulkanApi)
{
    spdlog::info("Start editor imgui initialization");

    rollback = CreateRef<Rollback>("Editor");

    descriptorPool = vulkanApi.descriptors->CreateDescriptorPool(vulkanApi.logicalDevice);
    queueFamilies = AVulkan::APhysicalDevice().GetQueueFamilies(vulkanApi.physicalDevice, vulkanApi.windowSurface);
    graphicsQueueFamily = queueFamilies.graphicsFamily.value();

    auto commandPool = vulkanApi.commandPool;
    auto commandBuffers = &vulkanApi.uiCommandBuffers;
    AVulkan::ACommandBuffer().Allocate(vulkanApi.logicalDevice, commandPool, *commandBuffers, vulkanApi.swapChainData->imagesCount);

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
    initInfo.RenderPass = vulkanApi.renderPass;
    initInfo.Subpass = 0;
    initInfo.MinImageCount = 2;
    initInfo.ImageCount = vulkanApi.swapChainData->imagesCount;
    initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    ImGui_ImplVulkan_Init(&initInfo);
    ImGui_ImplVulkan_CreateFontsTexture();
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

    spdlog::info("Editor imgui initialization completed");
}

ImguiVulkan::~ImguiVulkan()
{
    rollback->Dispose();

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    vkDestroyDescriptorPool(vulkanApi.logicalDevice, descriptorPool, nullptr);
}

void ImguiVulkan::StartFrame()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImguiVulkan::Update()
{
    bool show = true;
    ImGui::Begin("Another Window", &show);
    ImGui::Text("Hello from another window!");
    ImGui::End();
}

void ImguiVulkan::Render()
{
    ImGui::Render();

    auto drawData = ImGui::GetDrawData();
    ImGui_ImplVulkan_RenderDrawData(drawData, vulkanApi.uiCommandBuffers[vulkanApi.GetImageIndex()]);
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
}
