#include "headers/RTRenderer.hpp"

// RTRenderer::RTRenderer(){

// }

void RTRenderer::Render(SDL_Surface* surface){
    SDL_LockSurface(surface);
	SDL_PixelFormat pixelFormat = surface->format;
	const SDL_PixelFormatDetails *pixelFormatDetails = SDL_GetPixelFormatDetails(pixelFormat);
	uint8_t* pixelArray = (uint8_t*)surface->pixels;

	// looping through every pixel
	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){

			glm::vec2 coord(x / (float)surface->w, 1 - y / (float)surface->h); // 0 - surface size -> 0 - 1
			coord = coord * 2.0f - 1.0f; // 0 - 1 -> -1 - 1

			// ----- setting pixels -----
			Uint32 * const target_pixel = (Uint32 *) ((Uint8 *) surface->pixels + y * surface->pitch + x*pixelFormatDetails->bytes_per_pixel);
  			*target_pixel = PerPixel(coord);
			// --------------------------
		}
	}

	SDL_UnlockSurface(surface);
}

// returns color of set pixel (format = ARGB 0xff000000)
uint32_t RTRenderer::PerPixel(glm::vec2 coord){

	glm::vec3 rayOrigin(0.0f, 0.0f, 2.0f);
	glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
	float radius = 0.5f;

	// (bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
	// where
	// a = ray origin
	// b = ray direction
	// r = radius
	// t = hit distance
	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	// Quadratic forumula discriminant:
	// b^2 - 4ac
	float discriminant = b * b - 4.0f * a * c;
	if (discriminant >= 0.0f){
		return 0xffff00ff;
	}

    return 0xff000000;
}