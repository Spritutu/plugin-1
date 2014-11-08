// name : fengxiaopei
// mail : xiaopeifenng@gmail.com
//
#include "../include/plugin_manager.h"
#include <iostream>
#define _WIN
int main()
{
//        char* path = getenv("PLUGIN_PATH");
        char* path = "f:\\coding-on-github\\plugin\\config\\libconf.ini";
        if (!path)
        {
                std::cerr << "you should set PLUGIN_PATH first" << std::endl;
                exit(0);
        }
        plugin_manager* manager = new plugin_manager(path);
        manager->start();
}
