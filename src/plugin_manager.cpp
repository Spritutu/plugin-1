#include "../include/plugin_manager.h"
#include "../include/plugin_entity.h"
#include "../include/logging.h"
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

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
                LOG_ERROR << "config file parse failed!";
                return;
        }
        LOG_DEBUG << "read config file successfull";
        if (!_config->load_dlls())
        {
                LOG_ERROR << "load dlls/shared library failed!";
                return;
        }
        _config->start();                               // start all the dll/shared library's thread
        LOG_DEBUG << "plugin start all shared library/dll successful";
        while (true)                                    // refresh every N seconds if there's a status change in the config file
        {
                _config->print_dlls_info();
                if (!refresh())
                {
                        LOG_ERROR << "errors encountered while refreshing";
                }
                boost::this_thread::sleep(boost::posix_time::milliseconds(3000));
        }
}

void plugin_manager::stop()
{}

bool plugin_manager::refresh()
{
        return _config->check_diff();
}
