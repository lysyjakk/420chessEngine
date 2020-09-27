#ifndef PROMOTION_H_INCLUDED
#define PROMOTION_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "textureManager.hh"
#include "constants.hh"
#include <iostream>

#define PROMOTION_TO_QUEEN  0
#define PROMOTION_TO_ROOK   1
#define PROMOTION_TO_BISHOP 2
#define PROMOTION_TO_KNIGHT 3

enum class Site;

static int promotion_pawn(Site site)
{
  int result;

  /*if (site == (Site)0) // Only for white
  {
    SDL_Renderer *promotion_rend;
    SDL_Window *promotion_win;
    SDL_Rect srcRect, destRect;

    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
      promotion_win = SDL_CreateWindow("Promotion", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 410, 150, false);
      promotion_rend = SDL_CreateRenderer(promotion_win, -1, 0);

      if(promotion_rend)
      {
        SDL_SetRenderDrawColor(promotion_rend, 140, 140, 140, 255);
      }
    }

    SDL_Texture *queen  = TextureManager::LoadTexture(WHITE_QUEEN_TEX, promotion_rend);
    SDL_Texture *bishop = TextureManager::LoadTexture(WHITE_BISHOP_TEX, promotion_rend);
    SDL_Texture *rook   = TextureManager::LoadTexture(WHITE_ROOK_TEX, promotion_rend);
    SDL_Texture *knight = TextureManager::LoadTexture(WHITE_KNIGHT_TEX, promotion_rend);

    //Render promotion window
    SDL_RenderClear(promotion_rend);

    srcRect.w = 99;
    srcRect.h = 99;
    srcRect.x = 0;
    srcRect.y = 0;

    destRect.x = 0;
    destRect.y = 20;
    destRect.w = srcRect.w;
    destRect.h = srcRect.h;

    SDL_RenderCopy(promotion_rend, queen, &srcRect, &destRect);
    destRect.x += 100; 

    SDL_RenderCopy(promotion_rend, rook, &srcRect, &destRect);
    destRect.x += 100;

    SDL_RenderCopy(promotion_rend, bishop, &srcRect, &destRect);
    destRect.x += 100;

    SDL_RenderCopy(promotion_rend, knight, &srcRect, &destRect);
    SDL_RenderPresent(promotion_rend);

    SDL_Event event;
    bool is_promotion_event = true;

    while(is_promotion_event)
    {
      while (SDL_PollEvent(&event))
      {
        switch (event.type)
        {
          case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_LEFT)
            {
              result = (int)event.button.x / 100;
              is_promotion_event = false;
              SDL_DestroyRenderer(promotion_rend);
              SDL_DestroyWindow(promotion_win);
            }
          break;

          default:
          break;
        }
      }

      SDL_Delay(100);
    }
  }*/
  //else
  {
    result = PROMOTION_TO_QUEEN;
  }
  

  return result;
}

#endif //PROMOTION_H_INCLUDED