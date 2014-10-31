#include "../include/plugin_base.h"
#include <unistd.h>
#include <iostream>

class plugin_example : public plugin_base
{
public:
	plugin_example() {}
	virtual ~plugin_example() {}
private:
	void do_function()
	{
		size_t times = 10;
		while(times--)	
		{
			std::cout << "tick..." << std::endl;	
		}
	}
};

extern "C" plugin_base* create()
{
	return new plugin_example();
}

extern "C" void destroy(plugin_base* p)
{
	delete p;
}
