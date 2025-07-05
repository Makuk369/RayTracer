#include "headers/RTRenderer.hpp"
#include "headers/Structures.hpp"

void RTRenderer::Render(SDL_Surface* surface){
	SDL_LockSurface(surface);
	SDL_PixelFormat pixelFormat = surface->format;
	const SDL_PixelFormatDetails *pixelFormatDetails = SDL_GetPixelFormatDetails(pixelFormat);
	uint8_t* pixelArray = (uint8_t*)surface->pixels;

	uint32_t color = 0xff000000; //ARGB

	// looping through every pixel
	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){

			Vec2 coord = {x / (float)surface->w, y / (float)surface->h};

			color = SDL_rand_bits();
			color |= 0xff000000;

			// ----- setting pixel color -----
			Uint32 * const target_pixel = (Uint32 *) ((Uint8 *) surface->pixels + y * surface->pitch + x*pixelFormatDetails->bytes_per_pixel);
  			*target_pixel = color;
			// -------------------------------
		}
	}

	SDL_UnlockSurface(surface);
}