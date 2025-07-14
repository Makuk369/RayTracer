#include "headers/RTRenderer.hpp"
#include <execution>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"
#include "headers/Settings.hpp"
#include "headers/Utils.hpp"

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

			glm::vec4 color{0.0f};

			color += PerPixel(ray);
			Uint32 * const targetPixel = (Uint32 *) ((Uint8 *) mSurface->pixels + y * mSurface->pitch + x * mPixelFormatDetails->bytes_per_pixel);
			*targetPixel = RTUtils::Vec4ToARGB(color);
		}
	});

	SDL_UnlockSurface(mSurface);
}

void RTRenderer::RenderAntiAliased(const Scene& scene){
	SDL_LockSurface(mSurface);

	mCurrentScene = &scene;

	float pixelSamplesScale = 1.0 / RTSetings::SAMPLES_PER_PIXEL;

	std::for_each(std::execution::par, mVerticalIter.begin(), mVerticalIter.end(),
	[this, pixelSamplesScale](int y)
	{
		for (int x = 0; x < mSurface->w; x++)
		{
			Ray ray;
			
			glm::vec4 color{0.0f};
			
			for (int sample = 0; sample < RTSetings::SAMPLES_PER_PIXEL; sample++){
				ray.origin = mCamera.GetPosition();
				ray.direction = mCamera.GetRayDirections()[sample + (x * RTSetings::SAMPLES_PER_PIXEL) + (y * mSurface->w * RTSetings::SAMPLES_PER_PIXEL)];

				color += PerPixel(ray);
			}
			
			Uint32 * const targetPixel = (Uint32 *) ((Uint8 *) mSurface->pixels + y * mSurface->pitch + x * mPixelFormatDetails->bytes_per_pixel);
			*targetPixel = RTUtils::Vec4ToARGB(color * pixelSamplesScale);
		}
	});

	SDL_UnlockSurface(mSurface);
}

// returns color of pixel (format = ARGB 0xff000000)
glm::vec4 RTRenderer::PerPixel(Ray& ray)
{
	HitRecord hitRecord;

	uint32_t randSeed = SDL_rand_bits();

	// glm::vec3 normalizedRayDir = glm::normalize(ray.direction);
	// float a = 0.5f * (normalizedRayDir.y + 1.0f);
	// glm::vec3 color = (1.0f-a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f); // sky color

	glm::vec3 color{0.0f};
	glm::vec3 colorContribution{1.0f};

	for (size_t bounce = 0; bounce < mMaxBounces; bounce++){
		if(!mCurrentScene->HitObjects(ray, Interval(0.0f, RTUtilVars::INFINITE_F), hitRecord)){  // didn't hit (hit sky) == no more bounces
			// return 0.5f * glm::vec4{hitRecord.normal.x + 1, hitRecord.normal.y + 1, hitRecord.normal.z + 1 , 1.0f};
			// glm::vec3 direction = RTUtils::RandomOnHemisphere(randSeed, hitRecord.normal);
			// return 0.5f * PerPixel(Ray{hitRecord.position, direction}, scene);
			// return 0.5f * PerPixel(Ray{hitRecord.position + (hitRecord.normal * 0.001f), direction}, scene);

			color = glm::vec3{0.7f, 0.8f, 1.0f} * colorContribution; // sky color = glm::vec3{0.6f, 0.7f, 0.9f}
			break;
		}
		colorContribution *= glm::vec3{0.5f, 0.5f, 0.5f};

		ray.origin = hitRecord.position + hitRecord.normal * 0.0001f;
		ray.direction = hitRecord.normal + glm::normalize(RTUtils::RandomVec3(randSeed));
	}
	return glm::vec4{color, 1.0f};
}