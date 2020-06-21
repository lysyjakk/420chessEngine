/* > Includes *****************************************************************/

#include "../inc/game.hh"

/* > Defines ******************************************************************/

#define TARGET_FPS    30
#define OPTIMAL_TIME  1000000000 / TARGET_FPS
#define ONE_SEC_IN_MS 1000000

/* > Typedefs *****************************************************************/

typedef high_resolution_clock Time;

/* > Global Variables *********************************************************/



/* > Local Functions **********************************************************/

static long get_ms(auto time)
{
  auto now_in_ms = time_point_cast<microseconds>(time);
  auto value = now_in_ms.time_since_epoch();

  return value.count();
}

/* > Methods ******************************************************************/

void Game::run_game()
{
  this -> game_loop();
}

void Game::game_loop()
{
  auto     last_loop_time = Time::now();
  double   last_fps_time  = 0;
  uint16_t fps            = 0;

  ConsoleLogger *console_logger = new ConsoleLogger();
  console_logger -> init();
  //console_logger -> print("DUPA %s", "blada");
/*  while(true)
  {
    auto now = Time::now();
    long update_duration = get_ms(now) - get_ms(last_loop_time);

    last_loop_time = now;
    last_fps_time += update_duration;
    ++fps;

    if (last_fps_time >= ONE_SEC_IN_MS)
    {
      #ifdef WINDOWS
        system("cls");
      #else
        system("clear");
      #endif // WINDOWS

      cout << "FPS: " << fps << endl;

      last_fps_time = 0;
      fps = 0;
    }

    this_thread::sleep_for(
      milliseconds(
        (get_ms(last_loop_time) - get_ms(Time::now()) + OPTIMAL_TIME)/ONE_SEC_IN_MS));
  }
*/
}