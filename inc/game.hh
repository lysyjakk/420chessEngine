#ifndef __GAME_H
#define __GAME_H

/* > Includes *****************************************************************/

#include <signal.h>
#include <atomic>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glu.h>
#include "../inc/traceAndError.hh"
#include "../inc/errorCodes.hh"
#include "../inc/consoleLogger.hh"
#include "../inc/textureManager.hh"

using namespace std;
using namespace chrono;

class Game
{
public:
  Game()  = default;
  ~Game() = default;

  void run_game();
  void init(const char* title,
            uint16_t    x_pos,
            uint16_t    y_pos,
            uint16_t    width,
            uint16_t    height,
            bool        fullscreen);

private:
  void game_loop();
  void render();

  bool m_is_game_running;

  //SDL attributes
  SDL_Window *m_window;
  SDL_Renderer *m_renderer;
  SDL_Rect m_src_rect;
  SDL_Rect m_dest_rect;
};

#endif //__GAME_H