#include "headers/Game.hpp"
#include "headers/GameSettings.hpp"
#include "headers/Timer.hpp"

Game::Game(){
	if(!SDL_Init(SDL_INIT_VIDEO)){
		SDL_Log( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return;
	}

	if((mWindow = SDL_CreateWindow("Ray Tracer", RayTracerSetings::gWindowWidth, RayTracerSetings::gWindowHeight, 0)) == nullptr){
		SDL_Log( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return;
	}
	// if(!SDL_CreateWindowAndRenderer("Snake", RayTracerSetings::gWindowWidth, RayTracerSetings::gWindowHeight, 0, &mWindow, &mRenderer)){
	// 	SDL_Log( "Window and renderer could not be created! SDL Error: %s\n", SDL_GetError() );
	// 	return;
	// }
	//Enable vsync
	// SDL_SetRenderVSync(renderer, 1);
	// SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255); // white = 255, 255, 255, 255
	
	if((mWindowSurface = SDL_GetWindowSurface(mWindow)) == nullptr){
		SDL_Log("mWindowSurface is NULL! SDL Error: %s\n", SDL_GetError());
	}

	if(!TTF_Init()){
		SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s\n", SDL_GetError());
	}

	SDL_Log("Game - init!\n");
}

Game::~Game(){
	// SDL_DestroyRenderer(mRenderer);
	// mRenderer = nullptr;
	SDL_DestroyWindow(mWindow);
	mWindow = nullptr;

	//Quit SDL subsystems
	SDL_Quit();
	TTF_Quit();
	SDL_Log("Game - quit!\n");
}

void Game::Run(){
	SDL_Log("Game - running!\n");
	bool isRunning = true;

	SDL_Event event;

	Timer timer;
	timer.Start();
	float deltaTime = 0;
	const float MAX_FPS = 60;
	unsigned int frameCount = 0;
	float fpsTimer = 1;
	float updateDelay = 1/MAX_FPS;

	float mouseX = 0.f;
	float mouseY = 0.f;
	bool mouseIsPressed = false;

	// ---------- MAIN GAME LOOP ----------
	while(isRunning){
		while(SDL_PollEvent(&event) != 0){
			if(event.type == SDL_EVENT_QUIT){
				isRunning = false;
			}
			else if(event.type == SDL_EVENT_MOUSE_MOTION || event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP){
				if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
					mouseIsPressed = true;
				}
				if(event.type == SDL_EVENT_MOUSE_BUTTON_UP){
					mouseIsPressed = false;
				}
				if(mouseIsPressed){
					//Get mouse position
					SDL_GetMouseState(&mouseX, &mouseY);
				}
			}
			else if(event.type == SDL_EVENT_KEY_DOWN){
				if(event.key.key == SDLK_SPACE){
					// if(mWindowTexture != nullptr){
					// 	SDL_DestroyTexture(mWindowTexture);
					// 	mWindowTexture = nullptr;
					// }

					// mWindowTexture = SDL_CreateTextureFromSurface(mRenderer, mWindowSurface);
					// if(mWindowTexture == nullptr){
					// 	SDL_Log("Unable to create texture from mWindowSurface! SDL Error: %s\n", SDL_GetError());
					// }
					// else{
					// 	SDL_Log("Updated mWindowTexture!\n");
					// }
				}
			}
		}

		deltaTime = timer.getDeltaTime();
		updateDelay -= deltaTime;
		if(updateDelay <= 0.f){
			// SDL_Log("update %f!\n", updateDelay);
			updateDelay = 1/MAX_FPS;
			frameCount++;

			//Clear screen
			// SDL_RenderClear(mRenderer);

			Render(mWindowSurface);

			// SDL_RenderTexture(mRenderer, mWindowTexture, NULL, NULL);
			
			//Update screen
			SDL_UpdateWindowSurface(mWindow);
			// SDL_RenderPresent(mRenderer);
		}

		fpsTimer -= deltaTime;
		if(fpsTimer <= 0.f){
			fpsTimer = 1.f;
			SDL_Log("FPS = %u\n", frameCount);
			frameCount = 0;
		}
	}
}

void Game::Render(SDL_Surface* surface){
	SDL_LockSurface(mWindowSurface);
	SDL_PixelFormat pixelFormat = surface->format;
	const SDL_PixelFormatDetails *pixelFormatDetails = SDL_GetPixelFormatDetails(pixelFormat);
	uint8_t* pixelArray = (uint8_t*)surface->pixels;

	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;

	// looping through every pixel
	for (int x = 0; x < surface->w; x++){
		for (int y = 0; y < surface->h; y++){
			r = (uint8_t)SDL_rand(256);
			g = (uint8_t)SDL_rand(256);
			b = (uint8_t)SDL_rand(256);
			pixelArray[y*surface->pitch + x*pixelFormatDetails->bytes_per_pixel + 0] = b; // b
			pixelArray[y*surface->pitch + x*pixelFormatDetails->bytes_per_pixel + 1] = g; // g
			pixelArray[y*surface->pitch + x*pixelFormatDetails->bytes_per_pixel + 2] = r; // r
		}
	}

	SDL_UnlockSurface(mWindowSurface);
}