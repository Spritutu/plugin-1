#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/concept_check.hpp>

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

struct conf_ini
{
	std::string name;
	std::string wife_name;
	std::string address;
	std::string job;
    
	void print()
	{
		std::cout << "name: " << name << std::endl;
		std::cout << "wife's name: " << wife_name << std::endl;
		std::cout << "address: " << address << std::endl;
		std::cout << "job: " << job << std::endl;
	}
};

class configurefile_pareser
{
public:
	explicit configurefile_pareser(const std::string& path) : file_path(path)
	{
		if(read_ini())
		error_code = 0;
	else
		error_code = -1;
	}
  
	bool read_conf(conf_ini& content)
	{
		if(error_code != -1)
		{
			content.name = get_option("host1","name");
			content.wife_name = get_option("host1","wifename");
			content.address = get_option("host1","address");
			content.job = get_option("host1","job");
		}
      
		return true;
	}
 
  
	const std::string get_option(const std::string& item, const std::string& key)
	{
		if(error_code != -1)
		{
			std::string str1 = item + '.' + key;
			std::string res = m_ptree.get<std::string>(str1);
			return res;
		}
	}
  
	~configurefile_pareser(){}
  
private:
	bool read_ini()
	{
		if(fs::exists(file_path))
		{
			pt::read_ini(file_path.string(),m_ptree);
			return true;
		}
		else
		{
			std::cerr << file_path << "is not exist";
			return false;
		}
	}
  
private:
fs::path file_path;
int error_code;
pt::ptree m_ptree;
};