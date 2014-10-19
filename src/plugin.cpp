// plugin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "include/plugin_conf.hpp"
#include <string>
#include <map>
#include <vector>
#include <boost/filesystem/path.hpp>

using boost::property_tree::ptree;
using boost::filesystem::path;
struct configure_ini
{
	std::string path;
	std::string type;
	std::string name;
	std::string status;
	std::string instances;
};

std::ostream& operator << (std::ostream& os, const configure_ini& ini)
{
	os << "path: " << ini.path << std::endl;
	os << "type: " << ini.type << std::endl;
	os << "name: " << ini.name << std::endl;
	os << "status: " << ini.status << std::endl;
	os << "instances: " << ini.instances << std::endl;
	return os;
}

static bool read_config(const char* path, ptree& tree)
{
	boost::filesystem::path p(path);
	if (exists(p))
	{
		read_ini(path,tree);
	}
	else
	{
		std::cout << "path: " << path << " is not exist" << std::endl;
		return false;
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	std::map<std::string, std::vector<std::pair<std::string,std::string> > > config_items;
	std::string path("config.ini");
	
//	configurefile_pareser parser(path);
//	conf_ini conf_file;
//	parser.read_conf(conf_file);
//	conf_file.print();
	using boost::property_tree::ptree;
	ptree pt;
	read_ini(path.c_str(),pt);
	for (auto& section : pt)
	{
		std::cout << '[' << section.first << ']' << std::endl;
		for (auto& key : section.second)
		{
			std::cout << key.first << "=" << key.second.get_value<std::string>() << std::endl;
		}
	}

	std::cout << "................" << std::endl;
	auto iter = pt.begin();
	for (iter; iter != pt.end(); ++iter)
	{
		std::cout << '[' << iter->first << ']' << std::endl;
		std::vector<std::pair<std::string, std::string> > vec;
		for (auto& key = iter->second.begin(); key != iter->second.end(); ++key)
		{
			vec.push_back(std::make_pair(key->first, key->second.get_value<std::string>()));
			std::cout << key->first << "=" << key->second.get_value<std::string>() << std::endl;
		}
		config_items.insert(std::make_pair(iter->first,vec));
	}

	std::vector<configure_ini> vec_ini;
	for (auto& item : config_items)
	{
		configure_ini ini;
		for (auto& key : item.second)
		{
			if (key.first == "path")
			{
				ini.path = key.second;
			}
			if (key.first == "type")
			{
				ini.type = key.second;
			}
			if (key.first == "name")
			{
				ini.type = key.second;
			}
			if (key.first == "status")
			{
				ini.status = key.second;
			}
			if (key.first == "instances")
			{
				ini.status = key.second;
			}
		}
		vec_ini.push_back(ini);
	}
//	std::cout << vec_ini[0];
	std::cout << "!!!!!!!!!!" << std::endl;
	for (auto& mem : vec_ini)
	{
		std::cout << mem;
	}

	return 0;
}

