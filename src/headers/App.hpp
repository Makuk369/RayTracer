#pragma once
#include <SDL3/SDL.h>

class App{
    public:
        App();
        ~App();
        
        void Run();
        
    private:
        SDL_Window* mWindow = nullptr;
        SDL_Surface* mWindowSurface = nullptr;
        SDL_Texture *mWindowTexture = nullptr;
};
