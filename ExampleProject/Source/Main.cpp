
#include <Engine.h>

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

	}

	return 0;
}