
#include "Hierarchy.h"

void Hierarchy::Update()
{
    ImGui::Begin("Entities");

    const auto view = ecs->registry->view<NameComponent, IdComponent>();

    for (int i = 0; i < ecs->allEntities.size(); i++)
    {
        auto entityRef = ecs->allEntities[i];
        std::string entityName = "Entity";

        if (entityRef->HasComponent<NameComponent>())
        {
            const auto nameComponent = view.get<NameComponent>(entityRef->GetEntity());
            entityName = nameComponent.name;
        }

        if (entityRef->HasComponent<IdComponent>())
        {
            auto id = view.get<IdComponent>(entityRef->GetEntity()).id;
            entityName = std::format("{}_{}", entityName, id);
        }

        if (selectedItemIndex == i) 
        {
            const auto doSave = ImGui::Button("S");
            ImGui::SetItemTooltip("Save changes");

            ImGui::SameLine();
            const auto doDelete = ImGui::Button("X");
            ImGui::SetItemTooltip("Delete entity");

            if (doSave)
            {
                auto filePath = projectSettings->prefabsPath + "/" + entityName + ".yaml";
                serializer->SerializePrefab(entityRef, filePath);
            }

            if (doDelete)
            {
                ecs->DestroyEntity(entityRef);
            }

            ImGui::SameLine();
        }

        if (ImGui::Selectable(entityName.c_str(), selectedItemIndex == i ))
        {
            selectedItemIndex = i;
            inspector->Observe(ecs->allEntities[i]);
        }
    }

    if (ImGui::Button("Create entity"))
        ecs->CreateEntity();

    constexpr int worldNameLength = 32;
    static char worldName[worldNameLength] = "main"; 
    ImGui::InputText("WorldName", worldName, worldNameLength);

    if (ImGui::Button("Save world"))
        serializer->SerializeWorld(ecs, projectSettings->worldsPath + "/" + worldName + ".world");

    ImGui::End();
}

Hierarchy::Hierarchy(const Ref<Engine::EntitySerializer>& serializer, const Ref<Ecs>& ecs, const Ref<Inspector>& inspector,
                     const Ref<ProjectSettings>& projectSettings, const Ref<Engine::ResourcesStorageVulkan>& resourcesStorage)
    : serializer(serializer), ecs(ecs), inspector(inspector), projectSettings(projectSettings), selectedItemIndex(0)
{
}

Hierarchy::~Hierarchy()
{
}
