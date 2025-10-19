
#include <iostream>

#include "Editor.h"
#include "ExampleProject/Game.h"

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/ProjectSettings.h"

int main()
{
	Ref<ProjectSettings> projectSettings = CreateRef<ProjectSettings>("");

	try
	{
		const URef<Game> game = CreateUniqueRef<Game>(projectSettings);
		const URef<Editor> editor = CreateUniqueRef<Editor>(projectSettings, game->engine);
		game->Run();
	}
	catch (const std::exception& e)
	{
		std::cout << "Critical error: " << e.what();
	}
	return 0;
}