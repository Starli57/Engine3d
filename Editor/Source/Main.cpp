
#include <Engine.h>
#include <iostream>

#include "Editor.h"

#include "SharedLib/Ref.h"
#include "SharedLib/ProjectSettings.h"

int main()
{
	Ref<ProjectSettigns> projectSettings = CreateRef<ProjectSettigns>(
		"../ExampleProject/"
	);

	Ref<Editor> editor = CreateRef<Editor>();

    return 0;

	/*try
	{
		Engine* engine = new Engine(projectSettings);
		engine->Run();
		delete engine;
	}
	catch (const std::exception& e)
	{
		std::cout << "Critical error: " << e.what();
	};
	return 0;*/
}

