#include "headers/RTRenderer.hpp"
#include "RTRenderer.hpp"

void RTRenderer::Render(SDL_Surface* surface){
	SDL_LockSurface(surface);
	SDL_PixelFormat pixelFormat = surface->format;
	const SDL_PixelFormatDetails *pixelFormatDetails = SDL_GetPixelFormatDetails(pixelFormat);
	uint8_t* pixelArray = (uint8_t*)surface->pixels;

	// looping through every pixel
	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){

			Vec2 coord = {x / (float)surface->w, y / (float)surface->h}; // 0-surface size -> 0-1

			// ----- setting pixels -----
			Uint32 * const target_pixel = (Uint32 *) ((Uint8 *) surface->pixels + y * surface->pitch + x*pixelFormatDetails->bytes_per_pixel);
  			*target_pixel = PerPixel(coord);
			// --------------------------
		}
	}

	SDL_UnlockSurface(surface);
}

// returns color of set pixel (format = ARGB 0xff000000)
uint32_t RTRenderer::PerPixel(Vec2 coord){
	uint8_t r = (uint8_t)(coord.x * 255.0f);
	uint8_t g = (uint8_t)(coord.y * 255.0f);
    return 0xff000000 | (r << 16) | (g << 8);
}