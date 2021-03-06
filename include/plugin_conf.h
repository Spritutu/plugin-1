#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include <boost/concept_check.hpp>
#include <vector>
#include <map>

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;
using std::vector;
using std::string;
using std::map;
using std::pair;

class plugin_manager;
class plugin_entity;

struct LibItem
{
	string	path;
	string	name;
	int		type;
	bool	status;
	int		number;
};

struct comp
{
	bool operator()(const pair<string, LibItem>& item1, const pair<string, LibItem>& item2)
	{
		if (item2.first == item1.first)
		{
			LibItem first_item = item1.second;
			LibItem second_item = item2.second;
			return !(first_item.path == second_item.path && first_item.name == second_item.name \
				&& first_item.type == second_item.type && first_item.status == second_item.status \
				&& first_item.number == second_item.number);
		}
		else
			return true;
		
	}
};

class configurefile_parser : public boost::noncopyable
{
	typedef map<string, vector<pair<string, string> > > ItemMap;
public:
	explicit	configurefile_parser(const std::string& path);

public:
	bool		parse(void);
	string		get_item(const string& item, const string& key);

public:
	ItemMap		config_item;

private:
	fs::path	file_path;
	int			error_code;
	pt::ptree	m_ptree;
};

class config_manager : public boost::noncopyable
{
public:
	config_manager(const string path);

public:
	bool					parse(void);
	bool					check_diff(void);
	bool                    load_dlls(void);
	void                    start(void);
	void                    close(void);
	void                    print_dlls_info(void);

private:
	plugin_entity			load_dll(const string& name, const LibItem& item);
	void					add_plugin(const string& name, const LibItem& item);
	void					stop_plugin(const string& name);
	void					increase_thread_num(const string& name, size_t num);

private:
	map<string, LibItem>	build_items(void);
	bool					deal_difference(map<string, LibItem> items);

private:
	configurefile_parser	        _parser;
	map<string, LibItem>	        _all_libs;
	map<string, plugin_entity>      _entities;
};
