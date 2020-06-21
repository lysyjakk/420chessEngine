#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <memory>
#include <string>
#include "../inc/traceAndError.hh"
#include "../inc/constants.hh"
#include "../inc/errorCodes.hh"



class ConsoleLogger
{
public:
  ConsoleLogger()  = default;
  ~ConsoleLogger() = default;

  void init();
  void clear();
  template<typename ... Args>
  void print(const std::string& format, Args ... args);

private:
  std::string m_console_filename;
};