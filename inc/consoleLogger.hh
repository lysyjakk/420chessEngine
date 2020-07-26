#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <memory>
#include <string>
#include <sys/stat.h>
#include "../inc/traceAndError.hh"
#include "../inc/constants.hh"
#include "../inc/errorCodes.hh"

class ConsoleLogger
{
public:
  ConsoleLogger();
  ~ConsoleLogger();

  void init();
  void clear() const;
  template<typename ... Args>
  void print(const std::string& format, Args ... args) const;

private:
  std::string m_console_filename;
  bool        m_is_initializated;
};

template<typename ... Args>
void ConsoleLogger::print(const std::string& format, Args ... args) const
{
  size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
  if (size <= 0)
  {
    TRACE_ERROR("Error during formatting!");
    return;
  }

  std::unique_ptr<char[]> buf(new char[ size ]); 
  snprintf(buf.get(), size, format.c_str(), args ...);

  std::string output = std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside

  int result = system(((std::string("echo ") +
                      output + std::string(" > ") +
                      m_console_filename)).c_str());
  
  if (result != 0)
  {
    FATAL_ERROR(ERROR_CANNOT_ACCESS_TO_FILE,
    "Cannot acess to %s !", m_console_filename.c_str());
  }

  return;
}