#include "Castenstein.h"

Font::Font(string path)
{
	SDL_Surface *bitmap = Engine::LoadSDLSurface(path);
	if (bitmap->w % 16 > 0 || bitmap->h % 16 > 0 || bitmap->w > 4096) throw;

	Width = bitmap->w >> 4;
	Height = bitmap->h >> 4;
	CharacterSpacing = 2;
	Buffer = new unsigned char[bitmap->w * bitmap->h];
	CharacterDimensions = new Point[256];

	unsigned char *bitmapPtr = Buffer;
	for (int i = 0, *buffer = (int*)bitmap->pixels; i < bitmap->w * bitmap->h; i++)
	{
		int r = (*buffer & 0xff0000) >> 16;
		int g = (*buffer & 0xff00) >> 8;
		int b = *buffer & 0xff;
		buffer++;
		*bitmapPtr++ = (r + g + b) / 3;
	}
	SDL_FreeSurface(bitmap);

	for (int i = 0; i < 256; i++)
	{
		CharacterDimensions[i] = Point(0, CharacterSpacing * 3);
		int characterX = (i & 15) * Width, characterY = (i >> 4) * Height;

		bool found = false;
		for (int x = 0; x < Width; x++)
		{
			for (int y = 0; y < Height; y++)
			{
				if (Buffer[(x + characterX) + (y + characterY) * (Width << 4)])
				{
					CharacterDimensions[i].X = x;
					found = true;
					break;
				}
			}
			if (found) break;
		}

		found = false;
		for (int x = Width - 1; x >= 0; x--)
		{
			for (int y = 0; y < Height; y++)
			{
				if (Buffer[(x + characterX) + (y + characterY) * (Width << 4)])
				{
					CharacterDimensions[i].Y = x + 1;
					found = true;
					break;
				}
			}
			if (found) break;
		}
	}
}
Font::~Font()
{
	delete[] Buffer, CharacterDimensions;
}

int Font::getCharacterSpacing()
{
	return CharacterSpacing;
}

void Font::setCharacterSpacing(int characterSpacing)
{
	CharacterSpacing = characterSpacing;
}