
#include <iostream>
#include <execution>
#include <vector>>
#include <future>

#include "EngineCore/Core/Ref.h"
#include "EngineCore/Core/ProjectSettings.h"

#include "ResourcesConverterObj.h"

int main()
{
	try
	{
		const auto inFolder = "../ResourcesConverter/Input/";
		const auto outFolder = "../ExampleProject/Resources/";
		
		std::vector<std::future<void>> conversionTasks;

		for (const auto& entry : std::filesystem::directory_iterator(inFolder)) 
		{
			if (entry.is_directory())
			{
				auto directoryName = entry.path().filename().string();
				conversionTasks.push_back(std::async(std::launch::async, [inFolder, outFolder, directoryName]()
					{
						URef<ResourcesConverterObj> objConverter = CreateUniqueRef<ResourcesConverterObj>();
						objConverter->Convert(inFolder + directoryName + "/", outFolder + directoryName + "/");
					}));
			}
			else
			{
				std::filesystem::copy(entry.path(), outFolder + entry.path().filename().string(), std::filesystem::copy_options::overwrite_existing);
			}
		}

		for (auto& task : conversionTasks) 
		{
			task.get(); 
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "Critical error: " << e.what();
	}

	return 0;
}