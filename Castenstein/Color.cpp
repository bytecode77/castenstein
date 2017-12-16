#include "Castenstein.h"

Color::Color()
{
	R = 0;
	G = 0;
	B = 0;
}
Color::Color(int r, int g, int b)
{
	R = r;
	G = g;
	B = b;
}
Color::Color(int rgb)
{
	R = (rgb & 0xff0000) >> 16;
	G = (rgb & 0xff00) >> 8;
	B = rgb & 0xff;
}

int Color::getRGB()
{
	return R << 16 | G << 8 | B;
}