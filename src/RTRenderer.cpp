#include "headers/RTRenderer.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"
#include "headers/Settings.hpp"
#include "headers/Utils.hpp"

RTRenderer::RTRenderer(SDL_Surface* surface, const Camera& camera)
	: mSurface(surface), mCamera(camera)
{}

void RTRenderer::Render(const Scene& scene){
	SDL_LockSurface(mSurface);
	SDL_PixelFormat pixelFormat = mSurface->format;
	const SDL_PixelFormatDetails *pixelFormatDetails = SDL_GetPixelFormatDetails(pixelFormat);
	uint8_t* pixelArray = (uint8_t*)mSurface->pixels;

	for (int y = 0; y < mSurface->h; y++){
		for (int x = 0; x < mSurface->w; x++){

			Ray ray;
			ray.origin = mCamera.GetPosition();
			ray.direction = mCamera.GetRayDirections()[x + y * mSurface->w];

			glm::vec4 color = TraceRay(ray, scene);
			Uint32 * const targetPixel = (Uint32 *) ((Uint8 *) mSurface->pixels + y * mSurface->pitch + x * pixelFormatDetails->bytes_per_pixel);
			*targetPixel = RTUtils::Vec4ToARGB(color);
		}
	}
	SDL_UnlockSurface(mSurface);
}

void RTRenderer::RenderAntiAliased(const Scene& scene){
	SDL_LockSurface(mSurface);
	SDL_PixelFormat pixelFormat = mSurface->format;
	const SDL_PixelFormatDetails *pixelFormatDetails = SDL_GetPixelFormatDetails(pixelFormat);
	uint8_t* pixelArray = (uint8_t*)mSurface->pixels;

	float pixelSamplesScale = 1.0 / RTSetings::SAMPLES_PER_PIXEL;

	for (int y = 0; y < mSurface->h; y++){
		for (int x = 0; x < mSurface->w; x++){

			Ray ray;
			ray.origin = mCamera.GetPosition();
			
			glm::vec4 color{0.0f};

			for (int sample = 0; sample < RTSetings::SAMPLES_PER_PIXEL; sample++){
				ray.direction = mCamera.GetRayDirections()[sample + (x * RTSetings::SAMPLES_PER_PIXEL) + (y * mSurface->w * RTSetings::SAMPLES_PER_PIXEL)];

				color += TraceRay(ray, scene);
			}
			
			Uint32 * const targetPixel = (Uint32 *) ((Uint8 *) mSurface->pixels + y * mSurface->pitch + x * pixelFormatDetails->bytes_per_pixel);
			*targetPixel = RTUtils::Vec4ToARGB(color * pixelSamplesScale);
		}
	}
	SDL_UnlockSurface(mSurface);
}

// returns color of pixel (format = ARGB 0xff000000)
glm::vec4 RTRenderer::TraceRay(const Ray& ray, const Scene& scene)
{
	HitRecord hitRecord;

	if(scene.HitObjects(ray, Interval(0, RTUtilVars::INFINITE_F), hitRecord)){
		return 0.5f * glm::vec4{hitRecord.normal.x + 1, hitRecord.normal.y + 1, hitRecord.normal.z + 1 , 1.0f};
	}
	
	glm::vec3 normalizedRayDir = glm::normalize(ray.direction);
	float a = 0.5f * (normalizedRayDir.y + 1.0f);
	return (1.0f-a) * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) + a * glm::vec4(0.5f, 0.7f, 1.0f, 1.0f);
}