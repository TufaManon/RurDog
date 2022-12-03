#include "Game.h"
#include "config.h"

GAME_MODULE_BEGIN
Game* Game::game = new Game();
Drawer* Drawer::drawer = new Drawer();
void Game::Init()

{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_Log(ERROR, "Can't init SDL");
		SDL_Log(SDL_GetError());
		exit(1);
	}
	window = SDL_CreateWindow(GAME_NAME,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		SDL_Log(ERROR,"Can't create window");
		SDL_Log(SDL_GetError());
		exit(1);
	}
	renderer = SDL_CreateRenderer(window,-1,0);
	if (renderer == nullptr)
	{
		SDL_Log(ERROR,"Can't create renderer");
		SDL_Log(SDL_GetError());
		exit(1);
	}
}

void Game::Close()
{
	delete stage;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::Start()
{
	SDL_Event event;
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT)
				quit = true;
		}
		stage->Update();
	}
}

inline void Drawer::Present()
{
	SDL_RenderPresent(renderer);
}

inline void Drawer::SetDrawColor(RGBA color)
{
	SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha * 255);
}

inline void Drawer::SetDrawColor(HSLA color)
{
	SetDrawColor(color.ToRGBA());
}

void Drawer::DrawBrick(const Brick& brick, SDL_Texture* texture)
{
	SDL_SetRenderTarget(renderer, texture);
	DrawBrick(brick);
}

void Drawer::DrawBrick(const Brick& brick)
{	
	HSLA hsla(brick.color);
	SetDrawColor(hsla);
	SDL_Rect rect;
	rect.x = brick.relative_coord.x * PLAYER_BRICK_SIZE;
	rect.y = brick.relative_coord.y * PLAYER_BRICK_SIZE;
	rect.w = PLAYER_BRICK_SIZE / 2;
	rect.h = PLAYER_BRICK_SIZE / 2;
	SDL_RenderFillRect(renderer, &rect);
	hsla.saturation *= 0.8;
	SetDrawColor(hsla);
	rect.x += PLAYER_BRICK_SIZE / 2;
	rect.y += PLAYER_BRICK_SIZE / 2;
	SDL_RenderFillRect(renderer, &rect);
	hsla.lightness *= 0.95;
	SetDrawColor(hsla);
	rect.y -= PLAYER_BRICK_SIZE / 2;
	SDL_RenderFillRect(renderer, &rect);
	SetDrawColor(hsla);
	hsla.hue *= 0.95;
	rect.y += PLAYER_BRICK_SIZE / 2;
	rect.x -= PLAYER_BRICK_SIZE / 2;
	SDL_RenderFillRect(renderer, &rect);

}

SDL_Texture* Drawer::DrawPlayField(const Play_Field& field)
{
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
		SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET, PLAYER_BRICK_SIZE * 10, PLAYER_BRICK_SIZE * 22);
	SDL_SetRenderTarget(renderer, texture);
	for (int y = 0; y < 22; y++)
		for (int x = 0; x < 10; x++)
			DrawBrick(field.field_brick[y][x]);
	auto falling = field.GetFalling();
	for (const auto& brick : falling)
		DrawBrick(brick);
	SDL_SetRenderTarget(renderer, NULL);
	return texture;
}

void Drawer::DrawTextrue(SDL_Texture* texture, const SDL_Rect& s, const SDL_Rect& t)
{
	SDL_RenderCopy(renderer, texture, &s, &t);
}

void Drawer::DrawTextrue(SDL_Texture* texture)
{
	SDL_Rect rect;
	rect.x = 30;
	rect.y = 20;
	rect.w = 10 * 32;
	rect.h = 22 * 32;
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}


bool Play_Stage::Auto_Fall(uint64_t time)
{
	if (time >= fall_time)
	{
		play_field.Down();
		auto_fall = true;
		return true;
	}
	return false;
}

void Play_Stage::Next_Count()
{
	play_field.Locking();
}

void Play_Stage::UpdateContent()
{
	CheckAutoFall();
	auto texture = Drawer::GetDrawer().DrawPlayField(play_field);

	Drawer::GetDrawer().Clear();
	Drawer::GetDrawer().DrawTextrue(texture);
	Drawer::GetDrawer().Present();

	SDL_DestroyTexture(texture);
}
void Play_Stage::CheckAutoFall()
{	
	if (auto_fall)
	{
		UnblockingAct(std::bind(&Play_Stage::Auto_Fall, this, std::placeholders::_1), fall_time);
		auto_fall = false;
	}
}
GAME_MODULE_END

