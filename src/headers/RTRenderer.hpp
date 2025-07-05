#pragma once
#include <SDL3/SDL.h>
#include "Structures.hpp"

class RTRenderer{
    public:
        RTRenderer() = default;

        void Render(SDL_Surface* surface);

    private:
        uint32_t PerPixel(Vec2 coord);
};
