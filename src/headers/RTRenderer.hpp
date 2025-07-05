#pragma once
#include <SDL3/SDL.h>
#include "glm/glm.hpp"

class RTRenderer{
    public:
        RTRenderer() = default;

        void Render(SDL_Surface* surface);

    private:
        uint32_t PerPixel(glm::vec2 coord);
};
