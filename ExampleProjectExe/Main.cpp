
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
        game->Run();
    }
    catch (const std::exception& e)
    {
        std::cout << "Critical error: " << e.what();
    }

    return 0;
}
