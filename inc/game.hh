#ifndef __GAME_H
#define __GAME_H

/* > Includes *****************************************************************/

#include <iostream>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <cstring>
#include <atomic>
#include "../inc/traceAndError.hh"
#include "../inc/errorCodes.hh"
#include "../inc/consoleLogger.hh"

using namespace std;
using namespace chrono;

class Game
{
public:
  Game()  = default;
  ~Game() = default;

  void run_game();

private:
  void game_loop();
};

#endif //__GAME_H