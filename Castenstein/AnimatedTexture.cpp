#include "Castenstein.h"

AnimatedTexture::AnimatedTexture(string path, Color fogColor, float ambientBrightness)
{
	SDL_Surface *surface = Engine::LoadSDLSurface(path);
	if (surface->w / surface->h != surface->w * 1.f / surface->h || (surface->w & (surface->w - 1))) throw;
	FrameCount = surface->w / surface->h;
	Frames = new Texture*[FrameCount];

	for (int i = 0; i < FrameCount; i++)
	{
		Frames[i] = new Texture();
		Frames[i]->Size = surface->h;
		Frames[i]->SizeExponent = int(log2(Frames[i]->Size));
		Frames[i]->Buffer = new int**[2];
		for (int j = 0; j < 2; j++) Frames[i]->Buffer[j] = new int*[FogLevels];
		Frames[i]->LoadFromSDLSurface(surface, i, FrameCount, fogColor, ambientBrightness);
	}

	SDL_FreeSurface(surface);
}
AnimatedTexture::AnimatedTexture(Texture *texture)
{
	FrameCount = 1;
	Frames = new Texture*[1];
	Frames[0] = texture;
}
AnimatedTexture::~AnimatedTexture()
{
	delete[] Frames;
}