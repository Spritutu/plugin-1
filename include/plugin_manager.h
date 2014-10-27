#pragma once
#include <boost/noncopyable.hpp>
#include <string>
#include "plugin_conf.h"

using std::string;

class plugin_manager : public boost::noncopyable
{
public:
        explicit        plugin_manager(const string& path);

                        ~plugin_manager();

        void            start(void);

        void            stop(void);
private:
        bool            refresh(void);
private:
        config_manager* _config;
};