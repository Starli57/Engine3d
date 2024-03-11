
#include <Engine.h>
#include <iostream>

#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"

#include "SharedLib/ProjectSettings.h"

int main()
{
	Ref<ProjectSettigns> projectSettings = CreateRef<ProjectSettigns>(
		"../ExampleProject/"
	);

	try
	{
		Engine* engine = new Engine(projectSettings);
		engine->Run();
		delete engine;
	}
	catch (const std::exception& e)
	{
		std::cout << "Critical error: " << e.what();
	};
	return 0;
}