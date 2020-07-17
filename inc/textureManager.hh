#ifndef TEXTUREMANAGER_H_INCLUDED
#define TEXTUREMANAGER_H_INCLUDED

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glu.h>

class TextureManager{
    public:
        static SDL_Texture* LoadTexture(const char* fileName, SDL_Renderer *renderer);
};


#endif // TEXTUREMANAGER_H_INCLUDED