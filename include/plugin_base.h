#pragma once
#include <thread>
class plugin_base
{
public:
	plugin_base()
	{
		_status = true;
	}
	virtual ~plugin_base() {}

	void function() 
	{
		do_function();	
	}

	void thread_cancel()
	{
		std::terminate();
	}
private:
	void virtual do_function() = 0;	
public:
	bool _status;                    // the thread's running state
};

typedef plugin_base* (*create_t)(void);
typedef void(*destroy_t)(plugin_base*);