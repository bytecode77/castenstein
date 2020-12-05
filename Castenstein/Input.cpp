#include "Castenstein.h"

bool Input::Quit;
int Input::MouseX;
int Input::MouseY;
int Input::MouseZ;
int Input::MouseXSpeed;
int Input::MouseYSpeed;
int Input::MouseZSpeed;
bool *Input::MouseDown;
bool *Input::KeyDown;

void Input::Initialize()
{
	MouseDown = new bool[3];
	KeyDown = new bool[2048];
	memset(MouseDown, 0, 3);
	memset(KeyDown, 0, 2048);
}
void Input::Destroy()
{
	delete[] MouseDown;
	delete[] KeyDown;
}

void Input::Update()
{
	MouseXSpeed = 0;
	MouseYSpeed = 0;
	MouseZSpeed = 0;

	SDL_Event e;
	int key;

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
			case SDL_QUIT:
				Quit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (e.button.button)
				{
					case SDL_BUTTON_LEFT: MouseDown[0] = true; break;
					case SDL_BUTTON_RIGHT: MouseDown[1] = true; break;
					case SDL_BUTTON_MIDDLE: MouseDown[2] = true; break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (e.button.button)
				{
					case SDL_BUTTON_LEFT: MouseDown[0] = false; break;
					case SDL_BUTTON_RIGHT: MouseDown[1] = false; break;
					case SDL_BUTTON_MIDDLE: MouseDown[2] = false; break;
				}
				break;
			case SDL_MOUSEMOTION:
				MouseXSpeed += (e.motion.x - MouseX);
				MouseYSpeed += (e.motion.y - MouseY);
				MouseX = e.motion.x;
				MouseY = e.motion.y;
				break;
			case SDL_MOUSEWHEEL:
				MouseZ += e.wheel.y;
				MouseZSpeed = e.wheel.y;
				break;
			case SDL_KEYDOWN:
				key = e.key.keysym.sym;
				if ((key & SDLK_SCANCODE_MASK) == SDLK_SCANCODE_MASK) key += 1024 - SDLK_SCANCODE_MASK;
				if (key < 2048) KeyDown[key] = true;
				break;
			case SDL_KEYUP:
				key = e.key.keysym.sym;
				if ((key & SDLK_SCANCODE_MASK) == SDLK_SCANCODE_MASK) key += 1024 - SDLK_SCANCODE_MASK;
				if (key < 2048) KeyDown[key] = false;
				break;
		}
	}
}

bool Input::getQuit()
{
	return Quit;
}
int Input::getMouseX()
{
	return MouseX;
}
int Input::getMouseY()
{
	return MouseY;
}
int Input::getMouseZ()
{
	return MouseZ;
}
int Input::getMouseXSpeed()
{
	return MouseXSpeed;
}
int Input::getMouseYSpeed()
{
	return MouseYSpeed;
}
int Input::getMouseZSpeed()
{
	return MouseZSpeed;
}
bool Input::getMouseDown(MouseButton mouseButton)
{
	return MouseDown[int(mouseButton)];
}
bool Input::getKeyDown(int key)
{
	if ((key & SDLK_SCANCODE_MASK) == SDLK_SCANCODE_MASK) key += 1024 - SDLK_SCANCODE_MASK;
	return KeyDown[key];
}

void Input::setMousePosition(int x, int y, bool updateMouseSpeed)
{
	if (updateMouseSpeed)
	{
		MouseXSpeed += MouseX - x;
		MouseYSpeed += MouseY - y;
	}
	MouseX = x;
	MouseY = y;
	SDL_WarpMouseInWindow(Engine::Window, x, y);
}