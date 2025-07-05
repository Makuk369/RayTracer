#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

class Game{
    public:
        Game();
        ~Game();
        
        void Run();
        
    private:
        SDL_Window* mWindow = nullptr;
        SDL_Surface* mWindowSurface = nullptr;
        SDL_Texture *mWindowTexture = nullptr;
        SDL_Renderer* mRenderer = nullptr;
};
