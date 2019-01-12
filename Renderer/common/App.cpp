#include "App.h"
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <iostream>

#define APPTITLE 		"Soft-Renderer"

using namespace std;

static long getCurrentMillSecond()
{
    long lLastTime = 0;
    struct timeval stCurrentTime;
    
    gettimeofday(&stCurrentTime,NULL);
    lLastTime = stCurrentTime.tv_sec * 1000 + stCurrentTime.tv_usec * 0.001; // milliseconds
    return lLastTime;
}

App::App() :
    window(nullptr),
    renderer(nullptr),
    _canvas(nullptr),
	running(false)
{
    _time = getCurrentMillSecond();
}

App::~App()
{
	OnCleanup();
}

int App::OnInit()
{
	// Initialize the SDL library.
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "SDL_Init() failed: %s\n", SDL_GetError());
		return APP_FAILED;
	}
	
    _canvas = Canvas::getInstance();
    
    _camara = Camera::getInstance();
    
	window = SDL_CreateWindow(APPTITLE,
							  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
							  _canvas->getWidth(), _canvas->getHeight(), 
							  SDL_WINDOW_SHOWN);
    
    SDL_Surface * surface = SDL_GetWindowSurface(window);
    
    _canvas->setSurface(surface);

	if (window != NULL) {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	}
	
	// Success
	return APP_OK;
}

void App::OnCleanup()
{
	if (renderer != NULL) {
		SDL_DestroyRenderer(renderer);	
	}
	if (window != NULL)	{
		SDL_DestroyWindow(window);
	}

	SDL_Quit();
}

void App::onKeyPress(SDL_Keycode keyCode , double dt) {
    auto camera = Camera::getInstance();
    auto v = 45;
    auto velo = camera->getMoveVelo();
    switch (keyCode) {
        case SDLK_ESCAPE:
            running = false;
            break;
        case SDLK_w:
            camera->offsetPosition(camera->forward() * velo * dt);
            break;
        case SDLK_s:
            camera->offsetPosition(-camera->forward() * velo * dt);
            break;
        case SDLK_a:
            camera->offsetPosition(-camera->right() * velo * dt);
            break;
        case SDLK_d:
            camera->offsetPosition(camera->right() * velo * dt);
            break;
        case SDLK_z:
            camera->offsetPosition(camera->up() * velo * dt);
            break;
        case SDLK_x:
            camera->offsetPosition(-camera->up() * velo * dt);
            break;
        case SDLK_UP:
            camera->offsetDirection(v * dt, 0);
            break;
        case SDLK_DOWN:
            camera->offsetDirection(-v * dt, 0);
            break;
        case SDLK_LEFT:
            camera->offsetDirection(0, -v*dt);
            break;
        case SDLK_RIGHT:
            camera->offsetDirection(0, v *dt);
            break;
        default:
            break;
    }
}

int App::OnExecute()
{
	// Initialize application.
	int state = OnInit();
	if (state != APP_OK) {
		return state;
	}
	
	// Enter the SDL event loop.
	SDL_Event event;

	running = true;
    char * pTitle = new char[100]();
	while (running)
	{
        auto curTime = getCurrentMillSecond();
        auto dt = (curTime - _time) * 1.0f / 1000;
        while (SDL_PollEvent(&event)) {
        	OnEvent(&event , dt);
        }
        
		OnUpdate(dt);
        OnRender(dt);
        sprintf(pTitle , "%d fps" , int(1 / dt));
        SDL_SetWindowTitle(window ,pTitle);
        _time = curTime;
	}

	return state;
}

void App::OnEvent(SDL_Event* event , double dt)
{
	switch (event->type)
	{
		case SDL_QUIT:
			running = false;
			break;
			
		case SDL_KEYDOWN:
            onKeyPress(event->key.keysym.sym , dt);
            break;
		default:
			break;
	}
}

void App::OnUpdate(double dt)
{
	// Update your game logic here
}

void App::OnRender(double dt)
{
    SDL_RenderClear(renderer);

    _canvas->update(dt);
    
    SDL_UpdateWindowSurface(window);
	
    SDL_RenderPresent(renderer);
}

