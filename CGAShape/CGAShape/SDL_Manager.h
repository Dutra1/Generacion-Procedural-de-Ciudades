#pragma once
#ifndef SDL_MANAGER
#define SDL_MANAGER

class SDL_Manager {
private:
	SDL_Window * window;
	SDL_GLContext context;
public:
	void initialize();
	void swap();
	void quit();
};

#endif