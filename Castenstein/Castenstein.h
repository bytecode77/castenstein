#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(linker, "/subsystem:windows")
#define _SCL_SECURE_NO_WARNINGS

#include "SDL\SDL.h"
#include "SDL\SDL_image.h"

#include <Windows.h>
#include <string>
#include <cmath>
#include <ctime>
#include <thread>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

#define clip(value, start, end) min(max(value, start), end)
#define sind(value) sin((value) * float(M_PI / 180))
#define cosd(value) cos((value) * float(M_PI / 180))
#define tand(value) tan((value) * float(M_PI / 180))

const int FogDetail = 7, FogLevels = 1 << FogDetail;
#define ComputeFogLevel(distance, fogRange) (FogLevels - (fogRange == 0 ? 1 : clip(int((distance) * FogLevels / (fogRange)), 1, FogLevels)))

#include "MouseButton.h"
#include "Point.h"
#include "Color.h"
#include "Font.h"
#include "Graphics.h"
#include "Input.h"
#include "Texture.h"
#include "AnimatedTexture.h"
#include "Sky.h"
#include "Block.h"
#include "Map.h"
#include "Player.h"
#include "Sprite.h"
#include "RenderThread.h"
#include "Engine.h"
#include "FPSCounter.h"