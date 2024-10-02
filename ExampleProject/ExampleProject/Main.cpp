
#include "Game.h"

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/ProjectSettings.h"

int main()
{
	Ref<ProjectSettigns> projectSettings = CreateRef<ProjectSettigns>(
		"../ExampleProject/"
	);

	try
	{
		URef<Game> game = CreateUniqueRef<Game>(projectSettings);
		game->Run();
	}
	catch (const std::exception& e)
	{
		std::cout << "Critical error: " << e.what();
	}

	return 0;
}