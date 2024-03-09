
#include <Engine.h>
#include <iostream>

#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"

int main()
{
	try
	{
		Engine* engine = new Engine();
		engine->Run();
		delete engine;
	}
	catch (const std::exception& e)
	{
		std::cout << "Critical error: " << e.what();
	};
	return 0;
}