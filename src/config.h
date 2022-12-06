#pragma once
#define DEBUG_MODE
#include <time.h>
#include <string>
#include <SDL.h>

constexpr auto GAME_WINDOW_WIDTH = 1280;
constexpr auto GAME_WINDOW_HEIGHT = 800;
constexpr auto PLAYER_BRICK_SIZE = 32;
constexpr auto NET_PLAYER_BRICK_SIZE = 4;
//锁定延迟
constexpr auto LOCK_DELAY = 500;
//自动移动延迟
constexpr auto DAS = 99;
//自动重复速率
constexpr auto ARR = 33;

#define GAME_MODULE_BEGIN namespace RurDog {
#define GAME_MODULE_END }

// Color Hex Date of Tetrmino
constexpr auto CYAN = "#00e09e";
constexpr auto YELLO = "#fff143";
constexpr auto PURPLE = "#8d4bbb";
constexpr auto GREEN = "#40de5a";
constexpr auto RED = "#ff461f";
constexpr auto BLUE = "#44cef6";
constexpr auto ORANGE = "#ea5506";
constexpr auto BACKGROUND = "#ddd";
#define DEBUG_MARK "[DEBUG]: "
#ifdef DEBUG_MODE
#define TIME_STAMP "\t --%s "
inline void Debug(const char* s)
{
	std::string str(DEBUG_MARK);
	str.append(s);
	str.append(TIME_STAMP);
	auto t = time(NULL);
	SDL_Log(str.c_str(), asctime(localtime(&t)));
}
#else 
inline void Debud(const char* s)
{
	return;
}
#endif


constexpr auto GAME_NAME= "RurDog";
