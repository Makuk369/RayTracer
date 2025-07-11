#include "headers/App.hpp"
#include "headers/Settings.hpp"
#include "headers/Camera.hpp"
#include "headers/RTRenderer.hpp"
#include "headers/Timer.hpp"
#include "headers/Sphere.hpp"

App::App()
{
	if(!SDL_Init(SDL_INIT_VIDEO)){
		SDL_Log( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return;
	}

	if((mWindow = SDL_CreateWindow("Ray Tracer", RTSetings::WINDOW_WIDTH, RTSetings::WINDOW_HEIGHT, 0)) == nullptr){
		SDL_Log( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return;
	}
	
	if((mWindowSurface = SDL_GetWindowSurface(mWindow)) == nullptr){
		SDL_Log("mWindowSurface is NULL! SDL Error: %s\n", SDL_GetError());
	}

	SDL_Log("Window size = %ux%u\n", RTSetings::WINDOW_WIDTH, RTSetings::WINDOW_HEIGHT);
	SDL_Log("App - init!\n");
}

App::~App(){
	SDL_DestroyWindow(mWindow);
	mWindow = nullptr;

	//Quit SDL subsystems
	SDL_Quit();
	SDL_Log("App - quit!\n");
}

void App::Run(){
	SDL_Log("App - running!\n");
	bool isRunning = true;

	SDL_Event event;
	// float mouseX = 0.f;
	// float mouseY = 0.f;
	// bool mouseIsPressed = false;

	Timer timer;
	timer.Start();
	float deltaTime = 0;
	unsigned int frameCount = 0;
	float fpsTimer = 1;
	float updateDelay = 1.f/RTSetings::MAX_FPS;

	Camera camera(mWindowSurface);
	RTRenderer renderer(mWindowSurface, camera);

	Scene scene1;
	scene1.Add(std::make_shared<Sphere>(glm::vec3{0.0f, 0.0f, -1.0f}, 0.5f));
	// scene1.Add(std::make_shared<Sphere>(glm::vec3{0.0f, 2.5f, -1.0f}, 2.0f));
	scene1.Add(std::make_shared<Sphere>(glm::vec3{0.0f, -100.5f, -1.0f}, 100.0f));

	// ---------- MAIN GAME LOOP ----------
	while(isRunning){
		deltaTime = timer.getDeltaTime();

		while(SDL_PollEvent(&event) != 0){
			if(event.type == SDL_EVENT_QUIT){
				isRunning = false;
			}

			// mCamera.OnUpdate(event, deltaTime);
    	}

		updateDelay -= deltaTime;
		if(updateDelay <= 0.f){
			updateDelay = 1.f/RTSetings::MAX_FPS;
			frameCount++;

			if(RTSetings::USE_ANTI_ALIASING){
				renderer.RenderAntiAliased(scene1);
			}else{
				renderer.Render(scene1);
			}
			
			//Update screen
			SDL_UpdateWindowSurface(mWindow);
		}

		if(RTSetings::SHOW_FPS){
			fpsTimer -= deltaTime;
			if(fpsTimer <= 0.f){
				fpsTimer = 1.f;
				SDL_Log("FPS = %u; %.2fms per frame\n", frameCount, (1.f/frameCount)*1000);
				frameCount = 0;
			}
		}
	}
}