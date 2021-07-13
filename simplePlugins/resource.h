#include <iostream>

class IPlugin
{
public:
	IPlugin() = default;
	virtual ~IPlugin() = default;
	virtual std::string GetName() const = 0;
};

