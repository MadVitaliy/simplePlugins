#include <iostream>
#include "../simplePlugins/resource.h"

extern "C" __declspec(dllexport) void func()
{
	std::cout << "DLL-lib1" << std::endl;
}

class Square : IPlugin
{
public:
	Square();
	~Square();
	std::string GetName() const override
	{
		return std::string("Squre");
	}
};

Square::Square()
{
}

Square::~Square()
{
}

extern "C" __declspec(dllexport) IPlugin * newPlugin()
{
	return reinterpret_cast<IPlugin*>(new Square());
}