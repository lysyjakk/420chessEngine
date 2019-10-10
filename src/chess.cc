#include <iostream>
#include <cstdio>
#include <string.h>
#include "../inc/traceAndError.hh"

int main (int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "1";
    exit(EXIT_FAILURE);
  }

  if (strncmp(argv[1], "play", (unsigned)sizeof(argv[1])) == 0)
  {
    TRACE_INFO("dupa");
    TRACE_WARNING("dupa");
    TRACE_DEBUG("dupa");
    TRACE_ERROR("dupa");
    TRACE_FATALERROR("dupa", 10);
  }

  else if (strncmp(argv[1], "learn", (unsigned)sizeof(argv[1])) == 0)
  {
  }

  else
  {
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
