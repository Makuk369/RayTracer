#include "headers/Camera.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

#include <SDL3/SDL_log.h>

Camera::Camera(float verticalFOV, float nearClip, float farClip, uint32_t screenWidth, uint32_t screenHeight)
	: mVerticalFOV(verticalFOV), mNearClip(nearClip), mFarClip(farClip), mViewportWidth(screenWidth), mViewportHeight(screenHeight)
{
	mForwardDirection = glm::vec3(0, 0, -1);
	mPosition = glm::vec3(0, 0, 1);

	RecalculateView();
	RecalculateRayDirections();
	SDL_Log("Camera init!\n");
}

void Camera::OnUpdate(SDL_Event inputEvent, float deltaTime)
{
	SDL_GetMouseState(&mMousePos.x, &mMousePos.y);
	glm::vec2 delta = (mMousePos - mLastMousePos) * 0.002f;
	mLastMousePos = mMousePos;

	if(inputEvent.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
        if(inputEvent.button.button == SDL_BUTTON_RIGHT){
            mRMBisHeld = true;
            SDL_HideCursor();
        }
	}
    else if (inputEvent.type == SDL_EVENT_MOUSE_BUTTON_UP){
        mRMBisHeld = false;
        SDL_ShowCursor();
    }

	if(!mRMBisHeld) return;

	bool moved = false;

	constexpr glm::vec3 upDirection(0.0f, -1.0f, 0.0f);
	glm::vec3 rightDirection = -glm::cross(mForwardDirection, upDirection);

	float speed = 5.0f;

	// Movement
    if(inputEvent.type == SDL_EVENT_KEY_DOWN){
        if(inputEvent.key.key == SDLK_W){
            mPosition += mForwardDirection * speed * deltaTime;
            moved = true;
        }
        else if(inputEvent.key.key == SDLK_S){
            mPosition -= mForwardDirection * speed * deltaTime;
            moved = true;
        }
        if(inputEvent.key.key == SDLK_D){
            mPosition += rightDirection * speed * deltaTime;
            moved = true;
        }
        else if(inputEvent.key.key == SDLK_A){
			mPosition -= rightDirection * speed * deltaTime;
            moved = true;
        }
        if(inputEvent.key.key == SDLK_SPACE){
            mPosition += upDirection * speed * deltaTime;
            moved = true;
        }
        else if(inputEvent.key.key == SDLK_C){
            mPosition -= upDirection * speed * deltaTime;
            moved = true;
        }
    }

	// Rotation
	if (delta.x != 0.0f || delta.y != 0.0f){
		float pitchDelta = delta.y * -GetRotationSpeed(); // around x-axis
		float yawDelta = delta.x * GetRotationSpeed(); // around y-axis

		glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, rightDirection), glm::angleAxis(-yawDelta, glm::vec3(0.f, 1.0f, 0.0f))));
		mForwardDirection = glm::rotate(q, mForwardDirection);

		moved = true;
	}

	if (moved){
		RecalculateView();
		RecalculateRayDirections();
	}
}

void Camera::RecalculateProjection()
{
	mProjection = glm::perspectiveFov(glm::radians(mVerticalFOV), (float)mViewportWidth, (float)mViewportHeight, mNearClip, mFarClip);
	mInverseProjection = glm::inverse(mProjection);
}

void Camera::RecalculateView()
{
	mView = glm::lookAt(mPosition, mPosition + mForwardDirection, glm::vec3(0, -1, 0));
	mInverseView = glm::inverse(mView);
}

void Camera::RecalculateRayDirections()
{
	SDL_Log("Racalculating rays!\n");

	mRayDirections.resize(mViewportWidth * mViewportHeight);

	for (uint32_t y = 0; y < mViewportHeight; y++){
		for (uint32_t x = 0; x < mViewportWidth; x++){

			glm::vec2 coord = { (float)x / (float)mViewportWidth, (float)y / (float)mViewportHeight };
			coord = coord * 2.0f - 1.0f; // -1 - 1

			glm::vec4 target = mInverseProjection * glm::vec4(coord.x, coord.y, 1, 1);
			glm::vec3 rayDirection = glm::vec3(mInverseView * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0)); // World space
			mRayDirections[x + y * mViewportWidth] = rayDirection;
		}
	}
}