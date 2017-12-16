#include "Castenstein.h"

int Engine::Width;
int Engine::Height;
SDL_Window *Engine::Window;
SDL_Renderer *Engine::Renderer;
SDL_Texture *Engine::ScreenTexture;

int Engine::ThreadCount;
RenderThread **Engine::Threads;
float *Engine::FloorDistanceCache;
int *Engine::FloorBrightnessCache;
int *Engine::SkyVCache;

int *Engine::BackBuffer;
float *Engine::DepthBuffer;
Map *Engine::CurrentMap;
Sky *Engine::CurrentSky;
Player *Engine::CurrentPlayer;
list<Sprite*> *Engine::Sprites;

void Engine::Initialize(int width, int height)
{
	Width = width;
	Height = height;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_ShowCursor(0);
	Window = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_SHOWN);
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	ScreenTexture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, Width, Height);

	ThreadCount = thread::hardware_concurrency();
	Threads = new RenderThread*[ThreadCount];
	for (int i = 0; i < ThreadCount; i++)
	{
		Threads[i] = new RenderThread();
		Threads[i]->ThreadNumber = i;
		Threads[i]->Mutex = SDL_CreateMutex();
		Threads[i]->Cond = SDL_CreateCond();
		Threads[i]->Running = false;
		Threads[i]->Quit = false;
		Threads[i]->SDLThread = SDL_CreateThread(RenderThreadFunction, nullptr, Threads[i]);
	}

	FloorDistanceCache = new float[Height];
	FloorBrightnessCache = new int[Height];
	SkyVCache = new int[Height];

	BackBuffer = new int[Width * Height];
	DepthBuffer = new float[Width];
	Sprites = new list<Sprite*>();

	setTitle("Castenstein");
	setPlayer(nullptr);
	setMap(nullptr);
	setSky(Sky::CreateDummySky());
	Input::Initialize();
}
void Engine::Destroy()
{
	Input::Destroy();

	for (int i = 0; i < ThreadCount; i++)
	{
		Threads[i]->Quit = true;
		SDL_LockMutex(Threads[i]->Mutex);
		SDL_CondSignal(Threads[i]->Cond);
		SDL_UnlockMutex(Threads[i]->Mutex);
		SDL_WaitThread(Threads[i]->SDLThread, nullptr);
		SDL_DestroyMutex(Threads[i]->Mutex);
		SDL_DestroyCond(Threads[i]->Cond);
	}
	delete[] Threads;
	delete[] FloorDistanceCache;
	delete[] FloorBrightnessCache;
	delete[] SkyVCache;
	delete[] BackBuffer;
	delete[] DepthBuffer;
	delete Sprites;

	SDL_DestroyTexture(ScreenTexture);
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}
void Engine::Render()
{
	int yOffset = -int(CurrentPlayer->Tilt * Height / 45);
	for (int y = 0; y < Height; y++)
	{
		if (y < (Height >> 1) + yOffset)
		{
			FloorDistanceCache[y] = Height * (1 + CurrentPlayer->Z * 2) / (Height - 2 * (y - yOffset));
		}
		else
		{
			FloorDistanceCache[y] = Height * (1 - CurrentPlayer->Z * 2) / (2 * (y - yOffset) - Height);
		}
		FloorBrightnessCache[y] = ComputeFogLevel(FloorDistanceCache[y], CurrentMap->FogRange);
		SkyVCache[y] = int((y - yOffset + Height) * CurrentSky->Height / 1.5f / Height) << CurrentSky->WidthExponent;
	}

	for (Sprite *sprite : *Sprites)
	{
		float transformedX = (sprite->X - CurrentPlayer->X) * cosd(-CurrentPlayer->Rotation) - (sprite->Y - CurrentPlayer->Y) * sind(-CurrentPlayer->Rotation);
		sprite->Depth = (sprite->X - CurrentPlayer->X) * sind(-CurrentPlayer->Rotation) + (sprite->Y - CurrentPlayer->Y) * cosd(-CurrentPlayer->Rotation);

		if (sprite->Render = sprite->Depth < 0)
		{
			int projectedX = (Width >> 1) - int(transformedX * Height / sprite->Depth);
			int projectedY = (Height >> 1) + yOffset + int(CurrentPlayer->Z * Height / sprite->Depth);

			int spriteSize = int(Height / -sprite->Depth);
			sprite->RenderStartX = projectedX - (spriteSize >> 1);
			sprite->RenderEndX = projectedX + (spriteSize >> 1);
			sprite->RenderStartY = projectedY - (spriteSize >> 1);
			sprite->RenderEndY = projectedY + (spriteSize >> 1);

			if (sprite->Render = sprite->RenderStartX < Width && sprite->RenderEndX > 0)
			{
				if (sprite->SpriteTexture->FrameCount == 1)
				{
					sprite->RenderTexture = sprite->SpriteTexture->Frames[0];
				}
				else
				{
					float angle = atan2(sprite->X - CurrentPlayer->X, sprite->Y - CurrentPlayer->Y) / float(M_PI) / 2 + .5f - sprite->Rotation / 360;
					int frame = int(round(angle * sprite->SpriteTexture->FrameCount));
					if (frame < 0) frame = sprite->SpriteTexture->FrameCount - (-frame % sprite->SpriteTexture->FrameCount);
					sprite->RenderTexture = sprite->SpriteTexture->Frames[frame % sprite->SpriteTexture->FrameCount];
				}

				sprite->RenderU = 0;
				sprite->RenderV = 0;
				sprite->RenderStepUV = sprite->RenderTexture->Size * 1.f / spriteSize;
				if (sprite->RenderStartX < 0)
				{
					sprite->RenderU = -sprite->RenderStartX * sprite->RenderStepUV;
					sprite->RenderStartX = 0;
				}
				if (sprite->RenderStartY < 0)
				{
					sprite->RenderV = -sprite->RenderStartY * sprite->RenderStepUV;
					sprite->RenderStartY = 0;
				}
				sprite->RenderEndX = min(sprite->RenderEndX, Width);
				sprite->RenderEndY = min(sprite->RenderEndY, Height);
				sprite->RenderShadowLevel = CurrentMap->Blocks[int(sprite->Y)][int(sprite->X)]->CeilingTexture == nullptr ? 0 : 1;
				sprite->RenderFogLevel = ComputeFogLevel(-sprite->Depth, CurrentMap->FogRange);
			}
		}
	}

	Sprites->sort([](Sprite *a, Sprite *b) { return a->Depth < b->Depth; });

	for (int i = 0; i < ThreadCount; i++)
	{
		Threads[i]->Running = true;
		SDL_LockMutex(Threads[i]->Mutex);
		SDL_CondSignal(Threads[i]->Cond);
		SDL_UnlockMutex(Threads[i]->Mutex);
	}
	for (int i = 0; i < ThreadCount; i++)
	{
		SDL_LockMutex(Threads[i]->Mutex);
		while (Threads[i]->Running) SDL_CondWait(Threads[i]->Cond, Threads[i]->Mutex);
		SDL_UnlockMutex(Threads[i]->Mutex);
	}
}
void Engine::Present()
{
	SDL_UpdateTexture(ScreenTexture, nullptr, BackBuffer, Width << 2);
	SDL_RenderCopy(Renderer, ScreenTexture, nullptr, nullptr);
	SDL_RenderPresent(Renderer);
}
void Engine::AddSprite(Sprite *sprite)
{
	Sprites->push_back(sprite);
}

string Engine::getTitle()
{
	return string(SDL_GetWindowTitle(Window));
}
int Engine::getWidth()
{
	return Width;
}
int Engine::getHeight()
{
	return Height;
}
Map *Engine::getMap()
{
	return CurrentMap;
}
Sky *Engine::getSky()
{
	return CurrentSky;
}
Player *Engine::getPlayer()
{
	return CurrentPlayer;
}

void Engine::setTitle(string title)
{
	SDL_SetWindowTitle(Window, title.c_str());
}
void Engine::setIcon(string path)
{
	SDL_Surface *icon = LoadSDLSurface(path);
	SDL_SetWindowIcon(Window, icon);
	SDL_FreeSurface(icon);
}
void Engine::setMap(Map *map)
{
	CurrentMap = map;
}
void Engine::setSky(Sky *sky)
{
	CurrentSky = sky;
}
void Engine::setPlayer(Player *player)
{
	CurrentPlayer = player;
}

int Engine::RenderThreadFunction(void *data)
{
	SDL_SetThreadPriority(SDL_THREAD_PRIORITY_LOW);
	RenderThread *thread = (RenderThread*)data;

	while (true)
	{
		SDL_LockMutex(thread->Mutex);
		while (!thread->Running && !thread->Quit) SDL_CondWait(thread->Cond, thread->Mutex);
		SDL_UnlockMutex(thread->Mutex);
		if (thread->Quit) break;

		int yOffset = -int(CurrentPlayer->Tilt * Height / 45);

		for (int x = thread->ThreadNumber; x < Width; x += ThreadCount)
		{
			float planeX = (x - (Width >> 1)) * 1.f / Height;
			float rayVx = planeX * cosd(CurrentPlayer->Rotation) + sind(CurrentPlayer->Rotation);
			float rayVy = planeX * sind(CurrentPlayer->Rotation) - cosd(CurrentPlayer->Rotation);

			float sideX, sideY;
			float deltaX = sqrt(1 + (rayVy * rayVy) / (rayVx * rayVx));
			float deltaY = sqrt(1 + (rayVx * rayVx) / (rayVy * rayVy));
			int stepX, stepY;
			int mapX = int(CurrentPlayer->X), mapY = int(CurrentPlayer->Y);

			if (rayVx < 0)
			{
				stepX = -1;
				sideX = (CurrentPlayer->X - mapX) * deltaX;
			}
			else
			{
				stepX = 1;
				sideX = (mapX + 1 - CurrentPlayer->X) * deltaX;
			}
			if (rayVy < 0)
			{
				stepY = -1;
				sideY = (CurrentPlayer->Y - mapY) * deltaY;
			}
			else
			{
				stepY = 1;
				sideY = (mapY + 1 - CurrentPlayer->Y) * deltaY;
			}

			bool side;
			while (true)
			{
				if (sideX < sideY)
				{
					sideX += deltaX;
					mapX += stepX;
					side = false;
				}
				else
				{
					sideY += deltaY;
					mapY += stepY;
					side = true;
				}

				if (mapX < 0 || mapY < 0 || mapX >= CurrentMap->Size.X || mapY >= CurrentMap->Size.Y)
				{
					break;
				}
				else if (CurrentMap->Blocks[mapY][mapX]->IsWall)
				{
					Block *block = CurrentMap->Blocks[mapY][mapX];

					float wallDistance = DepthBuffer[x] = side ? (mapY - CurrentPlayer->Y + ((1 - stepY) >> 1)) / rayVy : (mapX - CurrentPlayer->X + ((1 - stepX) >> 1)) / rayVx;
					int shadowLevel = 0;
					int brightnessLevel = ComputeFogLevel(wallDistance, CurrentMap->FogRange);

					float wallX, wallU;
					if (side)
					{
						wallX = CurrentPlayer->X + wallDistance * rayVx;
						wallU = stepY < 0 ? wallX : -wallX;
						if (CurrentMap->Blocks[mapY - stepY][mapX]->CeilingTexture != nullptr) shadowLevel = 1;
					}
					else
					{
						wallX = CurrentPlayer->Y + wallDistance * rayVy;
						wallU = stepX > 0 ? wallX : -wallX;
						shadowLevel = 1;
					}
					wallX -= floor(wallX);

					float intersectionX, intersectionY;
					if (side)
					{
						intersectionX = mapX + wallX;
						intersectionY = stepY > 0 ? float(mapY) : mapY + 1.f;
					}
					else
					{
						intersectionX = stepX > 0 ? float(mapX) : mapX + 1.f;
						intersectionY = mapY + wallX;
					}

					int actualWallHeight = int(Height / wallDistance) + 2;
					int actualWallDrawStart = (Height >> 1) - (actualWallHeight >> 1) + yOffset - int(CurrentPlayer->Z * Height / wallDistance);
					int wallHeight = actualWallHeight + min(actualWallDrawStart, 0);
					int wallDrawStart = clip(actualWallDrawStart, 0, Height);
					int wallDrawEnd = clip(wallDrawStart + wallHeight, 0, Height);

					int *backBuffer = &BackBuffer[x];
					Texture *texture = nullptr;
					int oldCeilingX = -1, oldCeilingY = -1;
					int skyU = int((x * .25f / Width + CurrentPlayer->Rotation / 360) * CurrentSky->Width) & CurrentSky->Width - 1;

					for (int y = 0; y < wallDrawStart; y++)
					{
						float weight = FloorDistanceCache[y] / wallDistance;
						float ceilingX = weight * intersectionX + (1 - weight) * CurrentPlayer->X;
						float ceilingY = weight * intersectionY + (1 - weight) * CurrentPlayer->Y;

						if (oldCeilingX != int(ceilingX) || oldCeilingY != int(ceilingY))
						{
							oldCeilingX = int(ceilingX);
							oldCeilingY = int(ceilingY);
							texture = CurrentMap->Blocks[clip(oldCeilingY, 0, CurrentMap->Size.Y - 1)][clip(oldCeilingX, 0, CurrentMap->Size.X - 1)]->CeilingTexture;
						}
						if (texture != nullptr)
						{
							int u = int(ceilingX * texture->Size) & (texture->Size - 1);
							int v = int(ceilingY * texture->Size) & (texture->Size - 1);
							*backBuffer = texture->Buffer[1][FloorBrightnessCache[y]][(u << texture->SizeExponent) + v];
						}
						else
						{
							*backBuffer = CurrentSky->Buffer[skyU + SkyVCache[y]];
						}
						backBuffer += Width;
					}

					int uOffset = (int(wallU * block->BlockTexture->Size) & (block->BlockTexture->Size - 1)) << block->BlockTexture->SizeExponent;
					float startV = (wallDrawStart - actualWallDrawStart) * block->BlockTexture->Size * 1.f / actualWallHeight;
					float stepV = block->BlockTexture->Size * 1.f / actualWallHeight;

					for (int y = wallDrawStart; y < wallDrawEnd; y++)
					{
						*backBuffer = block->BlockTexture->Buffer[shadowLevel][brightnessLevel][uOffset + int(startV)];
						backBuffer += Width;
						startV += stepV;
					}

					Block *floorBlock = nullptr;
					int oldFloorX = -1, oldFloorY = -1;

					for (int y = wallDrawEnd; y < Height; y++)
					{
						float weight = FloorDistanceCache[y] / wallDistance;
						float floorX = weight * intersectionX + (1 - weight) * CurrentPlayer->X;
						float floorY = weight * intersectionY + (1 - weight) * CurrentPlayer->Y;

						if (oldFloorX != int(floorX) || oldFloorY != int(floorY))
						{
							oldFloorX = int(floorX);
							oldFloorY = int(floorY);
							floorBlock = CurrentMap->Blocks[clip(oldFloorY, 0, CurrentMap->Size.Y - 1)][clip(oldFloorX, 0, CurrentMap->Size.X - 1)];
						}
						if (floorBlock != nullptr && floorBlock->BlockTexture)
						{
							int u = int(floorX * floorBlock->BlockTexture->Size) & (floorBlock->BlockTexture->Size - 1);
							int v = int(floorY * floorBlock->BlockTexture->Size) & (floorBlock->BlockTexture->Size - 1);
							*backBuffer = floorBlock->BlockTexture->Buffer[floorBlock->CeilingTexture == nullptr ? 0 : 1][FloorBrightnessCache[y]][(u << floorBlock->BlockTexture->SizeExponent) + v];
						}
						backBuffer += Width;
					}

					break;
				}
			}
		}

		for (Sprite *sprite : *Sprites)
		{
			if (sprite->Render)
			{
				int threadNumber = (ThreadCount * sprite->RenderStartX + thread->ThreadNumber - sprite->RenderStartX) % ThreadCount;
				float startU = sprite->RenderU + sprite->RenderStepUV * threadNumber;
				for (int x = sprite->RenderStartX + threadNumber; x < sprite->RenderEndX; x += ThreadCount)
				{
					int uOffset = int(startU) << sprite->RenderTexture->SizeExponent;
					if (-sprite->Depth < DepthBuffer[x])
					{
						int *backBuffer = &BackBuffer[x + sprite->RenderStartY * Width];
						float startV = sprite->RenderV;
						for (int y = sprite->RenderStartY; y < sprite->RenderEndY; y++)
						{
							int color = sprite->RenderTexture->Buffer[sprite->RenderShadowLevel][sprite->RenderFogLevel][uOffset + int(startV)];
							if (color != 0xff00ff) *backBuffer = color;
							backBuffer += Width;
							startV += sprite->RenderStepUV;
						}
					}
					startU += sprite->RenderStepUV * ThreadCount;
				}
			}
		}

		thread->Running = false;
		SDL_LockMutex(thread->Mutex);
		SDL_CondSignal(thread->Cond);
		SDL_UnlockMutex(thread->Mutex);
	}
	return 0;
}
SDL_Surface* Engine::LoadSDLSurface(string path)
{
	SDL_Surface *bitmap = nullptr;
	string ext = path.substr(path.find_last_of(".") + 1);

	if (ext == "bmp") bitmap = SDL_LoadBMP(path.c_str());
	else if (ext == "jpg" || ext == "jpeg") bitmap = IMG_LoadJPG_RW(SDL_RWFromFile(&path[0], "rb"));
	else if (ext == "png") bitmap = IMG_LoadPNG_RW(SDL_RWFromFile(&path[0], "rb"));
	else if (ext == "gif") bitmap = IMG_LoadGIF_RW(SDL_RWFromFile(&path[0], "rb"));
	else if (ext == "tga") bitmap = IMG_LoadTGA_RW(SDL_RWFromFile(&path[0], "rb"));
	else if (ext == "tif" || ext == "tiff") bitmap = IMG_LoadTIF_RW(SDL_RWFromFile(&path[0], "rb"));

	if (bitmap == nullptr) throw;
	SDL_Surface *image = SDL_CreateRGBSurface(SDL_SWSURFACE, bitmap->w, bitmap->h, 32, 0xff0000, 0x00ff00, 0x0000ff, 0);
	SDL_BlitSurface(bitmap, nullptr, image, nullptr);
	SDL_FreeSurface(bitmap);
	return image;
}