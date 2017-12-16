#include "Castenstein.h"

void Graphics::FillRectangle(int x, int y, int width, int height, Color color)
{
	x = clip(x, 0, Engine::Width);
	y = clip(y, 0, Engine::Height);
	width = clip(x + width, 0, Engine::Width);
	height = clip(y + height, 0, Engine::Height);
	int rgb = color.getRGB();

	for (int y2 = y; y2 < height; y2++)
	{
		for (int x2 = x, *backBuffer = &Engine::BackBuffer[y2 * Engine::Width + x2]; x2 < width; x2++)
		{
			*backBuffer++ = rgb;
		}
	}
}
void Graphics::FillRectangle(int x, int y, int width, int height, Color color, float opacity)
{
	x = clip(x, 0, Engine::Width);
	y = clip(y, 0, Engine::Height);
	width = clip(x + width, 0, Engine::Width);
	height = clip(y + height, 0, Engine::Height);
	opacity = clip(opacity, 0, 1);
	int opacityA = int(opacity * 256);
	int opacityB = 256 - opacityA;

	for (int y2 = y; y2 < height; y2++)
	{
		for (int x2 = x, *backBuffer = &Engine::BackBuffer[y2 * Engine::Width + x2]; x2 < width; x2++)
		{
			int r = (*backBuffer & 0xff0000) >> 16;
			int g = (*backBuffer & 0xff00) >> 8;
			int b = *backBuffer & 0xff;
			*backBuffer++ = (r * opacityB + color.R * opacityA) >> 8 << 16 | (g * opacityB + color.B * opacityA) >> 8 << 8 | (b * opacityB + color.B * opacityA) >> 8;
		}
	}
}
void Graphics::DrawString(int x, int y, string text, Font *font)
{
	DrawString(x, y, text, font, Color(255, 255, 255));
}
void Graphics::DrawString(int x, int y, string text, Font *font, Color color)
{
	int textLength = int(text.length());
	unsigned char *bitmapPtr = font->Buffer;

	for (int i = 0, x2 = x; i < textLength; i++)
	{
		char character = text[i];
		int characterX = (text[i] & 15) * font->Width, characterY = (text[i] >> 4) * font->Height;
		Point characterDimension = font->CharacterDimensions[character];
		int characterWidth = characterDimension.Y - characterDimension.X;

		for (int py = 0; py < font->Height; py++)
		{
			for (int px = 0, *backBufferPtr = &Engine::BackBuffer[x2 + (y + py) * Engine::Width]; px < characterWidth; px++)
			{
				int r = (*backBufferPtr & 0xff0000) >> 16;
				int g = (*backBufferPtr & 0xff00) >> 8;
				int b = *backBufferPtr & 0xff;
				int opacityA = bitmapPtr[(px + characterDimension.X + characterX) + (py + characterY) * (font->Width << 4)];
				int opacityB = 255 - opacityA;
				*backBufferPtr++ = (r * opacityB + color.R * opacityA) >> 8 << 16 | int(g * opacityB + color.G * opacityA) >> 8 << 8 | (b * opacityB + color.B * opacityA) >> 8;
			}
		}
		x2 += characterWidth + font->CharacterSpacing;
	}
}
void Graphics::DrawMinimap(int x, int y, int size, float relativeBlockSize, float opacity, float shadedAreaBrightness)
{
	opacity = clip(opacity, 0, 1);

	int radius = (size >> 1);
	int radius2 = radius * radius;
	float blockSize = size * relativeBlockSize;
	float sinRotation = sind(Engine::CurrentPlayer->Rotation);
	float cosRotation = cosd(Engine::CurrentPlayer->Rotation);

	for (int y2 = 0, *backBuffer = &Engine::BackBuffer[x + y * Engine::Width]; y2 < size; y2++)
	{
		for (int x2 = 0; x2 < size; x2++)
		{
			if ((x2 - radius) * (x2 - radius) + (y2 - radius) * (y2 - radius) < radius2)
			{
				float ox = (x2 - radius) / blockSize;
				float oy = (y2 - radius) / blockSize;
				float mapX = ox * cosRotation - oy * sinRotation + Engine::CurrentPlayer->X;
				float mapY = ox * sinRotation + oy * cosRotation + Engine::CurrentPlayer->Y;

				Color color;
				float actualOpacity = opacity;
				if (mapX >= 0 && mapX < Engine::CurrentMap->Size.X && mapY >= 0 && mapY < Engine::CurrentMap->Size.Y && Engine::CurrentMap->Blocks[int(mapY)][int(mapX)]->BlockTexture != nullptr)
				{
					Texture *texture = Engine::CurrentMap->Blocks[int(mapY)][int(mapX)]->BlockTexture;
					float brightness = abs(x2 - radius) < radius - y2 ? 1 : shadedAreaBrightness;
					if (!Engine::CurrentMap->Blocks[int(mapY)][int(mapX)]->IsWall && Engine::CurrentMap->Blocks[int(mapY)][int(mapX)]->CeilingTexture != nullptr) brightness *= Engine::CurrentMap->AmbientBrightness;
					color = texture->AverageColor;
					color.R = int(color.R * brightness);
					color.G = int(color.G * brightness);
					color.B = int(color.B * brightness);
				}
				else
				{
					color = Color(0, 0, 0);
					actualOpacity /= 4;
				}

				int screenR = (*backBuffer & 0xff0000) >> 16;
				int screenG = (*backBuffer & 0xff00) >> 8;
				int screenB = *backBuffer & 0xff;
				int pixelR = int(color.R * actualOpacity + screenR * (1 - actualOpacity));
				int pixelG = int(color.G * actualOpacity + screenG * (1 - actualOpacity));
				int pixelB = int(color.B * actualOpacity + screenB * (1 - actualOpacity));
				*backBuffer = pixelR << 16 | pixelG << 8 | pixelB;
			}
			backBuffer++;
		}
		backBuffer += Engine::Width - size;
	}
}