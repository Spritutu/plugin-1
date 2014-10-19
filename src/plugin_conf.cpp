#include "include/plugin_conf.hpp"
#include <boost/lexical_cast.hpp>
#include <algorithm>

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
	map<string, LibItem> items;
	map<string, LibItem> map_diff;
	if (!_parser.parse())
	{
		return false;
	}
	items = build_items();
	set_difference(items.begin(), items.end(), _all_libs.begin(), _all_libs.end(), std::inserter(map_diff,map_diff.end()),comp());
	return deal_difference(map_diff);
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
	return true;
}