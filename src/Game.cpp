#include "Game.h"

const Global_Config Global_Config::config;

GAME_MODULE_BEGIN
Game* Game::game = new Game();
void Game::Destory()
{
	game->Close();
	delete game;
}

inline void ErrorAbort(const char* msg)
{
	SDL_LogError(1, msg);
	SDL_Log("SDL ERROR IS IN HERE: %s", SDL_GetError());
	exit(1);
}

void Game::Init()

{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		ErrorAbort("can't init SDL by SDL_Init func");
	}
	window = SDL_CreateWindow(GAME_NAME,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		Global_Config::config.window_config.width,
		Global_Config::config.window_config.height,
		SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		ErrorAbort("can't create window by SDL_CreateWindow func");
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == nullptr)
	{
		ErrorAbort("can't create renderer by SDL_CreateRenderer");
	}

}

void Game::Close()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::EventHandle()
{
	SDL_Event event;

	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_QUIT)
		{
			quit = true;
		}
		else if (event.type == SDL_EventType::SDL_KEYDOWN)
		{
			switch (event.key.keysym.scancode)
			{
			case SDL_SCANCODE_A:
				key_que.push({ true,_Vaild_Input::LEFT });
				break;
			case SDL_SCANCODE_S:
				key_que.push({ true,_Vaild_Input::DOWN });
				break;
			case SDL_SCANCODE_D:
				key_que.push({ true,_Vaild_Input::RIGHT });
				break;
			case SDL_SCANCODE_W:
				key_que.push({ true,_Vaild_Input::UP });
				break;
			case SDL_SCANCODE_J:
				key_que.push({ true,_Vaild_Input::OK });
				break;
			case SDL_SCANCODE_K:
				key_que.push({ true,_Vaild_Input::CANCEL });
				break;
			case SDL_SCANCODE_ESCAPE:
				key_que.push({ true,_Vaild_Input::MENU });
				break;
			default:
				break;
			}
		}
	}
}

void Game::Start()
{
	while (!quit)
	{
		EventHandle();
	}
}

GAME_MODULE_END

