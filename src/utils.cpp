#include "utils.h"
GAME_MODULE_BEGIN
bool RGBA::Check(std::string hex)
{
	if (hex.size() == 0) return false;
	bool right_head = hex[0] == '#';
	bool right_range = true;
	bool right_size = hex.size() == 7 || hex.size() == 4;
	for (auto c : hex.substr(1))
	{
		right_range &= c >= 0x41 && c < 0x46 || c >= 0x61 && c <= 0x66 || c >= 0x30 && c <= 0x39;
	}
	return right_head && right_range && right_size;
}
RGBA HSLA2RGBA(double h, double s, double l, double a)
{
	if (s == 0) return RGBA(255 * l, 255 * l, 255 * l, a);
	double q, p, hk = h / 360;
	if (l >= 0.5) q = l + s - (l * s);
	else q = l * (1 + s);
	p = 2 * l - q;
	
	return RGBA(255 * Hue2RGB(p, q, hk + (1.0 / 3.0)), 255 * Hue2RGB(p, q, hk), 255 * Hue2RGB(p, q, hk - (1.0 / 3.0)), a);
}

double Hue2RGB(double v1, double v2, double vH)
{
	if (vH < 0) vH += 1.0;
	if (vH > 1) vH -= 1.0;
	if (6.0 * vH < 1) return v1 + (v2 - v1) * 6.0 * vH;
	if (2.0 * vH < 1) return v2;
	if (3.0 * vH < 2) return v1 + (v2 - v1) * ((2.0 / 3.0) - vH) * 6.0;
	return v1;
}
GAME_MODULE_END
