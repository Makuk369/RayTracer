#include "headers/RTRenderer.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"
#include "headers/GameSettings.hpp"

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
			*targetPixel = Vec4ToARGB(color);
		}
	}
	SDL_UnlockSurface(mSurface);
}

// returns color of pixel (format = ARGB 0xff000000)
glm::vec4 RTRenderer::TraceRay(const Ray& ray, const Scene& scene)
{
	HitRecord hitRecord;

	if(scene.HitObjects(ray, Interval(0, RayTracerSetings::INFINITE_F), hitRecord)){
		return 0.5f * glm::vec4{hitRecord.normal.x + 1, hitRecord.normal.y + 1, hitRecord.normal.z + 1 , 1.0f};
	}
	
	glm::vec3 normalizedRayDir = glm::normalize(ray.direction);
	float a = 0.5f * (normalizedRayDir.y + 1.0f);
	return (1.0f-a) * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) + a * glm::vec4(0.5f, 0.7f, 1.0f, 1.0f);
}

uint32_t RTRenderer::Vec4ToARGB(const glm::vec4 colorVec)
{
	uint8_t r = (uint8_t)(colorVec.r * 255.0f);
	uint8_t g = (uint8_t)(colorVec.g * 255.0f);
	uint8_t b = (uint8_t)(colorVec.b * 255.0f);
	uint8_t a = (uint8_t)(colorVec.a * 255.0f);

	return ((a << 24) | (r << 16) | (g << 8) | b);
}