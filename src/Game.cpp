#include "Game.h"

int main(int flagc, char* flags[])
{
	Game game;
	game.start();
	game.close();
	return 0;
}

void Game::init()

{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_Log("Can't init SDL");
		SDL_Log(SDL_GetError());
		exit(1);
	}
	window = SDL_CreateWindow(GAME_NAME,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		SDL_Log("Can't create window");
		SDL_Log(SDL_GetError());
		exit(1);
	}
	renderer = SDL_CreateRenderer(window,-1,0);
	if (renderer == nullptr)
	{
		SDL_Log("Can't create render");
		SDL_Log(SDL_GetError());
		exit(1);
	}
}

void Game::close()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::start()
{
	SDL_Event event;
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
				quit = true;
		}
		SDL_SetRenderDrawColor(renderer, 55, 55, 55, 1);
		SDL_RenderClear(renderer);
		sence.update(renderer);
		SDL_RenderPresent(renderer);
	}
}

PlayerSence::PlayerSence()
{
	time = 0;
	unit.region.coord.x = 80;
	unit.region.coord.y = 80;
	unit.region.h = 20;
	unit.region.w = 10;
	unit.region.tile_size = 24;
}

void PlayerSence::render(SDL_Renderer* renderer)
{
	SDL_Rect rect;
	rect.x = unit.region.coord.x;
	rect.y = unit.region.coord.y;
	rect.w = unit.region.w * unit.region.tile_size;
	rect.h = unit.region.h * unit.region.tile_size;
	SDL_SetRenderDrawColor(renderer, 123, 2, 123, 1);
	SDL_RenderDrawRect(renderer, &rect);
	rect.w = unit.region.tile_size;
	rect.h = unit.region.tile_size;
	for (int y = 0; y < 23; y++)
		for (int x = 0; x < 10; x++)
		{
			auto block = unit.world[y][x];
			if (block.region_coord.y > 2)
			{
				rect.x = unit.region.coord.x + block.region_coord.x * unit.region.tile_size;
				rect.y = unit.region.coord.y + (block.region_coord.y - 3) * unit.region.tile_size;
				SDL_SetRenderDrawColor(renderer, block.color.r, block.color.g, block.color.b, block.color.a);
				SDL_RenderFillRect(renderer, &rect);
				if (block.empty_block)
				{
					SDL_SetRenderDrawColor(renderer, 55, 55, 55, 1);
					SDL_RenderDrawRect(renderer, &rect);
				}
			}
		}
	if (unit.HasActor())
	{
		for (int i = 0; i < 4; i++)
		{
			if (unit.actor->blocks[i].region_coord.y > 2)
			{
				rect.x = unit.region.coord.x + unit.actor->blocks[i].region_coord.x * unit.region.tile_size;
				rect.y = unit.region.coord.y + (unit.actor->blocks[i].region_coord.y - 3) * unit.region.tile_size;
				SDL_SetRenderDrawColor(renderer, 
					unit.actor->blocks[i].color.r, 
					unit.actor->blocks[i].color.g, 
					unit.actor->blocks[i].color.b, 
					unit.actor->blocks[i].color.a);
				SDL_RenderFillRect(renderer, &rect);
				SDL_RenderDrawRect(renderer, &rect);

			}
		}
	}

}

void PlayerSence::update(SDL_Renderer* renderer)
{
	if (SDL_GetTicks64() - time > speed && action == NORMAL)
	{
		if (unit.HasActor())
		{
			unit.ActorFall();
			
			time = SDL_GetTicks64();
		}

		else unit.CreatActor();

		for (auto it = unit.signal.begin(); it != unit.signal.end();)
		{
			if (*it == unit.FALL_COMPELET)
			{
				this->action = BEFORE_CLOCK;
				time = SDL_GetTicks64();
				it = unit.signal.erase(it);
			}
			else it++;
		}
	}
	else if (action == BEFORE_CLOCK)
	{
		//listen move and rotation
		if (SDL_GetTicks64() - time > 1000)
		{
			unit.LockActor();
			action = NORMAL;
		}
	}
	render(renderer);
}
