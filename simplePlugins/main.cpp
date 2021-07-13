#include "resource.h"

#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <filesystem>

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

int main() 
{
	size_t numberOfPlugin = 0;
	IPlugin *plugins[3];
	constexpr auto DLLDirectory = L"../x64/Debug/plugins/";

	for (auto& p : fs::directory_iterator(DLLDirectory))
	{
		auto DLLFile = p.path().c_str();
		auto hGetProcIDDLL = LoadLibrary(DLLFile);

		if (hGetProcIDDLL)
		{
			auto* createPluginFunction = reinterpret_cast<dll_function>(GetProcAddress(hGetProcIDDLL, "newPlugin"));

			std::wcout << L"loaded the dynamyc library: " << DLLFile << std::endl;
			if (createPluginFunction)
			{
				std::cout << "function was located" << std::endl;
				plugins[numberOfPlugin] = createPluginFunction();
				numberOfPlugin++;
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

	if (numberOfPlugin == 0)
	{
		std::cout << std::endl << "No plugins were loaded" << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << std::endl << "Created " << numberOfPlugin << " plugins:" << std::endl;
	for (size_t i = 0; i < numberOfPlugin; ++i)
	{
		std::cout << plugins[i]->GetName() << std::endl;
	}

	for (size_t i = 0; i < numberOfPlugin; ++i)
	{
		delete plugins[i];
	}

	return EXIT_SUCCESS;
}