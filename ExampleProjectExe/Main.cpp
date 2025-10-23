
#include <iostream>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/ProjectSettings.h"
#include "ExampleProject/Game.h"

int main()
{
    Ref<ProjectSettings> projectSettings = CreateRef<ProjectSettings>("");

    try
    {
        const URef<Game> game = CreateUniqueRef<Game>(projectSettings);
        game->engine->engineContext->entitySerializer->InstantiateWorld(
            game->engine->engineContext->ecs,
            game->engine->engineContext->resourcesStorage->worldsPaths.at(0));
        
        game->Run();

    }
    catch (const std::exception& e)
    {
        std::cout << "Critical error: " << e.what();
    }

    return 0;
}
