#include "AssetsWindow.h"

AssetsWindow::AssetsWindow(const Ref<Ecs>& ecs, const Ref<AssetsDatabase>& assetsDatabase, const Ref<ProjectSettings>& projectSettings) :
    ecs(ecs), assetsDatabase(assetsDatabase), projectSettings(projectSettings)
{
}

AssetsWindow::~AssetsWindow()
{
}

void AssetsWindow::Update()
{
    ImGui::Begin("Assets");
    
    ImGui::Text("Prefabs");
    for (const auto& prefab : assetsDatabase->prefabsIndexByPath)
    {
        if (ImGui::Button(prefab.first.filename().string().c_str()))
        {
            EntitySerializer entitySerializer = EntitySerializer(projectSettings, assetsDatabase);
            entitySerializer.InstantiatePrefab(ecs, prefab.first);
        }
    }

    ImGui::Text("Worlds");
    for (const auto& world : assetsDatabase->worldsIndexByPath)
    {
        if (ImGui::Button(world.first.filename().string().c_str()))
        {
            EntitySerializer entitySerializer = EntitySerializer(projectSettings, assetsDatabase);
            entitySerializer.InstantiateWorld(ecs, world.first);
        }
    }

    ImGui::Text("Combined Meshes");
    for (const auto& mesh : assetsDatabase->combinedMeshIndexByPath)
    {
        if (ImGui::Button(mesh.first.filename().string().c_str()))
        {
            EntitySerializer entitySerializer = EntitySerializer(projectSettings, assetsDatabase);
            entitySerializer.InstantiateCombinedMesh(ecs, mesh.first);
        }
    }

    /*
     for (auto mesh : assetsDatabase->meshesPaths)
         ImGui::Text(mesh.first.c_str());
    
     for(auto texture : assetsDatabase->texturesPaths)
         ImGui::Text(texture.first.c_str());
    */

    ImGui::End();
}
