#ifndef __TRACEANDERROR_H_
#define __TRACEANDERROR_H_

/* > Includes *****************************************************************/

#include <time.h>
#include <sys/time.h>
#include <string.h>

/* > Defines ******************************************************************/

//////////////// COLORS ////////////////////

#define COLOR_RED          "\033[0;31m"
#define COLOR_BOLD_RED     "\033[1;31m"
#define COLOR_GREEN        "\033[0;32m"
#define COLOR_BOLD_GREEN   "\033[1;32m"
#define COLOR_YELLOW       "\033[0;33m"
#define COLOR_BOLD_YELLOW  "\033[1;33m"
#define COLOR_BLUE         "\033[0;34m"
#define COLOR_BOLD_BLUE    "\033[1;34m"
#define COLOR_MAGENTA      "\033[0;35m"
#define COLOR_BOLD_MAGENTA "\033[1;35m"
#define COLOR_CYAN         "\033[0;36m"
#define COLOR_BOLD_CYAN    "\033[1;35m"
#define COLOR_WHITE        "\033[0;97m"
#define COLOR_BOLD_WHITE   "\033[1;97m"
#define COLOR_RESET        "\033[0m"

#define COLOR_TRACE_DEFAULT "\033[1;97m"
#define COLOR_TRACE_ERROR   "\033[1;97;41m"
#define COLOR_TRACE_WARNING "\033[1;97;43m"

//////////////////////////////////////////////

#define __FILENAME__ (strrchr("/" __FILE__, '/') + 1)

#define TRACE( level, format, ... )                         \
          get_current_timestamp();                          \
          printf( "\n<%s> %s::%s(%d) [%s]: " format,        \
                                             timestamp,     \
                                             __FILENAME__,  \
                                             __FUNCTION__,  \
                                             __LINE__,      \
                                            level,          \
                                            ##__VA_ARGS__ );


#define TRACE_INFO( format, ... )                        \
          { printf(COLOR_TRACE_DEFAULT);                 \
            TRACE("INFO", format, ##__VA_ARGS__);        \
            printf(COLOR_RESET);                         \
          }

#define TRACE_WARNING( format, ... )                     \
          {                                              \
            printf(COLOR_TRACE_WARNING);                 \
            TRACE("WARNING", format, ##__VA_ARGS__);     \
            printf(COLOR_RESET);                         \
          }

#define TRACE_ERROR( format, ... )                       \
          { printf(COLOR_TRACE_ERROR);                   \
            TRACE("ERROR", format, ##__VA_ARGS__);       \
            printf(COLOR_RESET);                         \
          }

#define FATAL_ERROR( error_code, format, ... )           \
          {                                              \
            printf(COLOR_TRACE_ERROR);                   \
            TRACE("FATAL ERROR", format, ##__VA_ARGS__); \
            printf(COLOR_RESET);                         \
            exit(error_code);                            \
          }

#ifndef DEBUG_MODE
  #define TRACE_DEBUG( format, ... )                     \
          {                                              \
            printf(COLOR_TRACE_DEFAULT);                 \
            TRACE("DEBUG", format, ##__VA_ARGS__);       \
            printf(COLOR_RESET);                         \
          }
#endif

/* > Local Variables **********************************************************/

static char timestamp[64];

/* > Local Functions **********************************************************/

static void get_current_timestamp()
{
  char format[64];
  struct timeval tv;
  struct tm *tm;

  gettimeofday (&tv, NULL);
  tm = localtime (&tv.tv_sec);
  strftime (format, sizeof (format), "%H:%M:%S:%%06u", tm);
  snprintf (timestamp, sizeof (timestamp), format, tv.tv_usec);

  return;
}

#endif //__TRACEANDERROR_H_
