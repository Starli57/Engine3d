#include "EngineCore/Pch.h"

#include "Engine.h"

#include "EngineCore/Core/Ref.h"

int main()
{
	Ref<ProjectSettings> projectSettings = CreateRef<ProjectSettings>(
		"../ExampleProject/"
	);

	try
	{
		URef<Engine> engine = CreateUniqueRef<Engine>(projectSettings);
		engine->Run();
	}
	catch (const std::exception& e)
	{
		std::cout << "Critical error: " << e.what();
	}

	return 0;
}