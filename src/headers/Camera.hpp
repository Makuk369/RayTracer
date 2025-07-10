#pragma once
#include "glm/glm.hpp"
#include <SDL3/SDL.h>
#include <vector>

class Camera
{
    public:
        Camera(SDL_Surface* surface);

        const glm::vec3& GetPosition() const { return mPosition; };
        const std::vector<glm::vec3>& GetRayDirections() const { return mRayDirections; }
        const glm::vec3& GetViewportU() const { return mViewportU; };
        const glm::vec3& GetViewportV() const { return mViewportV; };

    private:
        void RecalculateRayDirections();
        void RecalculateRayDirectionsAntiAliased();

        int mSurfaceWidth = 0;
        int mSurfaceHeight = 0;

        float mFocalLength = 1.0f;
        float mViewportHeight = 2.0f;
        float mViewportWidth = 0.0f;

        glm::vec3 mPosition{0.0f, 0.0f, 0.0f};

        glm::vec3 mViewportU{0.0f};
        glm::vec3 mViewportV{0.0f};

        glm::vec3 mPixelDeltaU{0.0f};
        glm::vec3 mPixelDeltaV{0.0f};

        glm::vec3 mViewportUpperLeft{0.0f};
        glm::vec3 mPixel00Location{0.0f};

        std::vector<glm::vec3> mRayDirections;
};
