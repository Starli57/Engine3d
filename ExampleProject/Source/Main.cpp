
#include <Engine.h>
#include <iostream>

#include <SharedLib/Ref.h>

int main() 
{
	Ref<ProjectSettigns> projectSettings = CreateRef<ProjectSettigns>(
		"Resources/",
		"Shaders/"
	);

	try
	{
		URef<Engine> engine = CreateUniqueRef<Engine>(projectSettings);
	}
	catch (const std::exception& e)
	{
		std::cout << "Critical error: " << e.what();
	}

	return 0;
}