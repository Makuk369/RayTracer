#pragma once
#include <SDL3/SDL.h>
#include "glm/glm.hpp"

namespace RTUtils
{
    inline uint32_t Vec4ToARGB(glm::vec4 colorVec)
    {
        colorVec = glm::clamp(colorVec, 0.0f, 1.0f);

        uint8_t r = (uint8_t)(colorVec.r * 255.0f);
        uint8_t g = (uint8_t)(colorVec.g * 255.0f);
        uint8_t b = (uint8_t)(colorVec.b * 255.0f);
        uint8_t a = (uint8_t)(colorVec.a * 255.0f);

        return ((a << 24) | (r << 16) | (g << 8) | b);
    }

    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
    inline glm::vec3 RandomSquare()
    {
        return glm::vec3{SDL_randf() - 0.5f, SDL_randf() - 0.5f, 0.0f};
    }
    
} // namespace RTUtils

namespace RTUtilVars
{
    inline const float INFINITE_F = std::numeric_limits<float>::infinity();
    
} // namespace RTUtilVars

