#include "headers/RTRenderer.hpp"

// RTRenderer::RTRenderer(){

// }

void RTRenderer::Render(SDL_Surface* surface, const Camera& camera){
    SDL_LockSurface(surface);
	SDL_PixelFormat pixelFormat = surface->format;
	const SDL_PixelFormatDetails *pixelFormatDetails = SDL_GetPixelFormatDetails(pixelFormat);
	uint8_t* pixelArray = (uint8_t*)surface->pixels;

	Ray ray;
	ray.origin = camera.GetPosition();
	
	// looping through every pixel
	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			
			// glm::vec2 coord(x / (float)surface->w, 1 - y / (float)surface->h); // 0 - surface size -> 0 - 1
			// coord = coord * 2.0f - 1.0f; // 0 - 1 -> -1 - 1
			ray.direction = camera.GetRayDirections()[x + y * surface->w];
			
			// ----- setting pixels -----
			Uint32 * const target_pixel = (Uint32 *) ((Uint8 *) surface->pixels + y * surface->pitch + x * pixelFormatDetails->bytes_per_pixel);
			glm::vec4 color = TraceRay(ray);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
  			*target_pixel = ConvertVec4ToARGB(color);
			// --------------------------
		}
	}

	SDL_UnlockSurface(surface);
}

// returns color of set pixel (format = ARGB 0xff000000)
glm::vec4 RTRenderer::TraceRay(const Ray& ray)
{
	float radius = 0.5f;
	glm::vec3 sphereOrigin{0.0f, 0.0f, -1.0f};

	// Sphere formula (quadratic)
	glm::vec3 oc = sphereOrigin - ray.origin;
	float a = glm::dot(ray.direction, ray.direction);
	float b = -2.0f * glm::dot(ray.direction, oc);
	float c = glm::dot(oc, oc) - radius * radius;

	// Quadratic forumula discriminant:
	float discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0.0f){ // ray missed
		return glm::vec4(0.5, 0.8, 1, 1);
	}

	// Quadratic formula
	float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);

	glm::vec3 hitPoint = ray.origin + closestT * ray.direction;
	
	glm::vec normal = glm::normalize(hitPoint - sphereOrigin);
	return 0.5f * glm::vec4(normal.x + 1.0f, normal.y + 1.0f, normal.z + 1.0f, 1);

	// return glm::vec4(1, 0, 0, 1);
	/*
	glm::vec3 hitPoint = oc + ray.direction * closestT;
	glm::vec3 normal = glm::normalize(hitPoint);

	glm::vec3 lightDir = glm::normalize(glm::vec3(-1, 1, 1));

	float d = glm::max(glm::dot(normal, -lightDir), 0.0f); // == cos(angle)

	// normal * 0.5f + 0.5f  (-1 - 1 -> 0 - 1)
	glm::vec4 sphereColor(1.0f, 0.0f, 1.0f, 1.0f);
	sphereColor *= d;
    return sphereColor;
	*/
}

uint32_t RTRenderer::ConvertVec4ToARGB(const glm::vec4 colorVec){
	uint8_t r = (uint8_t)(colorVec.r * 255.0f);
	uint8_t g = (uint8_t)(colorVec.g * 255.0f);
	uint8_t b = (uint8_t)(colorVec.b * 255.0f);
	uint8_t a = (uint8_t)(colorVec.a * 255.0f);

	return ((a << 24) | (r << 16) | (g << 8) | b);
}