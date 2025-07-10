#pragma once
#include <SDL3/SDL.h>
#include "glm/glm.hpp"
#include "Camera.hpp"
#include "Ray.hpp"
#include "Scene.hpp"

class RTRenderer{
    public:
        RTRenderer(SDL_Surface* surface, const Camera& camera);

        void Render(const Scene& scene);
        void RenderAntiAliased(const Scene& scene);

    private:
        glm::vec4 TraceRay(const Ray& ray, const Scene& scene);

        SDL_Surface* mSurface = nullptr;
        const Camera mCamera;
};
