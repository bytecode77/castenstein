#include "Castenstein.h"
#include <sstream>

string IntToString(int value)
{
	ostringstream ss;
	ss << value;
	return ss.str();
}
string FloatToString(float value, int digits)
{
	ostringstream ss;
	ss << round(value * pow(10, digits)) / pow(10, digits);
	return ss.str();
}

int main(int argc, char *argv[])
{
	const int screenWidth = 1280;
	const int screenHeight = 720;
	const int minimapSize = screenWidth / 10;
	Engine::Initialize(screenWidth, screenHeight);
	Engine::setIcon("Media\\Images\\Icon.png");

	FPSCounter *fpsCounter = new FPSCounter(500);
	Font *font = new Font("Media\\Images\\Font.png");
	Map *map = new Map("Media\\Maps\\Map.txt");
	Player *player = new Player(map->getSpawnX(), map->getSpawnY(), map->getSpawnRotation());

	Engine::setMap(map);
	Engine::setPlayer(player);
	Input::setMousePosition(Engine::getWidth() / 2, Engine::getHeight() / 2, false);

	while (!Input::getQuit() && !Input::getKeyDown(SDLK_ESCAPE))
	{
		fpsCounter->Frame();
		player->UpdateInput(.0002f * max(fpsCounter->getFrameTime(), 1), .05f, .94f, .94f, .25f);

		Engine::Render();
		Graphics::FillRectangle(10, 10, 100, 36, Color(0, 0, 0), .5f);
		Graphics::FillRectangle(120, 10, 130, 88, Color(0, 0, 0), .5f);
		Graphics::DrawString(20, 20, "FPS: " + IntToString(fpsCounter->getFPS()), font);
		Graphics::DrawString(130, 20, "WASD: Move", font);
		Graphics::DrawString(130, 46, "LShift: Sprint", font);
		Graphics::DrawString(130, 72, "Mouse: Rotate", font);

		Graphics::FillRectangle(screenWidth - minimapSize / 2 - 60, 15 + minimapSize, 100, 26, Color(0, 0, 0), .5f);
		Graphics::DrawMinimap(screenWidth - minimapSize - 10, 10, minimapSize, .05f, .75f, .6f);
		Graphics::DrawString(screenWidth - minimapSize / 2 - 40, 20 + minimapSize, FloatToString(player->getX(), 1), font);
		Graphics::DrawString(screenWidth - minimapSize / 2 - 5, 20 + minimapSize, FloatToString(player->getY(), 1), font);

		Engine::Present();
		Input::Update();
		Input::setMousePosition(Engine::getWidth() / 2, Engine::getHeight() / 2, true);
	}

	Engine::Destroy();
	return 0;
}