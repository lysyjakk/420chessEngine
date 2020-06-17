#include <iostream>
#include <cstdio>
#include <algorithm>
#include <string>
#include <map>
#include "../inc/traceAndError.hh"
#include "../inc/errorCodes.hh"

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

enum class Modes
{
  PLAY,
  LEARN,
  //...
  INVALID_MODE
};

Modes resolveMode(std::string input)
{
     const std::map<std::string, Modes> modeStrings
  {
      { "play", Modes::PLAY },
      { "learn", Modes::LEARN },
      //...
  };

  transform(input.begin(), input.end(), input.begin(), ::tolower);

  auto itr = modeStrings.find(input);
  if( itr != modeStrings.end() )
  {
    return itr->second;
  }

  return Modes::INVALID_MODE;
}

int main (int argc, char* argv[])
{
  if (argc != 2)
  {
    FATAL_ERROR(ERROR_WRONG_ARGS_NUMBER,
    "Wrong number of input args!", NULL);
  }

  switch (resolveMode(argv[1]))
  {
  case Modes::PLAY:
    std::cout << "play DUPA";
    break;

  case Modes::LEARN:
    std::cout << "learn DUPA";
    break;
  
  default:
    FATAL_ERROR(ERROR_MODE_DOESNT_EXIST,
                "Invalid mode! Please try play/learn", NULL);
    break;
  }

  return EXIT_SUCCESS;
}
