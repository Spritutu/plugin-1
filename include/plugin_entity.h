#pragma once
#include <vector>

class plugin_base;

class plugin_entity
{
public:
	
private:
	int	_number;
	std::vector<plugin_base> _instances;
};