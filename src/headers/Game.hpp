#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
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
        SDL_Renderer* mRenderer = nullptr;

        Camera mCamera;
        RTRenderer mRTRenderer;

        uint32_t ConvertVec4ToARGB(const glm::vec4 colorVec);
        glm::vec4 ray_color(const Ray& r);
};
