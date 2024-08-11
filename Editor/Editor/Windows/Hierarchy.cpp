
#include "Hierarchy.h"

void Hierarchy::Update()
{
    ImGui::Begin("Entities");

    auto view = ecs->registry->view<NameComponent, IdComponent>();

    for (int i = 0; i < ecs->allEntities.size(); i++)
    {
        auto entityRef = ecs->allEntities[i];
        std::string entityName = "Entity";

        if (entityRef->HasComponent<NameComponent>())
        {
            auto nameComponent = view.get<NameComponent>(entityRef->GetEntity());
            entityName = nameComponent.name;
        }

        if (entityRef->HasComponent<IdComponent>())
        {
            auto id = view.get<IdComponent>(entityRef->GetEntity()).id;
            entityName = std::format("{}_{}", entityName, id);
        }

        if (selectedItemIndex == i) 
        {
            auto doSave = ImGui::Button("S");
            ImGui::SetItemTooltip("Save changes");

            ImGui::SameLine();
            auto doDelete = ImGui::Button("X");
            ImGui::SetItemTooltip("Delete entity");

            if (doSave)
            {
                auto filePath = projectSettings->prefabsPath + "/" + entityName + ".yaml";
                serializer->SerializePrefab(entityRef, filePath);
            }

            if (doDelete)
            {
                ecs->DestroyEntiy(entityRef);
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

    ImGui::End();
}

Hierarchy::Hierarchy(Ref<Ecs> ecs, Ref<Inspector> inspector, Ref<ProjectSettigns> projectSettings)
    : ecs(ecs), inspector(inspector), projectSettings(projectSettings)
{
    serializer = CreateRef<EntitySerializer>(projectSettings);
}

Hierarchy::~Hierarchy()
{
}
