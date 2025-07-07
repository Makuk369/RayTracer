#include "headers/Game.hpp"
#include "headers/GameSettings.hpp"
#include "headers/Timer.hpp"

Game::Game()
	: mCamera(45.0f, 0.1f, 100.0f, RayTracerSetings::WINDOW_WIDTH, RayTracerSetings::WINDOW_HEIGHT)
{
	if(!SDL_Init(SDL_INIT_VIDEO)){
		SDL_Log( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return;
	}

	if((mWindow = SDL_CreateWindow("Ray Tracer", RayTracerSetings::WINDOW_WIDTH, RayTracerSetings::WINDOW_HEIGHT, 0)) == nullptr){
		SDL_Log( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return;
	}
	// if(!SDL_CreateWindowAndRenderer("Snake", RayTracerSetings::WINDOW_WIDTH, RayTracerSetings::WINDOW_HEIGHT, 0, &mWindow, &mRenderer)){
	// 	SDL_Log( "Window and mRTRenderer could not be created! SDL Error: %s\n", SDL_GetError() );
	// 	return;
	// }
	//Enable vsync
	// SDL_SetRenderVSync(mRTRenderer, 1);
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
	float mouseX = 0.f;
	float mouseY = 0.f;
	bool mouseIsPressed = false;

	Timer timer;
	timer.Start();
	float deltaTime = 0;
	unsigned int frameCount = 0;
	float fpsTimer = 1;
	float updateDelay = 1.f/RayTracerSetings::MAX_FPS;

	// ---------- MAIN GAME LOOP ----------
	while(isRunning){
		deltaTime = timer.getDeltaTime();

		while(SDL_PollEvent(&event) != 0){
			if(event.type == SDL_EVENT_QUIT){
				isRunning = false;
			}

			mCamera.OnUpdate(event, deltaTime);
    	}

		updateDelay -= deltaTime;
		if(updateDelay <= 0.f){
			updateDelay = 1.f/RayTracerSetings::MAX_FPS;
			frameCount++;

			//Clear screen
			// SDL_RenderClear(mRenderer);

			mRTRenderer.Render(mWindowSurface, mCamera);

			// SDL_RenderTexture(mRenderer, mWindowTexture, NULL, NULL);
			
			//Update screen
			SDL_UpdateWindowSurface(mWindow);
			// SDL_RenderPresent(mRenderer);
		}

		if(RayTracerSetings::SHOW_FPS){
			fpsTimer -= deltaTime;
			if(fpsTimer <= 0.f){
				fpsTimer = 1.f;
				SDL_Log("FPS = %u; %.2fms per frame\n", frameCount, (1.f/frameCount)*1000);
				frameCount = 0;
			}
		}
	}
}