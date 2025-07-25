#include "headers/Camera.hpp"
#include "headers/Utils.hpp"
#include "headers/Settings.hpp"

Camera::Camera(SDL_Surface* surface)
	:mSurfaceWidth(surface->w), mSurfaceHeight(surface->h)
{
	mViewportWidth = mViewportHeight * (float(surface->w)/surface->h);

	mViewportU = glm::vec3{mViewportWidth, 0.0f, 0.0f};
	mViewportV = glm::vec3{0.0f, -mViewportHeight, 0.0f};

	mPixelDeltaU = mViewportU / (float)surface->w;
	mPixelDeltaV = mViewportV / (float)surface->h;

	mViewportUpperLeft = mPosition - glm::vec3(0, 0, mFocalLength) - mViewportU/2.0f - mViewportV/2.0f;
	mPixel00Location = mViewportUpperLeft + 0.5f * (mPixelDeltaU + mPixelDeltaV);

	RecalculateRayDirections();
}

void Camera::RecalculateRayDirections(){
	mRayDirections.resize(mSurfaceWidth * mSurfaceHeight);

	for (int y = 0; y < mSurfaceHeight; y++){
		for (int x = 0; x < mSurfaceWidth; x++){
			glm::vec3 pixelCenter = mPixel00Location + ((float)x * mPixelDeltaU) + ((float)y * mPixelDeltaV);
			mRayDirections[x + y * mSurfaceWidth] = pixelCenter - mPosition;
		}
	}
}