#pragma once
#include "glm/glm.hpp"
#include <SDL3/SDL_events.h>
#include <vector>

class Camera{
    public:
        Camera(float verticalFOV, float nearClip, float farClip, uint32_t screenWidth, uint32_t screenHeight);
        ~Camera() = default;

        void OnUpdate(SDL_Event inputEvent, float deltaTime);

        const glm::mat4& GetProjection() const { return mProjection; }
        const glm::mat4& GetInverseProjection() const { return mInverseProjection; }
        const glm::mat4& GetView() const { return mView; }
        const glm::mat4& GetInverseView() const { return mInverseView; }
        
        const glm::vec3& GetPosition() const { return mPosition; }
        const glm::vec3& GetDirection() const { return mForwardDirection; }

        const std::vector<glm::vec3>& GetRayDirections() const { return mRayDirections; }

        float GetRotationSpeed() { return 0.3f; };

    private:
        void RecalculateProjection();
        void RecalculateView();
        void RecalculateRayDirections();

        glm::mat4 mProjection{ 1.0f };
        glm::mat4 mView{ 1.0f };
        glm::mat4 mInverseProjection{ 1.0f };
        glm::mat4 mInverseView{ 1.0f };

        float mVerticalFOV = 45.0f;
        float mNearClip = 0.1f;
        float mFarClip = 100.0f;

        glm::vec3 mPosition{0.0f, 0.0f, 0.0f};
        glm::vec3 mForwardDirection{0.0f, 0.0f, 0.0f};

        // Cached ray directions
        std::vector<glm::vec3> mRayDirections;

        bool mRMBisHeld = false;
        glm::vec2 mMousePos{0.0f};
        glm::vec2 mLastMousePos{0.0f, 0.0f};

        uint32_t mViewportWidth = 0, mViewportHeight = 0;
};
