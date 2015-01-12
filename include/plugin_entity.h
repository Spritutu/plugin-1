#pragma once
#include <vector>
#include "plugin_base.h"
#ifdef _Window
#include <windows.h>
#endif

class plugin_entity
{
public:
	int	                        _number;
	bool                       _status;
	std::vector<plugin_base*>  _instances;
	create_t                   _create;
	destroy_t                  _destroy;
#ifdef _LINUX
	void*                      _dll_handle;
#else 
	HINSTANCE                  _dll_handle;
#endif
};
