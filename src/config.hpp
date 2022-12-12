#pragma once
#include <time.h>
#include <string>
#include <SDL.h>
#define GAME_NAME "DurDog"
struct Ele_Color
{
	std::string CYAN, YELLO, PURPLE, GREEN, RED, BLUE, ORANGE, GREY;
};


struct Window_Config
{
	int width;
	int height;
};


struct Delay_Config
{
	Uint64 lock_delay;
	Uint64 clear_delay;
	Uint64 auto_shift_delay;
	Uint64 auto_repeate_rate;
};

struct Interface_Cofig
{
	int tile_size;
	int net_player_tile_size;
	
};

class Global_Config
{
	Global_Config()
	{
		window_config.height = 800;
		window_config.width = 1280;

		delay_config.auto_repeate_rate = 33;
		delay_config.auto_shift_delay = 88;
		delay_config.clear_delay = 100;
		delay_config.lock_delay = 500;

		interface_config.tile_size = 32;
		interface_config.net_player_tile_size = 4;

		colors.BLUE = "#44cef6";
		colors.CYAN = "#7bcfa6";
		colors.GREEN = "#0eb83a";
		colors.GREY = "#d6ecf0";
		colors.ORANGE = "#ffa400";
		colors.PURPLE = "#801dae";
		colors.RED = "#dc3023";
		colors.YELLO = "#fff143";
	}
public:
	const static Global_Config config;
	Window_Config window_config;
	Delay_Config delay_config;
	Interface_Cofig interface_config;
	Ele_Color colors;
	Global_Config& operator=(const Global_Config& right) = delete;
	Global_Config(const Global_Config& right) = delete;
};



