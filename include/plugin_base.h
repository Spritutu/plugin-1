#pragma once

plugin_base* create()
{
        return new plugin_base();
}

void destroy(plugin_base* ptr)
{
        delete ptr;
}

class plugin_base
{
        plugin_base()
        {
                _status = false;
        }
private:
        bool _status;                    // the thread's running state
};