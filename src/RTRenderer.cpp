#include "headers/RTRenderer.hpp"

RTRenderer::RTRenderer(SDL_Surface* surface, const Camera& camera)
	: mSurface(surface), mCamera(camera)
{}

void RTRenderer::Render(){
	SDL_LockSurface(mSurface);
	SDL_PixelFormat pixelFormat = mSurface->format;
	const SDL_PixelFormatDetails *pixelFormatDetails = SDL_GetPixelFormatDetails(pixelFormat);
	uint8_t* pixelArray = (uint8_t*)mSurface->pixels;

	for (int y = 0; y < mSurface->h; y++){
		for (int x = 0; x < mSurface->w; x++){

			Ray ray;
			ray.origin = mCamera.GetPosition();
			ray.direction = mCamera.GetRayDirections()[x + y * mSurface->w];

			glm::vec4 color = TraceRay(ray);
			Uint32 * const targetPixel = (Uint32 *) ((Uint8 *) mSurface->pixels + y * mSurface->pitch + x * pixelFormatDetails->bytes_per_pixel);
			*targetPixel = ConvertVec4ToARGB(color);
		}
	}
	SDL_UnlockSurface(mSurface);
}

// returns color of set pixel (format = ARGB 0xff000000)
glm::vec4 RTRenderer::TraceRay(const Ray& ray)
{
	float t = CalcRayHit(ray);
	
	if(t >= 0){
		return glm::vec4(1, 0, 0, 1);
	}
	
	glm::vec3 normalizedRayDir = glm::normalize(ray.direction);
	float a = 0.5*(normalizedRayDir.y + 1.0);
	return (1.0f-a)*glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) + a*glm::vec4(0.5f, 0.7f, 1.0f, 1.0f);

	// glm::vec3 hitPoint = ray.origin + closestT * ray.direction;
	
	// glm::vec normal = glm::normalize(hitPoint - sphereOrigin);
	// return 0.5f * glm::vec4(normal.x + 1.0f, normal.y + 1.0f, normal.z + 1.0f, 1);

	// return glm::vec4(1, 0, 0, 1);
}

float RTRenderer::CalcRayHit(const Ray& ray)
{
	// Sphere formula (quadratic)
	glm::vec3 oc = mSphere.position - ray.origin;
	float a = glm::dot(ray.direction, ray.direction);
	float b = -2.0f * glm::dot(ray.direction, oc);
	float c = glm::dot(oc, oc) - mSphere.radius * mSphere.radius;

	// Quadratic forumula
	float discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0.0f) { // ray missed
        return -1.0;
    } else {
        return (-b - std::sqrt(discriminant)) / (2.0f * a); // closest t
    }
}

uint32_t RTRenderer::ConvertVec4ToARGB(const glm::vec4 colorVec)
{
	uint8_t r = (uint8_t)(colorVec.r * 255.0f);
	uint8_t g = (uint8_t)(colorVec.g * 255.0f);
	uint8_t b = (uint8_t)(colorVec.b * 255.0f);
	uint8_t a = (uint8_t)(colorVec.a * 255.0f);

	return ((a << 24) | (r << 16) | (g << 8) | b);
}