#include <boost/noncopyable.hpp>
#include <iostream>
#include <mutex>
#include <fstream>
#include <sstream>

class log_file
{
public:
        log_file(const std::string& path)
                : _path(path)
        {}

	log_file()
	{}
        
        void open(const std::string& file_name)
        {
                _file.open(file_name.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
                std::string log_head = "\n\n*****starting log**************\n\n";
                write(log_head);
        }

        bool is_open()
        {
                return _file.is_open();
        }

        void write(const std::string& str)
        {
                if (is_open())
                {
                        _file.write(str.c_str(), str.length());
                }
        }
private:
        std::string _path;
        std::fstream _file;
};

enum log_level
{
        DEBUG,
        INFO,
        WARN,
        ERROR,
};

void console_output(log_level level, const std::string& prefex, const std::string& str)
{
        if (level == DEBUG)
        {
                std::cout << "\033[1;32m" << prefex << "\033[0m" << str << std::endl;
        }else if (level == INFO)
        {
                std::cout << "\033[32m" << prefex << "\033[0m" << str << std::endl;
        }else if (level == WARN)
        {
                std::cout << "\033[1;33m" << prefex << "\033[0m"  << str << std::endl;
        }else if (level == ERROR)
        {
                std::cout << "\033[1;31m" << prefex << "\033[0m" << str << std::endl;
        }
}

template <class T>
T& single_locker()
{
        static T locker;
        return locker;
}

template <class T>
T& single_writer()
{
        static T writer;
        return writer;
}

void log_writer(log_level level, const std::string& log_context)
{
        std::lock_guard<std::mutex> lock(single_locker<std::mutex>());
        if (single_writer<log_file>().is_open())
        {
                single_writer<log_file>().write(log_context);
        }
	std::string prefex = "time : ";
        console_output(level, prefex, log_context);
}

class log_to_write : public boost::noncopyable
{
public:
        explicit log_to_write(log_level level) 
                : _level(level)
        {}

        ~log_to_write()
        {
                log_writer(_level, _oss.str());
        }

        template <class T>
        log_to_write& operator << (T& value)
        {
                _oss << value;
                return *this;
        }
private:
        std::ostringstream _oss;
        log_level _level;
};

#define LOG_INI(path) do \
        {\
                log_file& file = single_writer<log_file>(); \
                std::string filename = path; \
                if (!filename.empty()) \
                { \
                        file.open(filename); \
                } \
        } while (0); \

#define LOG_DEBUG  log_to_write(DEBUG)
#define LOG_INFO log_to_write(INFO)
#define LOG_WARN log_to_write(WARN)
#define LOG_ERROR log_to_write(ERROR)
