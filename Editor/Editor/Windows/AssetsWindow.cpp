#include "AssetsWindow.h"

AssetsWindow::AssetsWindow(Ref<AssetsDatabase> assetsDatabase, Ref<World> world) : assetsDatabase(assetsDatabase), world(world)
{
}

AssetsWindow::~AssetsWindow()
{
}

void AssetsWindow::Update()
{
    ImGui::Begin("Assets");

    for (auto prefab : assetsDatabase->prefabsPaths)
        if (ImGui::Button(prefab.first.c_str()))
            world->InstantiatePrefab(prefab.second);

    for (auto mesh : assetsDatabase->meshesPaths)
        ImGui::Text(mesh.first.c_str());

    for(auto texture : assetsDatabase->texturesPaths)
        ImGui::Text(texture.first.c_str());

    ImGui::End();
}
