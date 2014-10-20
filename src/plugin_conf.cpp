#include "include/plugin_conf.hpp"
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <thread>

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

string& configurefile_parser::get_item(const string& item, const string& key)
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

int configurefile_parser::get_total_num(void)
{
	return config_item.size();
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
	if (ret = true)
	{
		_all_libs = items;
	}
	return ret;
}

map<string, LibItem> config_manager::build_items(void)
{
	LibItem item;
	map<string, LibItem> items;
	int size = _parser.get_total_num();
	for (int i = 0; i < size; ++i)
	{
		string str;
		str = _parser.get_item("lib", "path");
		item.path = str;
		str = _parser.get_item("lib", "name");
		item.name = str;
		str = _parser.get_item("lib", "type");
		item.type = std::stoi(str);
		str = _parser.get_item("lib", "status");
		item.status = boost::lexical_cast<bool>(str);
		str = _parser.get_item("lib", "number");
		item.number = std::stoi(str);
		items.insert(std::pair<string,LibItem>("lib",item));
	}
	return items;
}

bool config_manager::deal_difference(map<string, LibItem> items)
{
	for (auto & item_diff : items)
	{
		string name = item_diff.first;
		LibItem item_diff = item_diff.second;
		auto iter = _all_libs.find(name);
		if (iter == _all_libs.end())
		{
			// load the dll and start it;
		}
		else
		{
			if (item_diff.status == false)
			{
				// stop the thread
			}
			if (item_diff.number < iter->second.number)
			{
				// decrease the thread number
			}
			if (item_diff.number > iter->second.number)
			{
				// increate the thread number
			}
		}
	}
	return true;
}

bool config_manager::load_dll(void)
{
	auto iter = _all_libs.begin();
	for (; iter != _all_libs.end(); ++iter)
	{
		plugin_entity entity_info;
		string name = iter->first;
		LibItem lib_info = iter->second;
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
		create = dlsym(handle,"create");
		if (!create)
		{
			std::cerr << dlerror() << std::endl;
			exit(EXIT_FAILURE);
		}
		dlerror();
		destroy = dlsym(handle, "destroy");
		if (!destroy)
		{
			std::cerr << dlerror() << std::endl;
			exit(EXIT_FAILURE);
		}
		dlclose(handle);
		entity_info._dll_handle = handle;
		entity_info._create = create;
		entity_info._destroy = destroy;
		entity_info._number = lib_info.number;
		_entities.insert(pair<string, plugin_entity>(name, entity_info));
	}
	return true;
}

void config_manager::start(void)
{
	for (auto& mem : _entities)
	{
		create_t create = mem.second._create;
		plugin_base* obj = create();
		std::thread(&obj::function, obj);       // every plugin should derived from plugin_base, and implement the do_function which is the real thread body
		mem.second._instances.push_back(obj);
	}
}