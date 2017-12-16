class Engine
{
private:
	static int Width;
	static int Height;
	static SDL_Window *Window;
	static SDL_Renderer *Renderer;
	static SDL_Texture *ScreenTexture;

	static int ThreadCount;
	static RenderThread **Threads;
	static float *FloorDistanceCache;
	static int *FloorBrightnessCache;
	static int *SkyVCache;

	static int *BackBuffer;
	static float *DepthBuffer;
	static Map *CurrentMap;
	static Sky *CurrentSky;
	static Player *CurrentPlayer;
	static list<Sprite*> *Sprites;

	static int RenderThreadFunction(void *data);
	static SDL_Surface* LoadSDLSurface(string path);
public:
	static void Initialize(int width, int height);
	static void Destroy();
	static void Render();
	static void Present();
	static void AddSprite(Sprite *sprite);

	static string getTitle();
	static int getWidth();
	static int getHeight();
	static Map *getMap();
	static Sky *getSky();
	static Player *getPlayer();

	static void setTitle(string title);
	static void setIcon(string path);
	static void setMap(Map *map);
	static void setSky(Sky *sky);
	static void setPlayer(Player *player);

	friend class Input;
	friend class Graphics;
	friend class Font;
	friend class Player;
	friend class Texture;
	friend class AnimatedTexture;
	friend class Sky;
};