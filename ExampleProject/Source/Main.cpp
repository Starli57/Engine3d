
#include <Engine.h>
#include <iostream>

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
	}

	return 0;
}