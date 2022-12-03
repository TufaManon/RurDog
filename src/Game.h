#pragma once
#include<SDL.h>
#include "Stage.h"
#undef main
GAME_MODULE_BEGIN

class Play_Stage : public Stage
{
public:
	Play_Field play_field;
	Play_Stage()
	{
		play_field.ConnectSignal(play_field.SIGNAL_TOUCH_OBSTACLE, std::bind(&Play_Stage::Next_Count, this));
	}
	bool auto_fall = true;
	uint64_t fall_time = 200;
	bool Auto_Fall(uint64_t time);
	void Next_Count();
	void UpdateContent() override;
	void CheckAutoFall();
};

class Game
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	Stage* stage;
	Game() { Init(); stage = new Play_Stage(); };
	static Game *game;
public:
	SDL_Renderer* GetRenderer() const
	{
		return renderer;
	}
	static void Destory()
	{
		game->Close();
		delete game;
	}
	static Game& GetGame()
	{
		return *game;
	}
	Game(Game&) = delete;
	Game& operator=(const Game&) = delete;
	void Init();
	void Close();
	void Start();
};


class Drawer
{
private:
	Drawer()
	{
		renderer = Game::GetGame().GetRenderer();
	}
	SDL_Renderer* renderer;
	static Drawer* drawer;
public:
	static Drawer& GetDrawer()
	{
		return *drawer;
	}
	static void Destory()
	{
		delete drawer;
	}
	void Present();
	Drawer(Drawer&) = delete;
	const Drawer& operator=(Drawer&) = delete;
	void SetDrawColor(RGBA color);
	void SetDrawColor(HSLA color);
	void DrawBrick(const Brick& brick, SDL_Texture* texture);
	void DrawBrick(const Brick& brick);
	SDL_Texture* DrawPlayField(const Play_Field& field);
	void DrawTextrue(SDL_Texture* texture,const SDL_Rect& src, const SDL_Rect& dest);
	void DrawTextrue(SDL_Texture* texture);
	void Clear()
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
	}
};


GAME_MODULE_END
