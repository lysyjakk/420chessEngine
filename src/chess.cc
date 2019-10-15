#include <iostream>
#include <cstdio>
#include <string.h>
#include "../inc/traceAndError.hh"
#include "../inc/errorCodes.hh"
#include "game.hh"

/**
*    @name neural network chess player
*
*    This program is implementation of computer chess player,
*    program use neural networks.
*    Code was written to needs BA dissertation.
*    Copyright MIT License.
*
*    @author Adam Wójcik
*
*    2019
*/

int main (int argc, char* argv[])
{
  if (argc != 2)
  {
    TRACE_FATAL_ERROR(ERROR_WRONG_ARGS_NUMBER,
                      "Wrong number of input args!", NULL);
  }

  if (strncmp(argv[1], "play", (unsigned)sizeof(argv[1])) == 0)
  {
    ;
  }

  else if (strncmp(argv[1], "learn", (unsigned)sizeof(argv[1])) == 0)
  {

  }

  else
  {
    TRACE_FATAL_ERROR(ERROR_MODE_DOESNT_EXIST,
                      "Mode '%s' doesnt exist! Please try play/learn",
                      argv[1]);
  }

  return EXIT_SUCCESS;
}
