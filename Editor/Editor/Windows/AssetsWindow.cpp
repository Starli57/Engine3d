#include "AssetsWindow.h"

AssetsWindow::AssetsWindow(Ref<EngineCore::EntitySerializer> serializer, const Ref<Ecs>& ecs, const Ref<AssetsDatabase>& assetsDatabase, const Ref<ProjectSettings>& projectSettings) :
    serializer(serializer), ecs(ecs), assetsDatabase(assetsDatabase), projectSettings(projectSettings)
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
            serializer->InstantiatePrefab(ecs, prefab.first);
        }
    }

    ImGui::Text("Worlds");
    for (const auto& world : assetsDatabase->worldsIndexByPath)
    {
        if (ImGui::Button(world.first.filename().string().c_str()))
        {
            serializer->InstantiateWorld(ecs, world.first);
        }
    }

    ImGui::Text("Combined Meshes");
    for (const auto& mesh : assetsDatabase->combinedMeshIndexByPath)
    {
        if (ImGui::Button(mesh.first.filename().string().c_str()))
        {
            serializer->InstantiateCombinedMesh(ecs, mesh.first);
        }
    }

    ImGui::End();
}
