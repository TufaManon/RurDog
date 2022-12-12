#pragma once
#include<string>
#include"utils.hpp"
GAME_MODULE_BEGIN
using ui8 = uint8_t;
using ui16 = uint16_t;
constexpr auto ALPHA_MAX = 1.0F;
constexpr auto ALPHA_MIN = 0.0F;
/// <summary>
/// the Degree struct is the packaged degree value between 0 and 360.
/// </summary>
struct Degree
{
	ui16 value;
	constexpr Degree(ui16 v) :value(v % 361) {}
	constexpr const Degree operator-(Degree& right) const
	{
		if (right.value < value)
			return Degree(value - right.value);

	}
	constexpr const Degree operator+(Degree& right) const
	{
		return Degree(value + right.value);
	}
	constexpr Degree operator/(Degree& right) const
	{

		return Degree(value / right.value);
	}
	constexpr Degree operator*(Degree& right) const
	{
		return Degree(value * right.value);
	}
	constexpr operator ui16() const
	{
		return value;
	}
	constexpr operator double() const
	{
		return (double)value;
	}
};
/// <summary>
/// a color storaged as rgb model, the red,green and blue ranges 0 to 255 and the alpha ranges 0 to 1;
/// </summary>
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
	ui8 red, green, blue;
	float alpha;

	RGBA(const RGBA& right)
	{
		*this = right;
	}

	RGBA(int hex, float alpha = 1.0)
	{
		red = hex >> 16;
		green = (hex >> 8) - (red << 8);
		blue = hex - (hex << 8);
		this->alpha = alpha;
	}

	RGBA(std::string hex, float alpha = 1.0)
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

	void SetRGBA(ui8 r, ui8 g, ui8 b, float a)
	{
		red = r;
		green = g;
		blue = b;
		alpha = a;
	}

	RGBA(ui8 r = 255,
		ui8 g = 255,
		ui8 b = 255,
		float a = 1.0)
	{
		if (r > 255 || g > 255 || b > 255 || a > 1.0)
		{
			red = 255;
			blue = 255;
			green = 255;
			alpha = 1.0;
			return;
		}
		red = r;
		green = g;
		blue = b;
		alpha = a;
	}

	constexpr double Nor(ui8 color) const
	{
		return ((double)color / 255);
	}


	Degree GetHue(double max, double min) const
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
		else if (max == _green) h = 60 * ((_blue - _red) / diff) + 120;
		else if (max == _blue) h = 60 * ((_red - _green) / diff) + 240;
		return h;
	}

	Degree GetHue() const
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



struct HSLA
{
	const double MAX_SATURATION = 1;
	const double MIN_SATURATOIN = 0;

	const double MAX_LIGHTNESS = 1;
	const double MIN_LIGHTNESS = 0;

	Degree hue = 0;
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

	HSLA(const RGBA& color) :HSLA(color.GetHue(), color.GetSaturation(), color.GetLightness(), color.alpha)
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

	RGBA ToRGBA() const
	{
		return HSLA2RGBA(hue, saturation, lightness, alpha);
	}

private:
	double Hue2RGB(double v1, double v2, double vH) const;

	RGBA HSLA2RGBA(double H, double S, double L, double A) const;
};
GAME_MODULE_END