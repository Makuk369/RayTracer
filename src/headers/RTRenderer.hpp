#pragma once
#include <SDL3/SDL.h>
#include "glm/glm.hpp"
#include "Camera.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"

class RTRenderer{
    public:
        RTRenderer(SDL_Surface* surface, const Camera& camera);

        void Render();

    private:
        glm::vec4 TraceRay(const Ray& ray);
        float CalcRayHit(const Ray& ray); // returns t (hit distance)
        uint32_t ConvertVec4ToARGB(const glm::vec4 colorVec);

        SDL_Surface* mSurface = nullptr;
        const Camera mCamera;

        Sphere mSphere{glm::vec3{0.0f, 0.0f, -1.0f}, 0.5f};
};
