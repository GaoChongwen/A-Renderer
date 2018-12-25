// ============================================================================
// [Include Section]
// ============================================================================
#include "CApp.h"
#include <iostream>

// ============================================================================
// [Defines & Constants]
// ============================================================================
#define TITLE 		"Test-Renderer"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;


CApp::CApp():window(nullptr),renderer(nullptr),_canvas(nullptr), running(false){}

CApp::~CApp()
{
	OnCleanup();
}

int CApp::OnInit()
{
	// 启动SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout<<(stderr, "SDL_Init() failed: %s\n", SDL_GetError())<<std::endl;
		return APP_FAILED;
	}

	// 设置画布
	_canvas = new Canvas(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	SDL_Surface* surface = SDL_GetWindowSurface(window);

	_canvas->setSurface(surface);

	// 设置窗口
	window = SDL_CreateWindow(TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (window != NULL) {
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	}

	// 成功
	return APP_OK;
}

void CApp::OnCleanup()
{
	if (renderer != NULL) {
		SDL_DestroyRenderer(renderer);
	}
	if (window != NULL) {
		SDL_DestroyWindow(window);
	}

	SDL_Quit();
}

int CApp::OnExecute()
{
	// 初始化App
	int state = OnInit();
	if (state != APP_OK) {
		return state;
	}

	// Enter the SDL event loop.
	SDL_Event event;

	running = true;

	while (running)
	{
		while (SDL_PollEvent(&event)) {
			OnEvent(&event);
		}

		OnUpdate();
		OnRender();
	}

	return state;
}

void CApp::OnEvent(SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_QUIT:
		running = false;
		break;

	case SDL_KEYDOWN:
		if (event->key.keysym.sym == SDLK_ESCAPE) {
			running = false;
		}
	default:
		break;
	}
}

void CApp::OnUpdate()
{
	// Update your game logic here
}

void CApp::OnRender()
{
	SDL_RenderClear(renderer);

	_canvas->update();

	SDL_UpdateWindowSurface(window);

	// Do your drawing here

	SDL_RenderPresent(renderer);
}
