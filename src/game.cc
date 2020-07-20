/* > Includes *****************************************************************/

#include "../inc/game.hh"

/* > Defines ******************************************************************/

#define TARGET_FPS    30
#define OPTIMAL_TIME  1000000000 / TARGET_FPS
#define ONE_SEC_IN_MS 1000000

/* > Typedefs *****************************************************************/

typedef high_resolution_clock Time;

/* > Global Variables *********************************************************/

static GameManager   game_manager;
static ConsoleLogger console_logger;

atomic<bool> quit(false);    // signal flag

//SDL Textures
SDL_Texture *board_tex;
SDL_Texture *field_selection_tex;

//White Pieces
SDL_Texture *w_queen_tex;
SDL_Texture *w_bishop_tex;
SDL_Texture *w_rook_tex;
SDL_Texture *w_knight_tex;
SDL_Texture *w_king_tex;
SDL_Texture *w_pawn_tex;

//Black Pieces
SDL_Texture *b_queen_tex;
SDL_Texture *b_bishop_tex;
SDL_Texture *b_rook_tex;
SDL_Texture *b_knight_tex;
SDL_Texture *b_king_tex;
SDL_Texture *b_pawn_tex;

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
    return;
}

/* > Methods ******************************************************************/

void Game::run_game()
{
  game_manager.start_new_game();

  this -> game_loop();

  return;
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

  while(m_is_game_running == true)
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

    this -> handle_event();
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

  this -> clean();

  return;
}

void Game::init(const char* title,
                uint32_t    x_pos,
                uint32_t    y_pos,
                uint32_t    width,
                uint32_t    height,
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

  w_queen_tex  = TextureManager::LoadTexture(WHITE_QUEEN_TEX, m_renderer);
  w_bishop_tex = TextureManager::LoadTexture(WHITE_BISHOP_TEX, m_renderer);
  w_rook_tex   = TextureManager::LoadTexture(WHITE_ROOK_TEX, m_renderer);
  w_knight_tex = TextureManager::LoadTexture(WHITE_KNIGHT_TEX, m_renderer);
  w_king_tex   = TextureManager::LoadTexture(WHITE_KING_TEX, m_renderer);
  w_pawn_tex   = TextureManager::LoadTexture(WHITE_PAWN_TEX, m_renderer);

  b_queen_tex  = TextureManager::LoadTexture(BLACK_QUEEN_TEX, m_renderer);
  b_bishop_tex = TextureManager::LoadTexture(BLACK_BISHOP_TEX, m_renderer);
  b_rook_tex   = TextureManager::LoadTexture(BLACK_ROOK_TEX, m_renderer);
  b_knight_tex = TextureManager::LoadTexture(BLACK_KNIGHT_TEX, m_renderer);
  b_king_tex   = TextureManager::LoadTexture(BLACK_KING_TEX, m_renderer);
  b_pawn_tex   = TextureManager::LoadTexture(BLACK_PAWN_TEX, m_renderer);

  TRACE_INFO("Textures loaded successful");

  return;
}

void Game::render()
{
  SDL_RenderClear(m_renderer);
  SDL_RenderCopy(m_renderer, board_tex, NULL, NULL);

  this -> place_all_pieces();

  SDL_RenderPresent(m_renderer);

  return;
}

void Game::set_piece_img_size(uint8_t x_pos, uint8_t y_pos)
{
  this -> m_src_rect.w = 99;
  this -> m_src_rect.h = 99;
  this -> m_src_rect.x = 0;
  this -> m_src_rect.y = 0;

  this -> m_dest_rect.x = 28 + (x_pos * 100);
  this -> m_dest_rect.y = 27 + (y_pos * 100);
  this -> m_dest_rect.w = this -> m_src_rect.w;
  this -> m_dest_rect.h = this -> m_src_rect.h;

  return;
}

void Game::place_all_pieces()
{
  ChessBoard temp_board = game_manager.get_chess_board();

  for (uint8_t row = 0; row < MAX_BOARD_ROWS; ++row)
  {
    for (uint8_t column = 0; column < MAX_BOARD_COLUMNS; ++column)
    {

      if (temp_board[row][column].first == nullptr)
      {
        //continue;
      }
      
      switch (temp_board[row][column].second)
      {
      // White pieces
      case WHITE_PAWN:
        this -> set_piece_img_size(column, row);
        SDL_RenderCopy(m_renderer, w_pawn_tex, &m_src_rect, &m_dest_rect);
      break;
      case WHITE_ROOK:
        this -> set_piece_img_size(column, row);
        SDL_RenderCopy(m_renderer, w_rook_tex, &m_src_rect, &m_dest_rect);
      break;
      case WHITE_KNIGHT:
        this -> set_piece_img_size(column, row);
        SDL_RenderCopy(m_renderer, w_knight_tex, &m_src_rect, &m_dest_rect);
      break;
      case WHITE_BISHOP:
        this -> set_piece_img_size(column, row);
        SDL_RenderCopy(m_renderer, w_bishop_tex, &m_src_rect, &m_dest_rect);
      break;
      case WHITE_QUEEN:
        this -> set_piece_img_size(column, row);
        SDL_RenderCopy(m_renderer, w_queen_tex, &m_src_rect, &m_dest_rect);
      break;
      case WHITE_KING:
        this -> set_piece_img_size(column, row);
        SDL_RenderCopy(m_renderer, w_king_tex, &m_src_rect, &m_dest_rect);
      break;

    // Black pieces
      case BLACK_PAWN:
        this -> set_piece_img_size(column, row);
        SDL_RenderCopy(m_renderer, b_pawn_tex, &m_src_rect, &m_dest_rect);
      break;
      case BLACK_ROOK:
        this -> set_piece_img_size(column, row);
        SDL_RenderCopy(m_renderer, b_rook_tex, &m_src_rect, &m_dest_rect);
      break;
      case BLACK_KNIGHT:
        this -> set_piece_img_size(column, row);
        SDL_RenderCopy(m_renderer, b_knight_tex, &m_src_rect, &m_dest_rect);
      break;
      case BLACK_BISHOP:
        this -> set_piece_img_size(column, row);
        SDL_RenderCopy(m_renderer, b_bishop_tex, &m_src_rect, &m_dest_rect);
      break;
      case BLACK_QUEEN:
        this -> set_piece_img_size(column, row);
        SDL_RenderCopy(m_renderer, b_queen_tex, &m_src_rect, &m_dest_rect);
      break;
      case BLACK_KING:
        this -> set_piece_img_size(column, row);
        SDL_RenderCopy(m_renderer, b_king_tex, &m_src_rect, &m_dest_rect);
      break;

      case NONE:
      break;

      default:
        break;
      }
    }
  }

  return;
}

void Game::handle_event()
{
  SDL_Event event;

  while(SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_MOUSEBUTTONDOWN:
      TRACE_INFO("Click!");
    break;

    case SDL_QUIT:
      m_is_game_running = false;
    break;
    }
  }

  return;
}

void Game::clean() const
{
  SDL_DestroyWindow(m_window);
  SDL_DestroyRenderer(m_renderer);
  SDL_Quit();

  TRACE_INFO("Game cleaned");

  return;
}