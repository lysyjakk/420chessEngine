#include <iostream>
#include <cstdio>
#include <algorithm>
#include <string>
#include <map>
#include "../inc/game.hh"
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

static Game *game;

enum class Modes
{
  PLAY,
  LEARN,
  //...
  INVALID_MODE
};

void run_game()
{
  game = new Game();

  if (game == nullptr)
  {
    FATAL_ERROR(ERROR_CANNOT_INIT_GAME,
    "Cannot init game engine!");
  }
    game -> run_game();

  return;
}

Modes resolve_mode(std::string input)
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
    "Wrong number of input args!");
  }

  switch (resolve_mode(argv[1]))
  {
  case Modes::PLAY:
    run_game();
    break;

  case Modes::LEARN:
    std::cout << "learn DUPA";
    break;
  
  default:
    FATAL_ERROR(ERROR_MODE_DOESNT_EXIST,
                "Invalid mode! Please try play/learn");
    break;
  }

  return EXIT_SUCCESS;
}
