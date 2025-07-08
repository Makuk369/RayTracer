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

	// tut stuff

	// Camera
    auto focal_length = 1.0f;
    auto viewport_height = 2.0f;
    auto viewport_width = viewport_height * (float(mWindowSurface->w)/mWindowSurface->h);
    auto camera_center = glm::vec3{0, 0, 0};

	// Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = glm::vec3(viewport_width, 0, 0);
    auto viewport_v = glm::vec3(0, -viewport_height, 0);

	// Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / (float)mWindowSurface->w;
    auto pixel_delta_v = viewport_v / (float)mWindowSurface->h;

	// Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center - glm::vec3(0, 0, focal_length) - viewport_u/2.0f - viewport_v/2.0f;
    auto pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);

	// ---------

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

			// mRTRenderer.Render(mWindowSurface, mCamera);
			SDL_LockSurface(mWindowSurface);
			SDL_PixelFormat pixelFormat = mWindowSurface->format;
			const SDL_PixelFormatDetails *pixelFormatDetails = SDL_GetPixelFormatDetails(pixelFormat);
			uint8_t* pixelArray = (uint8_t*)mWindowSurface->pixels;

			for (int y = 0; y < mWindowSurface->h; y++){
				for (int x = 0; x < mWindowSurface->w; x++){
					auto pixel_center = pixel00_loc + ((float)x * pixel_delta_u) + ((float)y * pixel_delta_v);
            		auto ray_direction = pixel_center - camera_center;

					Ray ray;
					ray.origin = camera_center;
					ray.direction = ray_direction;

					glm::vec4 color = ray_color(ray);
					Uint32 * const target_pixel = (Uint32 *) ((Uint8 *) mWindowSurface->pixels + y * mWindowSurface->pitch + x * pixelFormatDetails->bytes_per_pixel);
					*target_pixel = ConvertVec4ToARGB(color);
				}
			}
			SDL_UnlockSurface(mWindowSurface);

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

uint32_t Game::ConvertVec4ToARGB(const glm::vec4 colorVec){
	uint8_t r = (uint8_t)(colorVec.r * 255.0f);
	uint8_t g = (uint8_t)(colorVec.g * 255.0f);
	uint8_t b = (uint8_t)(colorVec.b * 255.0f);
	uint8_t a = (uint8_t)(colorVec.a * 255.0f);

	return ((a << 24) | (r << 16) | (g << 8) | b);
}

glm::vec4 Game::ray_color(const Ray& r) {
    glm::vec3 unit_direction = glm::normalize(r.direction);
    float a = 0.5*(unit_direction.y + 1.0);
    return (1.0f-a)*glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) + a*glm::vec4(0.5f, 0.7f, 1.0f, 1.0f);
}