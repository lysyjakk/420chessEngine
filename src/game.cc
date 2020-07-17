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

//SDL Textures
SDL_Texture *board_tex;
SDL_Texture *field_selection_tex;
/*SDL_Texture *queen_tex;
SDL_Texture *bishop_tex;
SDL_Texture *rook_tex;
SDL_Texture *horse_tex;*/

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
      
      last_fps_time = 0;
      fps = 0;
    }

    this -> render();

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

void Game::init(const char* title,
                uint16_t    x_pos,
                uint16_t    y_pos,
                uint16_t    width,
                uint16_t    height,
                bool        fullscreen)
{
  m_is_game_running = false;

  if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
  {
    TRACE_INFO("SDL Initialised...");
    m_window = SDL_CreateWindow(title,
                              x_pos,
                              y_pos,
                              width,
                              height,
                              (fullscreen ? SDL_WINDOW_FULLSCREEN : 0));

    if (m_window == NULL)
    {
      FATAL_ERROR(ERROR_FAILED_TO_INIT_SDL,
                  "Failed to init SDL window: %s", SDL_GetError());
    }

    TRACE_INFO("Window created");
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);

    if(m_renderer == NULL)
    {
      FATAL_ERROR(ERROR_FAILED_TO_INIT_SDL,
                  "Failed to init SDL renderer: %s", SDL_GetError());
    }

    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    TRACE_INFO("Renderer created");
    
    m_is_game_running = true;
    TRACE_INFO("SDL initialization successful");
  }

  TRACE_INFO("Loading textures...");

  board_tex           = TextureManager::LoadTexture(CHESS_BOARD_TEX, m_renderer);
  field_selection_tex = TextureManager::LoadTexture(FIELD_SELECTION_TEX, m_renderer);

  TRACE_INFO("Textures loaded successful");

  return;
}

void Game::render()
{
  SDL_RenderClear(m_renderer);
  SDL_RenderCopy(m_renderer, board_tex, NULL, NULL);
  SDL_RenderPresent(m_renderer);
}