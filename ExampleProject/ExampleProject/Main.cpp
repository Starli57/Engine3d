
#include "Game.h"

#include "EngineCore/Ref.h"
#include "EngineCore/ProjectSettings.h"

int main()
{
	Ref<ProjectSettigns> projectSettings = CreateRef<ProjectSettigns>(
		""
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