/* > Includes *****************************************************************/

#include "../inc/game.hh"

/* > Defines ******************************************************************/

#define TARGET_FPS    30
#define OPTIMAL_TIME  1000000000 / TARGET_FPS
#define ONE_SEC_IN_MS 1000000

#define FLIP_X_AXIS(x) (7 - x)
#define FLIP_Y_AXIS(y) (7 - y)

/* > Typedefs *****************************************************************/

typedef high_resolution_clock Time;

/* > Structs ******************************************************************/

static struct FieldSelectionPosition
{
  uint32_t x;
  uint32_t y;
} field_selction_pos;

static struct SelectedPiece
{
  uint32_t x;
  uint32_t y;
} selected_piece;


/* > Global Variables *********************************************************/

static GameManager   game_manager;
static ConsoleLogger console_logger;

static bool is_piece_grabbed = false;

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

static long get_ms(Time::time_point time)
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
  std::string program_name =  std::string("\n\n\
  _  _  ____   ___     ____ _                  _____ _   _  ____ ___ _   _ _____ \n\
 | || ||___ \\ / _ \\   / ___| |__   ___ ___ ___| ____| \\ | |/ ___|_ _| \\ | | ____|\n\
 | || |_ __) | | | | | |   | '_ \\ / _ / __/ __|  _| |  \\| | |  _ | ||  \\| |  _|  \n\
 |__   _/ __/| |_| | | |___| | | |  __\\__ \\__ | |___| |\\  | |_| || || |\\  | |___ \n\
    |_||_____|\\___/   \\____|_| |_|\\___|___|___|_____|_| \\_|\\____|___|_| \\_|_____|\n\
                                                                                \n\
  ");

  std::cout << program_name;

  game_manager.start_new_game();
  game_loop();

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
      BitBoardToGUI temp_board = game_manager.get_board();

      console_logger.clear();
      console_logger.print("FPS: %d", fps);

      console_logger.print("");
      console_logger.print("-----------------------------");
      console_logger.print("");

      for(int i = 56; i >= 0; i-=8)
      {
        console_logger.print("%02d %02d %02d %02d %02d %02d %02d %02d",
                            temp_board[i],
                            temp_board[i+1],
                            temp_board[i+2],
                            temp_board[i+3],
                            temp_board[i+4],
                            temp_board[i+5],
                            temp_board[i+6],
                            temp_board[i+7]);
      }
      console_logger.print("");
      console_logger.print("-----------------------------");

      last_fps_time = 0;
      fps = 0;
    }

    handle_event();
    render();

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

  clean();

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

  if (is_piece_grabbed == true)
  {
    set_img_size(field_selction_pos.x, field_selction_pos.y, 100, 100);
    SDL_RenderCopy(m_renderer, field_selection_tex, &m_src_rect, &m_dest_rect);
  }

  place_all_pieces();

  SDL_RenderPresent(m_renderer);

  return;
}

void Game::set_img_size(uint32_t x_pos,
                        uint32_t y_pos,
                        uint32_t width,
                        uint32_t height)
{
  m_src_rect.w = width;
  m_src_rect.h = height;
  m_src_rect.x = 0;
  m_src_rect.y = 0;

  m_dest_rect.x = x_pos;
  m_dest_rect.y = y_pos;
  m_dest_rect.w = m_src_rect.w;
  m_dest_rect.h = m_src_rect.h;

  return;
}

void Game::place_all_pieces()
{
  BitBoardToGUI board = game_manager.get_board();

  for (int row = 0; row < MAX_BOARD_ROWS; ++row)
  {
    for (int column = 0; column < MAX_BOARD_COLUMNS; ++column)
    {
      uint32_t x_pos = column;
      uint32_t y_pos = row;

      board_to_fixed_pos(x_pos, y_pos);

      switch (board[((7-row) * 8 + column)])
      {
      // White pieces
      case WHITE_PAWN:
        set_img_size(x_pos, y_pos, 99, 99);
        SDL_RenderCopy(m_renderer, w_pawn_tex, &m_src_rect, &m_dest_rect);
      break;
      case WHITE_ROOK:
        set_img_size(x_pos, y_pos, 99, 99);
        SDL_RenderCopy(m_renderer, w_rook_tex, &m_src_rect, &m_dest_rect);
      break;
      case WHITE_KNIGHT:
        set_img_size(x_pos, y_pos, 99, 99);
        SDL_RenderCopy(m_renderer, w_knight_tex, &m_src_rect, &m_dest_rect);
      break;
      case WHITE_BISHOP:
        set_img_size(x_pos, y_pos, 99, 99);
        SDL_RenderCopy(m_renderer, w_bishop_tex, &m_src_rect, &m_dest_rect);
      break;
      case WHITE_QUEEN:
        set_img_size(x_pos, y_pos, 99, 99);
        SDL_RenderCopy(m_renderer, w_queen_tex, &m_src_rect, &m_dest_rect);
      break;
      case WHITE_KING:
        set_img_size(x_pos, y_pos, 99, 99);
        SDL_RenderCopy(m_renderer, w_king_tex, &m_src_rect, &m_dest_rect);
      break;

    // Black pieces
      case BLACK_PAWN:
        set_img_size(x_pos, y_pos, 99, 99);
        SDL_RenderCopy(m_renderer, b_pawn_tex, &m_src_rect, &m_dest_rect);
      break;
      case BLACK_ROOK:
        set_img_size(x_pos, y_pos, 99, 99);
        SDL_RenderCopy(m_renderer, b_rook_tex, &m_src_rect, &m_dest_rect);
      break;
      case BLACK_KNIGHT:
        set_img_size(x_pos, y_pos, 99, 99);
        SDL_RenderCopy(m_renderer, b_knight_tex, &m_src_rect, &m_dest_rect);
      break;
      case BLACK_BISHOP:
        set_img_size(x_pos, y_pos, 99, 99);
        SDL_RenderCopy(m_renderer, b_bishop_tex, &m_src_rect, &m_dest_rect);
      break;
      case BLACK_QUEEN:
        set_img_size(x_pos, y_pos, 99, 99);
        SDL_RenderCopy(m_renderer, b_queen_tex, &m_src_rect, &m_dest_rect);
      break;
      case BLACK_KING:
        set_img_size(x_pos, y_pos, 99, 99);
        SDL_RenderCopy(m_renderer, b_king_tex, &m_src_rect, &m_dest_rect);
      break;

      case NONE:
      break;

      default:
        FATAL_ERROR(ERROR_INVALID_VARIABLE,
          "Invalid variable (%i) in chess board array!",
          board[row * 8 + column]);
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
      if (event.button.button == SDL_BUTTON_LEFT)
      {
        if (is_piece_grabbed == false)
        {
          is_piece_grabbed = true;

          selected_piece.x = (uint32_t)event.button.x;
          selected_piece.y = (uint32_t)event.button.y;

          fixed_to_board_pos(selected_piece.x, selected_piece.y);
        }
        else if (is_piece_grabbed == true)
        {
          is_piece_grabbed = false;
          uint32_t x_dest = (uint32_t)event.button.x;
          uint32_t y_dest = (uint32_t)event.button.y;

          fixed_to_board_pos(x_dest, y_dest);
          
          game_manager.move_piece(selected_piece.x,
                                  FLIP_Y_AXIS(selected_piece.y),
                                  x_dest,
                                  FLIP_Y_AXIS(y_dest));
        }
      }
    break;

    case SDL_QUIT:
      m_is_game_running = false;
    break;

    default:
      if (is_piece_grabbed == true)
      {
        field_selction_pos.x = (uint32_t)event.button.x;
        field_selction_pos.y = (uint32_t)event.button.y;

        fixed_to_board_pos(field_selction_pos.x, field_selction_pos.y);
        board_to_fixed_pos(field_selction_pos.x, field_selction_pos.y);

        
      }

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

// We need to convert event coordinators to board coordinators.
// We have to do it in such a way that every picture nicely coincides
// with the picture of the board.
//
//                          ATTENTION!
// In this method we are loosing event coordinators for ever.

inline void Game::fixed_to_board_pos(uint32_t &x, uint32_t &y) const
{
  x = (uint32_t)((x - 27) / 100);
  y = (uint32_t)((y - 27) / 100);

  return;
}

inline void Game::board_to_fixed_pos(uint32_t &x, uint32_t &y) const
{
  x = 27 + (x * 100);
  y = 27 + (y * 100);

  return;
}
