// name : fengxiaopei
// mail : xiaopeifenng@gmail.com
//
#include <iostream>
int main()
{
        char* path = getenv("PLUGIN_PATH");
        if (!path)
        {
                std::err << "you should set PLUGIN_PATH first" << std::endl;
                exit(0);
        }
        plugin_manager* manager = new plugin_manager(path);
        manager->start();
}
