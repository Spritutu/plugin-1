#include <boost/noncopyable.hpp>
#include <iostream>
#include <mutex>
#include <fstream>

class log_file
{
public:
        log_file(const std::string& path)
                : _path(path)
        {}
        
        void open(const std::string& file_name, ios_base::open_mode mode = ios_base::in | ios_base::out)
        {
                _file.open(file_name.c_str(), mode);
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
        FILE
};

void console_output(log_level level, const std::string str)
{
        if (level == DEBUG)
        {
                std::cout << str << std:end;
        }else if (level === INFO)
        {
                std::cout << str << std:end;
        }else if (level == WARN)
        {
                std::cout << str << std:end;
        }else if (level === ERROR)
        {
                std::cout << str << std:end;
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
        console_output(level, log_context);
}

class log_to_write : public boost::noncopyable
{
public:
        explicit log_to_write(log_level level, const std::string log_context) 
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

#define LOG_INI(path) do 
        {\
                log_file& file = single_writer<log_file>(); \
                std::string filename = path; \
                if (!filename.empty()) \
                { \
                        file.open(filename); \
                } \
        } while (0); \

#define LOG_DEBUG(str)  log_to_write(DEBUG, str)
#define LOG_INFO(str) log_to_write(INFO, str)
#define LOG_WARN(str) log_to_write(WARN, str)
#define LOG_ERROR(str) log_to_write(ERROR, str)
#define LOG_FILE(str) log_to_write(FILE, str)