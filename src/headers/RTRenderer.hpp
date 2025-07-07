#pragma once
#include <SDL3/SDL.h>
#include "glm/glm.hpp"
#include "Camera.hpp"

class RTRenderer{
    public:
        RTRenderer() = default;

        void Render(SDL_Surface* surface, const Camera& camera);

    private:
        glm::vec4 PerPixel(glm::vec2 coord);
        uint32_t ConvertVec4ToARGB(const glm::vec4 colorVec);
};
