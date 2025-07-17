#pragma once
#include <vector>
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

        void Reset();

    private:
        glm::vec3 PerPixel(Ray& ray, int bounceCount);
        glm::vec3 PerPixel(Ray& ray);

        SDL_Surface* mSurface = nullptr;
        const SDL_PixelFormatDetails *mPixelFormatDetails = nullptr;

        const Camera mCamera;
        const Scene* mCurrentScene = nullptr;

        int mMaxBounces = 5;

        std::vector<int> mHorizontalIter;
        std::vector<int> mVerticalIter;

        std::vector<glm::vec3> mAccumulationData;
        uint32_t mFrameIndex = 1;
};
