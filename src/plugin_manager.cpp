#include "../include/plugin_manager.h"
#include "../include/plugin_entity.h"
#include <unistd.h>

plugin_manager::plugin_manager(const string& path)
{
        _config = new config_manager(path);
}

plugin_manager::~plugin_manager()
{
        delete _config;
}

void plugin_manager::start()
{
        if (!_config->parse())
        {
                std::cerr << "config file parse failed!" << std::endl;
                return;
        }
        if (!_config->load_dlls())
        {
                std::cerr << "load dlls/shared library failed!" << std::endl;
                return;
        }
        _config->start();                               // start all the dll/shared library's thread
        while (true)                                    // refresh every N seconds if there's a status change in the config file
        {
                _config->print_dlls_info();
                if (!refresh())
                {
                        std::cerr << "errors encountered while refreshing" << std::endl;
                }
                sleep(3);
        }
}

void plugin_manager::stop()
{

}

bool plugin_manager::refresh()
{
        return _config->check_diff();
}
