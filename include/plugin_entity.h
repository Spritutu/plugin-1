#pragma once
#include <vector>

class plugin_base;

class plugin_entity
{
	typedef plugin_base* (*create_t)(void);
	typedef void         (*destroy_t)(void);
	
private:
	int	                        _number;
	std::vector<plugin_base*>        _instances;
	create_t                        _create;
	destroy_t                       _destroy;
	void*                           _dll_handle;
};