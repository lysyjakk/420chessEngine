#ifndef __TRACEANDERROR_H_
#define __TRACEANDERROR_H_

#include <ctime>
#include <iomanip>
#include <iostream>

#define TRACE(level, ...) printf( "%02d:%02d:%02d [%s] %s::%s(%d)  -   %s\n", \
                          time_str -> tm_hour,                                \
                          time_str -> tm_min,                                 \
                          time_str -> tm_sec,                                 \
                          level,                                              \
                          __FILE__,                                           \
                          __FUNCTION__,                                       \
                          __LINE__,                                           \
                          __VA_ARGS__ )


static std::time_t time_now = std::time(nullptr);
static struct tm* time_str = std::localtime(&time_now);


void TRACE_INFO(std::string message)
{
  TRACE("INFO", message.c_str());
}

void TRACE_WARNING(std::string message)
{
  TRACE("WARNING", message.c_str());
}

void TRACE_ERROR(std::string message)
{
  TRACE("ERROR", message.c_str());
}

void TRACE_DEBUG(std::string message)
{
  TRACE("DEBUG", message.c_str());
}

void TRACE_FATALERROR(std::string message, uint16_t error_code)
{
  TRACE("FATALLERROR", message.c_str());
  exit(error_code);
}

#endif
