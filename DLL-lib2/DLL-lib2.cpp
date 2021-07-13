#include <iostream>
#include "../simplePlugins/resource.h"

extern "C" __declspec(dllexport) void func()
{
	std::cout << "DLL-lib2" << std::endl;
}

class Triangle : IPlugin
{
public:
	Triangle();
	~Triangle();
	std::string GetName() const override
	{
		return std::string("Triangle");
	}
};

Triangle::Triangle()
{
}

Triangle::~Triangle()
{
}

extern "C" __declspec(dllexport) IPlugin * newPlugin()
{
	return reinterpret_cast<IPlugin*>(new Triangle());
}