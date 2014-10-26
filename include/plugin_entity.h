#pragma once
#include <vector>

class plugin_base;
typedef plugin_base* (*create_t)(void);
typedef void         (*destroy_t)(void);

class plugin_entity
{
public:
	int	                        _number;
	bool                            _status;
	std::vector<plugin_base*>       _instances;
	create_t                        _create;
	destroy_t                       _destroy;
	void*                           _dll_handle;
};
