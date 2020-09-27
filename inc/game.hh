#ifndef __GAME_H
#define __GAME_H

/* > Includes *****************************************************************/

#include <signal.h>
#include <atomic>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glu.h>
#include "traceAndError.hh"
#include "errorCodes.hh"
#include "consoleLogger.hh"
#include "textureManager.hh"
#include "gameManager.hh"
#include "promotion.hh"

using namespace std;
using namespace chrono;

class Game
{
public:
  Game()  = default;
  ~Game() = default;

  void run_game();
  void init(const char* title,
            uint32_t    x_pos,
            uint32_t    y_pos,
            uint32_t    width,
            uint32_t    height,
            bool        fullscreen);

private:
  void game_loop();
  void render();
  void place_all_pieces();
  void handle_event();

  void set_img_size(uint32_t x_pos,
                    uint32_t y_pos,
                    uint32_t width,
                    uint32_t height);

  inline void fixed_to_board_pos(uint32_t &x, uint32_t &y) const;
  inline void board_to_fixed_pos(uint32_t &x, uint32_t &y) const;

  void clean() const;

  bool m_is_game_running;

  //SDL attributes
  SDL_Window *m_window;
  SDL_Renderer *m_renderer;
  SDL_Rect m_src_rect;
  SDL_Rect m_dest_rect;
};

#endif //__GAME_H