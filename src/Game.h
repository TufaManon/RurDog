#pragma once
#include<SDL.h>
#include "Stage.h"
#include <SDL_ttf.h>
#undef main

GAME_MODULE_BEGIN
enum class _KEY_TYPE
{
	DOWN_DOWN, DWON_UP, LEFT_DOWN, LEFT_UP, RIGHT_DOWN, RIGHT_UP, HOLD_DOWN, R_DOWN, L_DOWN, R_UP, L_UP, HOLD_UP, DROP_DOWN, DROP_UP
};

class Play_Stage
{
	const int CHECK_ARR = 1;
	const int CHECK_AUTO_SHIFT = 1 << 1;
	const int CHECK_LOCK = 1 << 2;
	const int CHECK_DORP = 1 << 3;
	const int CHECK_SOFT_DROP = 1 << 4;
	int gravity = 2;
	Uint64 current_tick64 = 0;
	int MAX_RESET_LOCK_DELAY = 10;
	int reset_lock_delay_counter = 0;
	int lock_delay_counter = 0;
	int das_counter = 0;
	int arr_counter = 0;
	int drop_counter = 0;
	int soft_drop_counter = 0;
	int state = 0;
	int pressed_keys = 0;
	const int PRESSED_RIGHT = 1 << 0;
	const int PRESSED_LEFT = 1 << 1;
	const int PRESSED_R = 1 << 2;
	const int PRESSED_L = 1 << 3;
	const int PRESSED_DOWN = 1 << 4;
	const int PRESSED_DRAP = 1 << 5;
	const int PRESSED_HOLD = 1 << 6;
	Play_Field field;

	inline void Lock()
	{
		Debug("LOCK THE FALLING MINO");
		field.Lock();
		TryToCancelCountARR();
		TryToCancelCountDAS();
		TryToCancelCountDrop();
		TryToCancelCountLock();
		TryToCancelCountSoftDrop();
		StartDropCount();
	}

	bool Pressed(int flag)
	{
		return (pressed_keys & flag) != 0;
	}

	void Release(int flag)
	{
		pressed_keys = pressed_keys ^ flag;
	}

	void SetPressed(int flag)
	{
		pressed_keys |= flag;
	}

	inline void CancelCount(int f, int& v)
	{
		state = state ^ f;
		v = 0;
	}

	inline bool HasState(int f)
	{
		return (state & f) != 0;
	}

	inline void TryToCancelCountLock()
	{
		if (HasState(CHECK_LOCK))
		{
			CancelCount(CHECK_LOCK, lock_delay_counter);
			reset_lock_delay_counter = 0;
		}
	}
	inline void TryToCancelCountSoftDrop()
	{
		if (HasState(CHECK_SOFT_DROP))
			CancelCount(CHECK_SOFT_DROP, soft_drop_counter);
	}
	inline void TryToCancelCountDAS()
	{
		if (HasState(CHECK_AUTO_SHIFT))
			CancelCount(CHECK_AUTO_SHIFT, das_counter);
	}

	inline void TryToCancelCountARR()
	{
		if (HasState(CHECK_ARR))
			CancelCount(CHECK_ARR, arr_counter);
	}

	inline void TryToCancelCountDrop()
	{
		if (HasState(CHECK_DORP))
			CancelCount(CHECK_DORP, drop_counter);
	}

	inline void Move(_Ori ori)
	{
		if (reset_lock_delay_counter >= MAX_RESET_LOCK_DELAY)
		{
			return;
		}
		auto res = field.Move(ori);
		lock_delay_counter = 0;
		if (HasState(CHECK_LOCK) && res)
		{
			reset_lock_delay_counter++;
			lock_delay_counter = 0;
		}

	}

	inline void Rotating(_Ori ori)
	{
		if (reset_lock_delay_counter >= MAX_RESET_LOCK_DELAY)
		{
			return;
		}
		auto res = field.Rotate(ori);
		if (HasState(CHECK_LOCK) && res)
		{
			reset_lock_delay_counter++;
			lock_delay_counter = 0;
		}
	}

	inline void Drop()
	{
		auto res = field.Drop();
		if (HasState(CHECK_LOCK) && res)
		{
			CancelCount(CHECK_LOCK, lock_delay_counter);
		}
	}

	inline void HardDrop()
	{
		while (field.Drop());
		Lock();
	}

	inline void Hold()
	{
		if (field.Swap())
		{
			Debug("hold swap");
		}

	}

	inline void Check()
	{
		if (das_counter >= DAS)
		{
			StartARRCount();
			TryToCancelCountDAS();
			//Debug("Start Count ARR");
		}

		if (arr_counter >= ARR)
		{
			if (Pressed(PRESSED_RIGHT))
				Move(_Ori::R);
			else if (Pressed(PRESSED_LEFT))
				Move(_Ori::L);
			arr_counter = ARR - arr_counter;
		}

		if (lock_delay_counter >= LOCK_DELAY)
		{
			Lock();
			TryToCancelCountLock();
		}

		if (soft_drop_counter >= ARR)
		{
			Drop();
			soft_drop_counter = ARR - soft_drop_counter;
		}

		if (drop_counter >= 1000 / gravity)
		{
			Drop();
			drop_counter = 1000 / gravity - drop_counter;
		}
	}

	inline void Count(int dur)
	{
		if (HasState(CHECK_ARR))
		{
			arr_counter += dur;
		}
		if (HasState(CHECK_LOCK))
		{
			lock_delay_counter += dur ;
		}
		if (HasState(CHECK_AUTO_SHIFT))
		{
			das_counter += dur;
		}
		if (HasState(CHECK_DORP))
		{
			drop_counter += dur;
		}
		if (HasState(CHECK_SOFT_DROP))
		{
			soft_drop_counter += dur;
		}
	}


	inline void StartDASCount()
	{
		state = state | CHECK_AUTO_SHIFT;
	}
	inline void StartARRCount()
	{
		state = state | CHECK_ARR;
	}
	inline void StartLockCount()
	{
		state = state | CHECK_LOCK;
	}
	inline void StartSoftDropCount()
	{
		state = state | CHECK_SOFT_DROP;
	}
	inline void StartDropCount()
	{
		state = state | CHECK_DORP;
	}
public:
	Play_Stage()
	{
		field.ConnectSignal(field.SIGNAL_GAME_OVER, []()->void {
			Debug("game_over");
			});
		field.ConnectSignal(field.SIGNAL_TOUCH_OBSTACLE, [this]()->void {
			StartLockCount();
			});
		field.ConnectSignal(field.SIGNAL_LOCKED, [this]()->void {
			Debug("receive the locked signal");
			});
		StartDropCount();
	}

	void HandleInput(_KEY_TYPE key);

	void Update();

};

class Game
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	Uint64 current_time;
	int fps_counter = 0;
	int fps = 0;
	Play_Stage stage;
	Game() { Init(); };
	static Game* game;
	void HandleInput(const SDL_Event& event);
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
	void Start();
};


class Drawer
{
private:
	Drawer()
	{
		TTF_Init();
		renderer = Game::GetGame().GetRenderer();
		font = TTF_OpenFont("assets/SmileySans-Oblique.ttf", 50);
		if (font == nullptr)
		{
			SDL_Log("can't open the ttp file");
		}
		SDL_SetRenderDrawBlendMode(renderer, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	}
	~Drawer()
	{
		TTF_CloseFont(font);
		TTF_Quit();
	}
	SDL_Renderer* renderer;
	static Drawer* drawer;
	TTF_Font* font;
public:
	static Drawer& GetDrawer()
	{
		return *drawer;
	}
	static void Destory()
	{
		delete drawer;
	}
	void Present()
	{
		SDL_RenderPresent(renderer);
	}
	Drawer(Drawer&) = delete;
	const Drawer& operator=(Drawer&) = delete;
	void SetDrawColor(const RGBA color)
	{
		SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha * 255);
	}

	void SetDrawColor(const HSLA color)
	{
		SetDrawColor(color.ToRGBA());
	}
	void DrawBlock(const Coordinate& coord, RGBA color)
	{
		SDL_Rect rect{ coord.x,coord.y,PLAYER_BRICK_SIZE / 5, PLAYER_BRICK_SIZE / 5 };
		SetDrawColor(color);
		SDL_RenderFillRect(renderer, &rect);
	}
	void DrawPiece(const Piece& p);
	void DrawBack(const Piece& p, int x, int y)
	{
		if (y > 2)
		{
			SDL_Rect rect{ p.relative_coord.x * PLAYER_BRICK_SIZE,p.relative_coord.y * PLAYER_BRICK_SIZE,PLAYER_BRICK_SIZE,PLAYER_BRICK_SIZE };
			SetDrawColor(p.color);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
	void DrawPlayField(const Play_Field& field)
	{
		field.EachBack([this](const Piece& p, int x, int y)->void {
			DrawBack(p, x, y);
			});
		DrawGhost(field);
		DrawFalling(field.GetFalling());
	}
	void DrawGhost(const Play_Field& field);

	void DrawFalling(const Tetrmino& mino);

	void Clear()
	{
		SetDrawColor(RGBA("#999"));
		SDL_RenderClear(renderer);
	}

	void DrawText(const char* text, int x, int y, RGBA color)
	{
		SDL_Surface* surf = TTF_RenderUTF8_Blended(font, text, { color.red, color.green, color.blue, (ui8)(color.alpha * 255) });
		SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
		SDL_Rect rect{ x,y,surf->w,surf->h };
		SDL_RenderCopy(renderer, tex, NULL, &rect);
		SDL_DestroyTexture(tex);
		SDL_FreeSurface(surf);
	}
};
GAME_MODULE_END
