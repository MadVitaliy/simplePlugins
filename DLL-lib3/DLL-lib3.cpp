#include <iostream>


#include <iostream>
#include "../simplePlugins/resource.h"

extern "C" __declspec(dllexport) void func()
{
	std::cout << "DLL-lib3" << std::endl;
}

class Circle : IPlugin
{
public:
	Circle();
	~Circle();
	std::string GetName() const override
	{
		return std::string("Circle");
	}
};

Circle::Circle()
{
}

Circle::~Circle()
{
}

extern "C" __declspec(dllexport) IPlugin * newPlugin()
{
	return reinterpret_cast<IPlugin*>(new Circle());
}