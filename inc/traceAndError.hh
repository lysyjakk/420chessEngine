#ifndef __TRACEANDERROR_H_
#define __TRACEANDERROR_H_

#include <ctime>

#define TRACE( level, format, ... )   printf( "\n%02d:%02d:%02d %s::%s(%d) [%s]: " format, \
                                             time_str -> tm_hour,                          \
                                             time_str -> tm_min,                           \
                                             time_str -> tm_sec,                           \
                                             __FILE__,                                     \
                                             __FUNCTION__,                                 \
                                             __LINE__,                                     \
                                             level,                                        \
                                             __VA_ARGS__ )


#define TRACE_INFO( format, ... )          TRACE("INFO",    format, __VA_ARGS__)
#define TRACE_WARNING( format, ... )       TRACE("WARNING", format, __VA_ARGS__)
#define TRACE_ERROR( format, ... )         TRACE("ERROR",   format, __VA_ARGS__)
#define TRACE_FATAL_ERROR( error_code, format, ... ) \
          { TRACE("FATAL ERROR", format, __VA_ARGS__); exit(error_code); }

#ifndef DEBUG_MODE
  #define TRACE_DEBUG( format, ... )       TRACE("DEBUG", format, __VA_ARGS__)
#endif

static std::time_t time_now = std::time(nullptr);
static struct tm* time_str  = std::localtime(&time_now);

#endif
