// name : fengxiaopei
// mail : xiaopeifenng@gmail.com
//
#include "../include/plugin_manager.h"
#include "../include/logging.h"
#include <iostream>
#include <string>

int main()
{
#ifdef _LINUX
//        char* path = getenv("PLUGIN_PATH");
		char* path = "/home/f_x_p/coding-on-github/plugin/config/libconf.ini";
		std::cout << "linux getenv: " << path << std::endl;
#else
        char* path = "f:\\coding-on-github\\plugin\\config\\libconf.ini";
#endif
        LOG_INI("log.txt");
        if (!path)
        {
                LOG_ERROR << "you should set PLUGIN_PATH first";
                exit(0);
        }
        plugin_manager* manager = new plugin_manager(path);
        manager->start();
}
