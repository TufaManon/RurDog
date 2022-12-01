#pragma once
#include "RurDog.h"
#include<SDL.h>
#undef main

class PlayerSence
{
public:
	const int NORMAL = 0;
	const int BEFORE_CLOCK = 1;
	const int AFTER_CLOCK = 2;
	Uint32 speed = 200;
	int action = NORMAL;
	Uint32 time;
	PlayerSence();
	Unit unit;
	void render(SDL_Renderer* renderer);
	void update(SDL_Renderer* renderer);
};

class Game
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	PlayerSence sence;
public:
	Game() { init(); };
	Game(Game&) = delete;
	const Game operator=(const Game&) const = delete;
	Game operator=(const Game&) = delete;
	void init();
	void close();
	void start();
};

