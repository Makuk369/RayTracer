#pragma once
#include <limits>
#include <random>
#include <SDL3/SDL.h>
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"

namespace RTUtilVars
{
    inline const float INFINITE_F = std::numeric_limits<float>::infinity();
    inline uint32_t GLOBAL_SEED = SDL_rand_bits();
    
} // namespace RTUtilVars

namespace RTUtils
{
    inline uint32_t Vec4ToARGB(glm::vec4 colorVec)
    {
        colorVec = glm::clamp(colorVec, glm::vec4{0.0f}, glm::vec4{1.0f});

        colorVec = glm::sqrt(colorVec); // color corection (linear to gamma)

        uint8_t r = (uint8_t)(colorVec.r * 255.0f);
        uint8_t g = (uint8_t)(colorVec.g * 255.0f);
        uint8_t b = (uint8_t)(colorVec.b * 255.0f);
        uint8_t a = (uint8_t)(colorVec.a * 255.0f);

        return ((a << 24) | (r << 16) | (g << 8) | b);
    }

    inline uint32_t PCGHash(uint32_t seed)
    {
        uint32_t state = seed * 747796405u + 2891336453u;
        uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
        return (word >> 22u) ^ word;
    }

    inline float RandomFloat()
    {
        uint32_t& seed = RTUtilVars::GLOBAL_SEED;
        seed = PCGHash(seed);
        return (float)seed / (float)std::numeric_limits<uint32_t>::max();
    }

    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
    inline glm::vec3 RandomSquare()
    {
        return glm::vec3{RandomFloat() - 0.5f, RandomFloat() - 0.5f, 0.0f};
    }

    // Returns random vec3, x,y,z are in range [-1,1]
    inline glm::vec3 RandomVec3()
    {
        return glm::vec3{RandomFloat() * 2.0f - 1.0f, RandomFloat() * 2.0f - 1.0f, RandomFloat() * 2.0f - 1.0f};
    }

    inline glm::vec3 RandomUnitVec3()
    {
        return glm::normalize(glm::vec3{RandomFloat() * 2.0f - 1.0f, RandomFloat() * 2.0f - 1.0f, RandomFloat() * 2.0f - 1.0f});
    }

    // Returned vec3 is normalized
    inline glm::vec3 RandomOnHemisphere(const glm::vec3& normal)
    {
        glm::vec3 v = glm::normalize(glm::vec3{RandomFloat() * 2.0f - 1.0f, RandomFloat() * 2.0f - 1.0f, RandomFloat() * 2.0f - 1.0f});
        if(glm::dot(v, normal) > 0.0f){ // In the same hemisphere as the normal
            return v;
        }else{
            return -v;
        }
    }

    inline bool isNearZeroVec(glm::vec3 vec)
    {
        float s = 1e-8f;
        return (glm::abs(vec.x) < s) && (glm::abs(vec.y) < s) && (glm::abs(vec.z) < s);
    }
    
} // namespace RTUtils

