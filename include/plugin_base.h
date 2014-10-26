#pragma once

class plugin_base
{
public:
        plugin_base()
        {
                _status = false;
        }
	virtual ~plugin_base() {}

	void function() 
	{
		do_function();	
	}
private:
	void virtual do_function() = 0;	
public:
        bool _status;                    // the thread's running state
};
