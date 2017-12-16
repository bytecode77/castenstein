#include "Castenstein.h"

Texture::Texture()
{
}
Texture::Texture(string path, Color fogColor, float ambientBrightness)
{
	SDL_Surface *surface = Engine::LoadSDLSurface(path);
	if (surface->w != surface->h || (surface->w & (surface->w - 1))) throw;
	Size = surface->w;
	SizeExponent = int(log2(Size));
	Buffer = new int**[2];
	for (int i = 0; i < 2; i++) Buffer[i] = new int*[FogLevels];

	LoadFromSDLSurface(surface, 0, 1, fogColor, ambientBrightness);
	SDL_FreeSurface(surface);
}
Texture::~Texture()
{
	delete[] Buffer;
}

void Texture::LoadFromSDLSurface(SDL_Surface *surface, int frame, int frameCount, Color fogColor, float ambientBrightness)
{
	int avgR = 0, avgG = 0, avgB = 0;

	for (int i = 0; i < FogLevels; i++)
	{
		for (int j = 0; j < 2; j++) Buffer[j][i] = new int[Size * Size];
		for (int y = 0, *buffer2a = Buffer[0][i], *buffer2b = Buffer[1][i]; y < Size; y++)
		{
			for (int x = 0, *buffer = &((int*)surface->pixels)[y + frame * Size]; x < Size; x++)
			{
				if (i == FogLevels - 1)
				{
					avgR += (*buffer & 0xff0000) >> 16;
					avgG += (*buffer & 0xff00) >> 8;
					avgB += *buffer & 0xff;
				}

				if (*buffer == 0xff00ff)
				{
					*buffer2a++ = *buffer2b++ = 0xff00ff;
				}
				else
				{
					int r = (*buffer & 0xff0000) >> 16;
					int g = (*buffer & 0xff00) >> 8;
					int b = *buffer & 0xff;
					int fogR = fogColor.R * (FogLevels - 1 - i) >> FogDetail;
					int fogG = fogColor.G * (FogLevels - 1 - i) >> FogDetail;
					int fogB = fogColor.B * (FogLevels - 1 - i) >> FogDetail;

					int r1 = (r * i >> FogDetail) + fogR;
					int g1 = (g * i >> FogDetail) + fogG;
					int b1 = (b * i >> FogDetail) + fogB;
					*buffer2a++ = r1 << 16 | g1 << 8 | b1;
					int r2 = (int(r * ambientBrightness) * i >> FogDetail) + fogR;
					int g2 = (int(g * ambientBrightness) * i >> FogDetail) + fogG;
					int b2 = (int(b * ambientBrightness) * i >> FogDetail) + fogB;
					*buffer2b++ = r2 << 16 | g2 << 8 | b2;
				}

				buffer += Size * frameCount;
			}
		}
	}

	avgR /= Size * Size;
	avgG /= Size * Size;
	avgB /= Size * Size;
	AverageColor = Color(avgR, avgG, avgB);
}