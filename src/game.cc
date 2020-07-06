/* > Includes *****************************************************************/

#include "../inc/game.hh"

/* > Defines ******************************************************************/

#define TARGET_FPS    30
#define OPTIMAL_TIME  1000000000 / TARGET_FPS
#define ONE_SEC_IN_MS 1000000

/* > Typedefs *****************************************************************/

typedef high_resolution_clock Time;

/* > Global Variables *********************************************************/

static ConsoleLogger console_logger;
atomic<bool> quit(false);    // signal flag

/* > Local Functions **********************************************************/

static long get_ms(auto time)
{
  auto now_in_ms = time_point_cast<microseconds>(time);
  auto value = now_in_ms.time_since_epoch();

  return value.count();
}

static void got_SIGINT_signal(int)
{
    quit.store(true);
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

//SIGINT handler
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = got_SIGINT_signal;
  sigfillset(&sa.sa_mask);
  sigaction(SIGINT,&sa,NULL);

  console_logger.init();
TRACE_INFO("Cos");
  while(true)
  {
    auto now = Time::now();
    long update_duration = get_ms(now) - get_ms(last_loop_time);

    last_loop_time = now;
    last_fps_time += update_duration;
    ++fps;

    if (last_fps_time >= ONE_SEC_IN_MS)
    {
      console_logger.clear();
      console_logger.print("FPS: %d", fps);
      TRACE_INFO("DUPA");
      fprintf(stdout, "%s", "message 2, on stdout (using fprintf)\n");
      last_fps_time = 0;
      fps = 0;
    }

    this_thread::sleep_for(
      milliseconds(
        (get_ms(last_loop_time) - get_ms(Time::now()) + OPTIMAL_TIME)/ONE_SEC_IN_MS));
    
    //Check if SIGINT recived.
    if( quit.load() )
    {
      TRACE_ERROR("Received SIGINT signal! Closing the program...");
      break;
    }
  }
}