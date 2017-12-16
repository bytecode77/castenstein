#include "Castenstein.h"

Sky::Sky()
{
}
Sky::Sky(string path)
{
	SDL_Surface *surface = Engine::LoadSDLSurface(path);
	if ((surface->w & (surface->w - 1)) || (surface->h & (surface->h - 1))) throw;
	Width = surface->w;
	Height = surface->h;
	WidthExponent = int(log2(Width));
	Buffer = new int[Width * Height];
	memcpy(Buffer, surface->pixels, Width * Height * 4);
	SDL_FreeSurface(surface);
}
Sky::~Sky()
{
	delete[] Buffer;
}

Sky* Sky::CreateDummySky()
{
	Sky *sky = new Sky();
	sky->Width = 1;
	sky->Height = 1;
	sky->WidthExponent = 0;
	sky->Buffer = new int[0];
	sky->Buffer[0] = 0xffffff;
	return sky;
}