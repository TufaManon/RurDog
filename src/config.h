#pragma once
#define DEBUG_MODE
constexpr auto GAME_WINDOW_WIDTH = 1280;
constexpr auto GAME_WINDOW_HEIGHT = 800;
constexpr auto PLAYER_BRICK_SIZE = 32;
constexpr auto NET_PLAYER_BRICK_SIZE = 4;

#define STAGE_MODULE_BEGIN namespace RurDog {
#define STAGE_MODULE_END }
#define DRAW_MODULE_BEGIN namespace RurDog {
#define DRAW_MODULE_END }
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

constexpr auto WARNNING = "[warn]: %s";
constexpr auto ERROR = "[ERROR]: %s";
constexpr auto DEBUG = "[DEBUG]: %s";

#ifdef DEBUG_MODE

#define DEBUG(x) SDL_Log(ERROR,x)
#define WARNNING(x) SDL_Log(WARNNING,x)
#define ERROR(x) SDL_Log(ERROR,x)

#endif


constexpr auto GAME_NAME= "RurDog";
