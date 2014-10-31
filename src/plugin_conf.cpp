#include "../include/plugin_conf.h"
#include "../include/plugin_entity.h"
#include "../include/plugin_base.h"
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <thread>
#include <dlfcn.h>
configurefile_parser::configurefile_parser(const std::string& path) : file_path(path)
{
	error_code = 0;
	if (exists(file_path))
	{
		read_ini(path, m_ptree);
	}
	else
	{
		error_code = -1;
	}
}

bool configurefile_parser::parse(void)
{
	if (error_code == -1)
	{
		return false;
	}
	for (auto& item : m_ptree)
	{
		vector<pair<string, string> > vec_temp;
		for (auto& node : item.second)
		{
			vec_temp.push_back(std::make_pair(node.first, node.second.get_value<string>()));
		}
		config_item.insert(std::make_pair(item.first, vec_temp));
	}
	return true;
}

string configurefile_parser::get_item(const string& item, const string& key)
{
	string value;
	auto iter_map = config_item.find(item);
	if (iter_map != config_item.end())
	{
		for (auto& vec_item : iter_map->second)
		{
			if (vec_item.first == key)
			{
				value = vec_item.second;
			}
		}
	}
	return value;
}

config_manager::config_manager(const string path) : _parser(path)
{
}

bool config_manager::parse(void)
{
	if (!_parser.parse())
	{
		return false;
	}
	_all_libs = build_items();
	return true;
}

bool config_manager::check_diff(void)
{
	bool ret = true;
	for (auto& entity : _entities)
	{
		for (int i = 0; i < entity.second._number; ++i)
		{
			if (entity.second._instances[i]._status == false)
			{
				entity.second._instances[i]->thread_cancel();                          // first stop the thread
				entity.second._destroy(entity.second._instances[i]);
			}
		}
		auto instances = entity.second._instances;
		instances.erase(
			std::remove_if(instances.begin(), instances.end(), [](const plugin_base* ptr)
				      {
				return ptr->_status == false;   
			}), 
			instances.end());
		if (!instances.size())
		{
			dlclose(entity.second._dll_handle);
		}
	}
	map<string, LibItem> items;
	map<string, LibItem> map_diff;
	if (!_parser.parse())
	{
		return false;
	}
	items = build_items();
	set_difference(items.begin(), items.end(), _all_libs.begin(), _all_libs.end(), \
		std::inserter(map_diff,map_diff.end()),comp());
	ret = deal_difference(map_diff);                      // update the new plugin status
	if (ret == true)
	{
		_all_libs = items;
	}
	return ret;
}

map<string, LibItem> config_manager::build_items(void)
{
	LibItem item;
	map<string, LibItem> items;
	auto begin = _parser.config_item.begin();
	auto end = _parser.config_item.end();
	for(; begin != end; ++begin)
	{
		string str;
		str = _parser.get_item(begin->first, "path");
		item.path = str;
		str = _parser.get_item(begin->first, "name");
		item.name = str;
		str = _parser.get_item(begin->first, "type");
		item.type = std::stoi(str);
		str = _parser.get_item(begin->first, "status");
		item.status = boost::lexical_cast<bool>(str);
		str = _parser.get_item(begin->first, "number");
		item.number = std::stoi(str);
		items.insert(std::pair<string,LibItem>(begin->first, item));
	}
	return items;
}

bool config_manager::deal_difference(map<string, LibItem> items)
{
	for (auto & item_diff : items)
	{
		string name = item_diff.first;
		LibItem diff = item_diff.second;
		auto iter = _all_libs.find(name);
		if (iter == _all_libs.end())                                    // load the dll and start it;
		{
			add_plugin(name, diff);
		}
		else
		{
			if (diff.status == false)                          // stop the thread
			{
				stop_plugin(name);
			}
			if (diff.number < iter->second.number \
				|| diff.number > iter->second.number)      // decrease the thread number
			{
				int num = diff.number - iter->second.number;
				increase_thread_num(name, num);
			}
		}
	}
	return true;
}

void config_manager::add_plugin(const string& name, const LibItem& item)
{
	plugin_entity entity = load_dll(name, item);
	size_t num = entity._number;
	while (num--)
	{
		create_t create = entity._create;
		plugin_base* obj = create();
		if (entity._status == true)
		{
			std::thread(&plugin_base::function, obj);       // every plugin should derived from plugin_base, 
			obj->_status = true;                    // and implement the do_function which is the real thread body
		}                                               
		entity._instances.push_back(obj);
	}
}

void config_manager::stop_plugin(const string& name)
{
	auto item = _entities.find(name);
	if (item != _entities.end())
	{
		item->second._status= false;                    // just change the status flag. next refresh time check if it's time to stop the thread
		auto begin = item->second._instances.begin();
		auto end = item->second._instances.end();
		for_each(begin, end, [](plugin_base* ptr){
			ptr->_status = false;                   // change all the stances's state to stop status
		});
	}
}

void config_manager::increase_thread_num(const string& name, size_t num)
{
	auto item = _entities.find(name);
	auto entity = item->second;
	if (num > 0)                                            // add num threads
	{
		while (num--)
		{
			create_t create = entity._create;
			plugin_base* obj = create();
			std::thread(&plugin_base::function, obj);
			obj->_status = true;
			item->second._instances.push_back(obj);
		}
	}
	else
	{                                                       // decrease num threads 
		int i = 0;
		while (num++)
		{
			item->second._instances[i++]->_status = false;
		}
	}
}

bool config_manager::load_dlls(void)
{
	auto iter = _all_libs.begin();
	for (; iter != _all_libs.end(); ++iter)
	{
		plugin_entity entity = load_dll(iter->first, iter->second);
		_entities.insert(pair<string, plugin_entity>(iter->first, entity));
	}
	return true;
}

plugin_entity config_manager::load_dll(const string& name, const LibItem& item)
{
	plugin_entity entity_info;
	LibItem lib_info = item;
	string path = lib_info.path + "/" + lib_info.name;
	create_t create;
	destroy_t destroy;
	void* handle = dlopen(path.c_str(), RTLD_LAZY);
	if (!handle)
	{
		std::cerr << dlerror() << std::endl;
		exit(EXIT_FAILURE);
	}
	dlerror();
	create = (create_t)dlsym(handle, "create");
	if (!create)
	{
		std::cerr << dlerror() << std::endl;
		exit(EXIT_FAILURE);
	}
	dlerror();
	destroy = (destroy_t)dlsym(handle, "destroy");
	if (!destroy)
	{
		std::cerr << dlerror() << std::endl;
		exit(EXIT_FAILURE);
	}
	entity_info._status = lib_info.status;
	entity_info._dll_handle = handle;
	entity_info._create = create;
	entity_info._destroy = destroy;
	entity_info._number = lib_info.number;
	entity_info._dll_handle = handle;
	return entity_info;
}

void config_manager::start(void)
{
	for (auto& mem : _entities)
	{
		size_t num = mem.second._number;
		while (num--)
		{
			create_t create = mem.second._create;
			plugin_base* obj = create();
			if (mem.second._status == true)
			{
				std::thread plugin_thread(&plugin_base::function, obj);       // every plugin should derived from plugin_base, and implement the do_function which is the real thread body
				plugin_thread.detach();
				
			}
			mem.second._instances.push_back(obj);
		}
	}
}

void config_manager::close(void)
{
	for (auto& item : _entities)
	{
		for (int i = 0; i < item.second._number; ++i)
		{
			item.second._instances[i]->thread_cancel();                          // first stop the thread
			item.second._destroy(item.second._instances[i]);                       
		}
		dlclose(item.second._dll_handle);
	}
}

void config_manager::print_dlls_info(void)
{
	std::cout << "-------------------------------------------------" << std::endl;
	std::cout << "|" << '\t' << "name" << '\t' << "|" << '\t' << "status" << '\t' << "|" << '\t' << "number" << '\t' << "|" << std::endl;
	std::cout << "-------------------------------------------------" << std::endl;
	for (auto& item : _entities)
	{
		std::cout << '|' << '\t' << item.first << '\t' << "|" << '\t' << item.second._status << '\t' << "|" << '\t' << item.second._number << '\t' << "|" << std::endl;
		std::cout << "-------------------------------------------------" << std::endl;
	}
}
