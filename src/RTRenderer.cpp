#include "headers/RTRenderer.hpp"
#include <execution>
#include <algorithm>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"
#include "headers/Settings.hpp"
#include "headers/Utils.hpp"
#include "headers/Material.hpp"

RTRenderer::RTRenderer(SDL_Surface* surface, const Camera& camera)
	: mSurface(surface), mCamera(camera)
{
	mPixelFormatDetails = SDL_GetPixelFormatDetails(mSurface->format);

	mHorizontalIter.resize(surface->w);
	mVerticalIter.resize(surface->h);

	for (int i = 0; i < surface->w; i++)
	{
		mHorizontalIter[i] = i;
	}
	for (int i = 0; i < surface->h; i++)
	{
		mVerticalIter[i] = i;
	}

	mAccumulationData.resize(mSurface->w * mSurface->h, glm::vec3{0.0f});
}

void RTRenderer::Render(const Scene& scene){
	SDL_LockSurface(mSurface);

	mCurrentScene = &scene;

	std::for_each(std::execution::par, mVerticalIter.begin(), mVerticalIter.end(),
	[this](int y)
	{
		for (int x = 0; x < mSurface->w; x++)
		{
			Ray ray;
			ray.origin = mCamera.GetPosition();
			ray.direction = mCamera.GetRayDirections()[x + y * mSurface->w];

			if (RTSetings::USE_ANTI_ALIASING){
				ray.direction += RTUtils::RandomSquare() * mCamera.GetPixelDelta();
			}

			// glm::vec3 color = PerPixel(ray, mMaxBounces);
			glm::vec3 color = PerPixel(ray);
			mAccumulationData[x + y * mSurface->w] += color;

			glm::vec3 accumulatedColor = mAccumulationData[x + y * mSurface->w];
			accumulatedColor /= (float)mFrameIndex;

			Uint32 * const targetPixel = (Uint32*) ((Uint8*) mSurface->pixels + y * mSurface->pitch + x * mPixelFormatDetails->bytes_per_pixel);
			*targetPixel = RTUtils::Vec4ToARGB(glm::vec4{accumulatedColor, 1.0f});
		}
	});

	mFrameIndex++;

	SDL_UnlockSurface(mSurface);
}

// returns color of pixel
// recursive
glm::vec3 RTRenderer::PerPixel(Ray& ray, int bounceCount)
{
	// If we've exceeded the ray bounce limit, no more light is gathered.
	if(bounceCount <= 0)
	{
		return glm::vec3{0.0f, 0.0f, 0.0f};
	}

	HitRecord hitRec;

	if(mCurrentScene->HitObjects(ray, Interval(0.0f, RTUtilVars::INFINITE_F), hitRec))
	{
		Ray scatteredRay;
		glm::vec3 color;
		if(hitRec.mat->Scatter(ray, hitRec, color, scatteredRay))
		{
			return color * PerPixel(scatteredRay, bounceCount-1);
		}
		return glm::vec3{0.0f, 0.0f, 0.0f};
	}

	return glm::vec3{1.0f, 1.0f, 1.0f}; // sky
}

// loop
glm::vec3 RTRenderer::PerPixel(Ray& ray)
{
	glm::vec3 finalColor{1.0f};
	glm::vec3 color;
	HitRecord hitRec;

	for (size_t bounce = 0; bounce < mMaxBounces; bounce++)
	{
		if(mCurrentScene->HitObjects(ray, Interval(0.0f, RTUtilVars::INFINITE_F), hitRec))
		{
			if(hitRec.mat->Scatter(ray, hitRec, color))
			{
				finalColor *= color;
				continue;
			}
		}

		// no hit
		finalColor *= glm::vec3{1.0f, 1.0f, 1.0f}; // sky
		break;
	}
	return finalColor;
}

void RTRenderer::Reset()
{
	std::fill(mAccumulationData.begin(), mAccumulationData.end(), glm::vec3{0.0f});
	mFrameIndex = 1;
}
