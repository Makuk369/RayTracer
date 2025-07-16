#include "headers/RTRenderer.hpp"
#include <execution>
#include <iostream>
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

			color += PerPixel(ray, mMaxBounces);
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

				color += PerPixel(ray, mMaxBounces);
			}
			
			Uint32 * const targetPixel = (Uint32 *) ((Uint8 *) mSurface->pixels + y * mSurface->pitch + x * mPixelFormatDetails->bytes_per_pixel);
			*targetPixel = RTUtils::Vec4ToARGB(color * pixelSamplesScale);
		}
	});

	SDL_UnlockSurface(mSurface);
}

// returns color of pixel (format = ARGB 0xff000000)
/*
glm::vec4 RTRenderer::PerPixel(Ray& ray)
{
	HitRecord hitRecord;

	uint32_t randSeed = SDL_rand_bits();

	glm::vec3 color{0.0f};
	glm::vec3 colorContribution{1.0f};
	glm::vec3 finalColor{1.0f};

	for (size_t bounce = 0; bounce < mMaxBounces; bounce++){
		if(!mCurrentScene->HitObjects(ray, Interval(0.0f, RTUtilVars::INFINITE_F), hitRecord)){  // didn't hit (hit sky) == no more bounces

			finalColor *= glm::vec3{0.7f, 0.8f, 1.0f} * colorContribution; // sky color = glm::vec3{0.7f, 0.8f, 1.0f}
			break;
		}
		// ray.origin = hitRecord.position + hitRecord.normal * 0.0001f;

		// ray.direction = hitRecord.normal + glm::normalize(RTUtils::RandomVec3(randSeed));

		// ray.direction = glm::reflect(ray.direction, hitRecord.normal + RTUtils::RandomVec3(randSeed) * 0.0f);
		// if(glm::dot(ray.direction, hitRecord.normal) < 0){
		// 	color = glm::vec3{0.7f, 0.8f, 1.0f} * colorContribution; // sky color = glm::vec3{0.6f, 0.7f, 0.9f}
		// 	break;
		// }

		if(!hitRecord.mat->Scatter(ray, hitRecord, color)) // didnt manage to scatter
		{
			finalColor *= glm::vec3{0.1f};
		}
		finalColor *= color * colorContribution;
		
		colorContribution *= glm::vec3{0.5f}; // color loss after bounce
	}
	return glm::vec4{finalColor, 1.0f};
}
*/

glm::vec4 RTRenderer::PerPixel(Ray& ray, int bounceCount)
{
	// If we've exceeded the ray bounce limit, no more light is gathered.
	if(bounceCount <= 0)
	{
		return glm::vec4{0.0f, 0.0f, 0.0f, 1.0f};
	}

	HitRecord hitRec;

	if(mCurrentScene->HitObjects(ray, Interval(0.0f, RTUtilVars::INFINITE_F), hitRec))
	{
		Ray scatteredRay;
		glm::vec4 color;
		if(hitRec.mat->Scatter(ray, hitRec, color, scatteredRay))
		{
			return color * PerPixel(scatteredRay, bounceCount-1);
		}
		return glm::vec4{0.0f, 0.0f, 0.0f, 1.0f};
	}

	return glm::vec4{0.7f, 0.8f, 1.0f, 1.0f}; // sky
}
