#pragma once
#include "config.h"
#include<string>
#include<vector>
#include<cmath>

GAME_MODULE_BEGIN
using Tricolor = int;

#define SWAP(x,y) auto temp = x; x = y; y = temp;

constexpr auto RGBA_MAX = 255;
constexpr auto ALPHA_MAX = 1.0F;
constexpr auto ALPHA_MIN = 0.0F;

template<typename T>
T Max(std::initializer_list<T> list)
{
	if (list.size() == 0) return 0;
	auto it = list.begin();
	auto max = *it;
	it++;
	for (; it != list.end(); it++)
	{
		if (*it > max)
			max = *it;
	}
	return max;
}

template<typename T>
T Min(const std::initializer_list<T> list)
{
	auto it = list.begin();
	auto min = *it;
	it++;
	for (; it != list.end(); it++)
	{
		if (*it < min)
			min = *it;
	}
	return min;
}
struct RGBA
{
private:
	bool Check(std::string hex);
	int HexStringToInt(char c)
	{
		if (c <= 0x39) return c - 0x30;
		if (c <= 0x46) return c - (0x41 - 10);
		if (c <= 0x66) return c - (0x61 - 10);
		return 0;
	}
public:
	Tricolor red, green, blue;
	float alpha;

	RGBA(int hex, float alpha = ALPHA_MAX)
	{
		red = hex >> 16;
		green = (hex >> 8) - (red << 8);
		blue = hex - (hex << 8);
		this->alpha = alpha;
	}

	RGBA(std::string hex, float alpha = ALPHA_MAX)
	{
		if (!Check(hex)) return;
		if (hex.size() == 4)
		{
			SetRGBA((HexStringToInt(hex[1]) << 4) + HexStringToInt(hex[1]),
				(HexStringToInt(hex[2]) << 4) + HexStringToInt(hex[2]),
				(HexStringToInt(hex[3]) << 4) + HexStringToInt(hex[3]), alpha);
		}
		else
		{
			SetRGBA((HexStringToInt(hex[1]) << 4) + HexStringToInt(hex[2]),
				(HexStringToInt(hex[3]) << 4) + HexStringToInt(hex[4]),
				(HexStringToInt(hex[5]) << 4) + HexStringToInt(hex[6]), alpha);
		}
	}

	void SetRGBA(Tricolor r, Tricolor g, Tricolor b, float a)
	{
		red = r;
		green = g;
		blue = b;
		alpha = a;
	}

	RGBA(Tricolor r = RGBA_MAX,
		Tricolor g = RGBA_MAX,
		Tricolor b = RGBA_MAX,
		float a = ALPHA_MAX)
	{
		if (r > RGBA_MAX || g > RGBA_MAX || b > RGBA_MAX || a > ALPHA_MAX)
		{
			red = RGBA_MAX;
			blue = RGBA_MAX;
			green = RGBA_MAX;
			alpha = ALPHA_MAX;
			return;
		}
		red = r;
		green = g;
		blue = b;
		alpha = a;
	}

	double Nor(Tricolor color) const 
	{
		return ((double)color / RGBA_MAX);
	}


	Tricolor GetHue(double max, double min) const
	{
		if (max == min) return 0;

		int h = 0;

		auto _red = Nor(red);
		auto _green = Nor(green);
		auto _blue = Nor(blue);

		double diff = max - min;

		if (max == _red)
		{
			h = 60 * (_green - _blue) / diff;
			if (h >= 0) return h;
			else return 360 + h;
		}
		else if (max == _green) h = 60 * ((_blue -_red) / diff) + 120;
		else if (max == _blue) h = 60 * ((_red - _green) / diff) + 240;
		return h;
	}

	Tricolor GetHue() const
	{
		auto _red = Nor(red);
		auto _green = Nor(green);
		auto _blue = Nor(blue);
		return GetHue(Max({ _red, _green, _blue }), Min({ _red, _green, _blue }));
	}

	double GetSaturation() const 
	{
		auto _red = Nor(red);
		auto _green = Nor(green);
		auto _blue = Nor(blue);
		return GetSaturation(Max({ _red, _green, _blue }), Min({ _red, _green, _blue }));
	}

	double GetSaturation(double max, double min) const
	{
		auto lightness = GetLightness(max, min);
		if (lightness == 0 || max == min) return 0;
		if (lightness <= 0.5) return (max - min) / (max + min);
		if (lightness > 0.5) return (max - min) / (2 - (max + min));
		return 0;
	}

	double GetLightness() const
	{
		auto _red = Nor(red);
		auto _green = Nor(green);
		auto _blue = Nor(blue);
		return GetLightness(Max({ _red, _green, _blue }), Min({ _red, _green, _blue }));
	}

	double GetLightness(double max, double min) const
	{
		return (max + min) / 2;
	}

};

double Hue2RGB(double v1, double v2, double vH);

RGBA HSLA2RGBA(double H, double S, double L, double A);

struct HSLA
{
	const Tricolor MAX_HUE = 360;
	const Tricolor MIN_HUE = 0;

	const double MAX_SATURATION = 1;
	const double MIN_SATURATOIN = 0;

	const double MAX_LIGHTNESS = 1;
	const double MIN_LIGHTNESS = 0;

	Tricolor hue = 0;
	double saturation = 0;
	double lightness = 0;

	float alpha;

	HSLA(double h, double s, double l, float a)
	{
		hue = h;
		saturation = s;
		lightness = l;
		alpha = a;
	}

	HSLA(RGBA color):HSLA(color.GetHue(), color.GetSaturation(), color.GetLightness(), color.alpha)
	{

	}

	HSLA(const HSLA& hsla)
	{
		hue = hsla.hue;
		saturation = hsla.saturation;
		lightness = hsla.lightness;
		alpha = hsla.alpha;
	}

	HSLA& operator=(const HSLA& hsla)
	{
		hue = hsla.hue;
		saturation = hsla.saturation;
		lightness = hsla.lightness;
		alpha = hsla.alpha;
	}

	RGBA ToRGBA()
	{
		return HSLA2RGBA(hue, saturation, lightness, alpha);
	}
private:
};

// Color Struct of Tetrmino
#define CYAN_COLOR RGBA(CYAN)
#define YELLO_COLOR RGBA(YELLO)
#define PURPLE_COLOR RGBA(PURPLE)
#define GREEN_COLOR RGBA(GREEN)
#define RED_COLOR RGBA(RED)
#define BLUE_COLOR RGBA(BLUE)
#define ORANGE_COLOR RGBA(ORANGE)
#define BACKGROUND_COLOR RGBA(BACKGROUND)
struct Coordinate
{
	int16_t x, y;

	// the default coordinate is (0,0)
	Coordinate(int16_t x = 0, int16_t y = 0)
	{
		this->x = x;
		this->y = y;
	}
	const Coordinate operator+(const Coordinate& right) const
	{
		return Coordinate(x + right.x, y + right.y);
	}
	const Coordinate& operator+=(const Coordinate& right)
	{
		this->x += right.x;
		this->y += right.y;
		return *this;
	}

	const Coordinate& operator-=(const Coordinate& right)
	{
		this->x -= right.x;
		this->y -= right.y;
		return *this;
	}

	Coordinate& operator=(const Coordinate& right)
	{
		this->x = right.x;
		this->y = right.y;
		return *this;
	}

	const Coordinate operator-(const Coordinate& right) const
	{
		return Coordinate(x - right.x, y - right.y);
	}
	bool operator==(const Coordinate& right) const
	{
		return x == right.x && y == right.y;
	}
	const Coordinate& operator<<(int off)
	{
		this->x -= off;
		return *this;
	}
	const Coordinate& operator>>(int off)
	{
		this->x += off;
		return *this;
	}

	const Coordinate& operator^(int off)
	{
		this->y += off;
		return *this;
	}
};

#define Coord(x,y) Coordinate(x,y)
GAME_MODULE_END
