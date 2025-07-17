#include "headers/App.hpp"
#include "headers/Settings.hpp"
#include "headers/Material.hpp"
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
	uint32_t frameCount = 0;
	float fpsTimer = 1.0f;
	float updateDelay = 1.f/RTSetings::MAX_FPS;

	Timer renderTimer;
	uint64_t renderTimeTotal = 0; // ms

	Camera camera(mWindowSurface);
	RTRenderer renderer(mWindowSurface, camera);

	// Materials
	// std::shared_ptr<Material> groundMat = std::make_shared<Lambertian>(glm::vec3{1.0f, 0.0f, 0.0f});
	// std::shared_ptr<Material> greenMat = std::make_shared<Lambertian>(glm::vec3{0.0f, 1.0f, 0.0f});
	// std::shared_ptr<Material> blueMat = std::make_shared<Lambertian>(glm::vec3{0.0f, 0.0f, 1.0f});
	// std::shared_ptr<Material> metalMat1 = std::make_shared<Metal>(glm::vec3{0.8f, 0.8f, 0.8f}, 0.3f);
	// std::shared_ptr<Material> rightMat = std::make_shared<Metal>(glm::vec3{0.8f, 0.6f, 0.2f}, 1.0f);

	std::shared_ptr<Material> material_ground = std::make_shared<Lambertian>(glm::vec3{0.8f, 0.8f, 0.0f});
	std::shared_ptr<Material> material_center = std::make_shared<Lambertian>(glm::vec3{0.1f, 0.2f, 0.5f});
	std::shared_ptr<Material> material_left = std::make_shared<Metal>(glm::vec3{0.8f, 0.8f, 0.8f}, 0.01f);
	std::shared_ptr<Material> material_right = std::make_shared<Metal>(glm::vec3{0.8f, 0.6f, 0.2f}, 1.0f);

	Scene scene1;
	// scene1.Add(std::make_shared<Sphere>(glm::vec3{0.0f, -100.5f, -1.0f}, 100.0f, groundMat));
	// scene1.Add(std::make_shared<Sphere>(glm::vec3{-0.6f, -0.1f, -1.2f}, 0.5f, metalMat1));
	// scene1.Add(std::make_shared<Sphere>(glm::vec3{0.6f, 0.0f, -1.2f}, 0.5f, blueMat));
	// scene1.Add(std::make_shared<Sphere>(glm::vec3{-1.0f, 0.0f, -1.0f}, 0.5f, metalMat1));
	// scene1.Add(std::make_shared<Sphere>(glm::vec3{1.0f, 0.0f, -1.0f}, 0.5f, rightMat));

	scene1.Add(std::make_shared<Sphere>(glm::vec3{ 0.0f, -100.5f, -1.0f}, 100.0f, material_ground));
    scene1.Add(std::make_shared<Sphere>(glm::vec3{ 0.0f,    0.0f, -1.2f},   0.5f, material_center));
    scene1.Add(std::make_shared<Sphere>(glm::vec3{-1.0f,    0.0f, -1.0f},   0.5f, material_left));
    scene1.Add(std::make_shared<Sphere>(glm::vec3{ 1.0f,    0.0f, -1.0f},   0.5f, material_right));

	// ---------- MAIN GAME LOOP ----------
	while(isRunning)
	{
		deltaTime = timer.getDeltaTime();

		while(SDL_PollEvent(&event) != 0)
		{
			if(event.type == SDL_EVENT_QUIT){
				isRunning = false;
			}
			else if(event.type == SDL_EVENT_KEY_DOWN)
			{
				if(event.key.key == SDLK_ESCAPE){
					isRunning = false;
				}
				if(event.key.key == SDLK_R){
					renderer.Reset();
				}
				if(event.key.key == SDLK_T){
					RTSetings::USE_ANTI_ALIASING = !RTSetings::USE_ANTI_ALIASING;
					SDL_Log("Anti-Aliasing set to: %s\n", RTSetings::USE_ANTI_ALIASING ? "on" : "off");
				}
			}

			// mCamera.OnUpdate(event, deltaTime);
    	}

		updateDelay -= deltaTime;
		if(updateDelay <= 0.f)
		{
			updateDelay = 1.f/RTSetings::MAX_FPS;
			frameCount++;

			renderTimer.Start();
			renderer.Render(scene1);
			renderTimeTotal += renderTimer.getTicks();
			
			//Update screen
			SDL_UpdateWindowSurface(mWindow);
		}

		if(RTSetings::SHOW_FPS)
		{
			fpsTimer -= deltaTime;
			if(fpsTimer <= 0.f)
			{
				fpsTimer = 1.f;
				SDL_Log("FPS = %u; %.2fms per render\n", frameCount, (float)renderTimeTotal / (float)frameCount);
				frameCount = 0;
				renderTimeTotal = 0;
			}
		}
	}
}