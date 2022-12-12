#pragma once
#include<SDL.h>
#include <SDL_ttf.h>
#include <queue>
#include "utils.hpp"
#include "config.hpp"
#include "Stage.h"
#undef main

GAME_MODULE_BEGIN



class Key_Throttle
{
	bool key_down_table[7]{ false };
	std::vector<Stage> stages;
	void HandleInput(Key input)
	{
		if (input.flag)
		{
			if (!key_down_table[(int)input.key])
			{
				for (Stage& stage : stages)
				{
					stage.HandleInput(input);
				}
				key_down_table[(int)input.key] = true;
			}
		}
		else
		{
			key_down_table[(int)input.key] = false;
		}
	}
};

class Game
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	Game() :quit(false) { Init(); };
	std::queue<Key> key_que;
	bool quit;
	static Game* game;
public:
	SDL_Renderer* GetRenderer() const
	{
		return renderer;
	}
	static void Destory();

	static Game& GetGame()
	{
		return *game;
	}
	Game(Game&) = delete;
	Game& operator=(const Game&) = delete;
	void Init();
	void Close();
	void EventHandle();
	void Start();
};

GAME_MODULE_END
