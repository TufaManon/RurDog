#include "Game.h"
#include "config.h"

GAME_MODULE_BEGIN
Game* Game::game = new Game();
Drawer* Drawer::drawer = new Drawer();
void Game::Init()

{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_Log(SDL_GetError());
		exit(1);
	}
	window = SDL_CreateWindow(GAME_NAME,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		SDL_Log(SDL_GetError());
		exit(1);
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == nullptr)
	{
		SDL_Log(SDL_GetError());
		exit(1);
	}
}

void Game::Close()
{
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
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				quit = true;
			HandleInput(event);
		}
		Drawer::GetDrawer().Clear();
		fps_counter++;
		stage.Update();
		if (SDL_GetTicks64() - current_time > 1000)
		{
			fps = fps_counter;
			fps_counter = 0;
			current_time = SDL_GetTicks64();
		}
		std::string fps_str = std::to_string(fps);
		Drawer::GetDrawer().DrawText(fps_str.c_str(), 500, 500, RGBA("#233"));
		Drawer::GetDrawer().Present();
	}
}

void Game::HandleInput(const SDL_Event& event)
{
	if (event.type == SDL_EventType::SDL_KEYDOWN)
	{
		if (event.key.keysym.scancode == SDL_SCANCODE_A)
		{
			stage.HandleInput(_KEY_TYPE::LEFT_DOWN);
		}
		else if (event.key.keysym.scancode == SDL_SCANCODE_D)
		{
			stage.HandleInput(_KEY_TYPE::RIGHT_DOWN);
		}
		else if (event.key.keysym.scancode == SDL_SCANCODE_S)
		{
			stage.HandleInput(_KEY_TYPE::DOWN_DOWN);
		}
		else if (event.key.keysym.scancode == SDL_SCANCODE_W)
		{
			stage.HandleInput(_KEY_TYPE::DROP_DOWN);
		}
		else if (event.key.keysym.scancode == SDL_SCANCODE_J)
		{
			stage.HandleInput(_KEY_TYPE::R_DOWN);
		}
		else if (event.key.keysym.scancode == SDL_SCANCODE_K)
		{
			stage.HandleInput(_KEY_TYPE::L_DOWN);
		}
		else if (event.key.keysym.scancode == SDL_SCANCODE_L)
		{
			stage.HandleInput(_KEY_TYPE::HOLD_DOWN);
		}
	}

	if (event.type == SDL_EventType::SDL_KEYUP)
	{
		if (event.key.keysym.scancode == SDL_SCANCODE_A)
		{
			stage.HandleInput(_KEY_TYPE::LEFT_UP);
		}
		else if (event.key.keysym.scancode == SDL_SCANCODE_D)
		{
			stage.HandleInput(_KEY_TYPE::RIGHT_UP);
		}
		else if (event.key.keysym.scancode == SDL_SCANCODE_S)
		{
			stage.HandleInput(_KEY_TYPE::DWON_UP);
		}
		else if (event.key.keysym.scancode == SDL_SCANCODE_J)
		{
			stage.HandleInput(_KEY_TYPE::R_UP);
		}
		else if (event.key.keysym.scancode == SDL_SCANCODE_K)
		{
			stage.HandleInput(_KEY_TYPE::L_UP);
		}
		else if (event.key.keysym.scancode == SDL_SCANCODE_L)
		{
			stage.HandleInput(_KEY_TYPE::HOLD_UP);
		}
		else if (event.key.keysym.scancode == SDL_SCANCODE_W)
		{
			stage.HandleInput(_KEY_TYPE::DROP_UP);
		}
	}
}

void Drawer::DrawPiece(const Piece& p)
{
	int block_with = PLAYER_BRICK_SIZE / 5;
	HSLA hsla(p.color);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
			DrawBlock(Coord(p.relative_coord.x * PLAYER_BRICK_SIZE + block_with * i, p.relative_coord.y * PLAYER_BRICK_SIZE + block_with * j), hsla.ToRGBA());
		hsla.lightness *= 0.95;
		hsla.saturation *= 0.9;
	}
}

void Drawer::DrawFalling(const Tetrmino& mino)
{
	Tetrmino falling_copy = mino;
	for (int i = 0; i < 4; i++)
	{
		falling_copy.pieces[i].relative_coord += falling_copy.bounding_box;
		DrawPiece(falling_copy.pieces[i]);
	}
}

void Drawer::DrawGhost(const Play_Field& field)
{
	Tetrmino falling_copy = field.GetFalling();
	while (field.TestingDrop(falling_copy));
	for (int i = 0; i < 4; i++)
	{
		falling_copy.pieces[i].relative_coord += falling_copy.bounding_box;
		falling_copy.pieces[i].color.alpha = 0.2;
		DrawPiece(falling_copy.pieces[i]);
	}
}


inline void Play_Stage::HandleInput(_KEY_TYPE key)
{
	switch (key)
	{
	case RurDog::_KEY_TYPE::DOWN_DOWN:
		if (!Pressed(PRESSED_DOWN))
		{
			//Debug("PRESSED DOWN");
			StartSoftDropCount();
			SetPressed(PRESSED_DOWN);
		}

		break;
	case RurDog::_KEY_TYPE::DWON_UP:
		if (Pressed(PRESSED_DOWN))
		{
			//Debug("RELEASE DOWN");
			Release(PRESSED_DOWN);
			TryToCancelCountSoftDrop();
		}
		break;
	case RurDog::_KEY_TYPE::LEFT_DOWN:
		if (!Pressed(PRESSED_LEFT)  && !Pressed(PRESSED_RIGHT))
		{
			//Debug("PRESSED LEFT");
			Move(_Ori::L);
			SetPressed(PRESSED_LEFT);
			StartDASCount();
		}
		break;
	case RurDog::_KEY_TYPE::LEFT_UP:
		if (Pressed(PRESSED_LEFT))
		{
			//Debug("RELEASE LEFT");
			Release(PRESSED_LEFT);
			TryToCancelCountDAS();
			TryToCancelCountARR();
		}
		break;
	case RurDog::_KEY_TYPE::RIGHT_DOWN:
		if (!Pressed(PRESSED_RIGHT) && !Pressed(PRESSED_LEFT) )
		{
			//Debug("PRESSED RIGHT");
			Move(_Ori::R);
			SetPressed(PRESSED_RIGHT);
			StartDASCount();
		}
		break;
	case RurDog::_KEY_TYPE::RIGHT_UP:
		if (Pressed(PRESSED_RIGHT))
		{
			//Debug("RELEASE RIGHT");
			Release(PRESSED_RIGHT);
			TryToCancelCountDAS();
			TryToCancelCountARR();
		}
		break;
	case RurDog::_KEY_TYPE::HOLD_DOWN:
		if (!Pressed(PRESSED_HOLD))
		{
			//Debug("PRESSED HOLD");
			SetPressed(PRESSED_HOLD);
			Hold();
		}
		break;
	case RurDog::_KEY_TYPE::R_DOWN:
		if (!Pressed(PRESSED_R))
		{
			//Debug("PRESSED R");
			SetPressed(PRESSED_R);
			Rotating(_Ori::R);
		}
		break;
	case RurDog::_KEY_TYPE::L_DOWN:
		if (!Pressed(PRESSED_L))
		{
			//Debug("PRESSED L");
			SetPressed(PRESSED_L);
			Rotating(_Ori::L);
		}
		break;
	case RurDog::_KEY_TYPE::R_UP:
		if (Pressed(PRESSED_R))
		{
			//Debug("RELEASE R");
			Release(PRESSED_R);
		}
		break;
	case RurDog::_KEY_TYPE::L_UP:
		if (Pressed(PRESSED_L))
		{
			//Debug("RELEASE L");
			Release(PRESSED_L);
		}
		break;
	case RurDog::_KEY_TYPE::HOLD_UP:
		if (Pressed(PRESSED_HOLD))
		{
			//Debug("RELEASE HOLD");
			Release(PRESSED_HOLD);
		}
		break;
	case RurDog::_KEY_TYPE::DROP_DOWN:
		if (!Pressed(PRESSED_DRAP))
		{
			//Debug("PRESSED HARD DROP");
			SetPressed(PRESSED_DRAP);
			HardDrop();
		}
		break;
	case _KEY_TYPE::DROP_UP:
		if (Pressed(PRESSED_DRAP))
		{
			//Debug("RELEASE HARD DROP");
			Release(PRESSED_DRAP);
		}
		break;
	default:
		break;
	}
}

void Play_Stage::Update()
{
	Check();
	auto time = SDL_GetTicks64();
	Count(time - current_tick64);
	current_tick64 = time;
	Drawer::GetDrawer().DrawPlayField(field);
}

void Game::Destory()
{
	Drawer::Destory();
	game->Close();
	delete game;
}
GAME_MODULE_END

