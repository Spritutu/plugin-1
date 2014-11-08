// name : fengxiaopei
// mail : xiaopeifenng@gmail.com
//
#include "../include/plugin_manager.h"
#include "../include/logging.h"
#include <iostream>
#define _WIN

int main()
{
#ifdef _LINUX
        char* path = getenv("PLUGIN_PATH");
#else
        char* path = "f:\\coding-on-github\\plugin\\config\\libconf.ini";
#endif
        LOG_INI("log.txt");
        if (!path)
        {
              //  LOG_ERROR << "you should set PLUGIN_PATH first";
                exit(0);
        }
        plugin_manager* manager = new plugin_manager(path);
        manager->start();
}
