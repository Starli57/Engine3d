
#include "Game.h"

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/ProjectSettings.h"

int main()
{
	Ref<ProjectSettings> projectSettings = CreateRef<ProjectSettings>(
		"../ExampleProject/",
		"../Externals/"
	);

	try
	{
		const URef<Game> game = CreateUniqueRef<Game>(projectSettings);
		game->Run();
	}
	catch (const std::exception& e)
	{
		std::cout << "Critical error: " << e.what();
	}

	int a;
	std::cin >> a;
	return 0;
}