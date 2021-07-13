#include "resource.h"

#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;
constexpr auto CurrentDirectory = L"../x64/Debug/plugins/";

//just to try filesystem
std::wstring DisplayPathInfo()
{
	// This path may or may not refer to an existing file. We are
	// examining this path string, not file system objects.
	fs::path pathToDisplay(CurrentDirectory);

	std::wostringstream wos;
	int i = 0;
	wos << L"Displaying path info for: " << pathToDisplay << std::endl;
	for (fs::path::iterator itr = pathToDisplay.begin(); itr != pathToDisplay.end(); ++itr)
	{
		wos << L"path part: " << i++ << L" = " << *itr << std::endl;
	}

	wos << L"root_name() = " << pathToDisplay.root_name() << std::endl
		<< L"root_path() = " << pathToDisplay.root_path() << std::endl
		<< L"relative_path() = " << pathToDisplay.relative_path() << std::endl
		<< L"parent_path() = " << pathToDisplay.parent_path() << std::endl
		<< L"filename() = " << pathToDisplay.filename() << std::endl
		<< L"stem() = " << pathToDisplay.stem() << std::endl
		<< L"extension() = " << pathToDisplay.extension() << std::endl;

	return wos.str();
}
//just to try filesystem
std::wstring DisplayDirectoryContent()
{
	// This path may or may not refer to an existing file. We are
	// examining this path string, not file system objects.
	fs::path pathToDisplay(CurrentDirectory);

	std::wostringstream wos;
	int i = 0;
	wos << L"Elevents of directory: " << pathToDisplay << std::endl;
	for (auto& p: fs::directory_iterator(pathToDisplay))
	{
		wos << L"Element" << i++ << L" = " << p.path() << std::endl;
	}

	return wos.str();
}


using dll_function = IPlugin * (*)();

struct PluginStruct
{
	HINSTANCE Handler = NULL;
	dll_function ProcAdd = nullptr;
	IPlugin *Plugin = nullptr;
};

int main() 
{
	size_t numberOfPlugin = 0;

	std::vector<PluginStruct> plugins;
	PluginStruct tempPlugin;
	constexpr auto DLLDirectory = L"../x64/Debug/plugins/";

	for (auto& p : fs::directory_iterator(DLLDirectory))
	{
		auto DLLFile = p.path().c_str();
		tempPlugin.Handler = LoadLibrary(DLLFile);

		if (tempPlugin.Handler)
		{
			tempPlugin.ProcAdd = reinterpret_cast<dll_function>(GetProcAddress(tempPlugin.Handler, "newPlugin"));

			std::wcout << L"loaded the dynamyc library: " << DLLFile << std::endl;
			if (tempPlugin.ProcAdd)
			{
				tempPlugin.Plugin = tempPlugin.ProcAdd();
				plugins.push_back(tempPlugin);

				std::cout << "function was located" << std::endl;
			}
			else
			{
				std::cout << "could not locate the function" << std::endl;
			}

		}
		else
		{
			std::wcout << L"could not load the dynamyc library: " << DLLFile << std::endl;
		}
	}

	if (plugins.empty())
	{
		std::cout << std::endl << "No plugins were loaded" << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << std::endl << "Created " << plugins.size() << " plugins:" << std::endl;
	for (const auto plugin : plugins)
	{
		std::cout << plugin.Plugin->GetName() << std::endl;
	}
	std::cout << std::endl;

	for (const auto plugin : plugins)
	{
		delete plugin.Plugin;
		if (FreeLibrary(plugin.Handler))
		{
			std::cout << "library free" << std::endl;
		}
	}
	return EXIT_SUCCESS;
}