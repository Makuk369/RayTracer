#pragma once
#include <SDL3/SDL.h>
#include "Camera.hpp"
#include "RTRenderer.hpp"

class Game{
    public:
        Game();
        ~Game();
        
        void Run();
        
    private:
        SDL_Window* mWindow = nullptr;
        SDL_Surface* mWindowSurface = nullptr;
        SDL_Texture *mWindowTexture = nullptr;
};
