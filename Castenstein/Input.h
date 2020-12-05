class Input
{
private:
	static bool Quit;
	static int MouseX;
	static int MouseY;
	static int MouseZ;
	static int MouseXSpeed;
	static int MouseYSpeed;
	static int MouseZSpeed;
	static bool *MouseDown;
	static bool *KeyDown;

	static void Initialize();
	static void Destroy();
public:
	static void Update();

	static bool getQuit();
	static int getMouseX();
	static int getMouseY();
	static int getMouseZ();
	static int getMouseXSpeed();
	static int getMouseYSpeed();
	static int getMouseZSpeed();
	static bool getMouseDown(MouseButton mouseButton);
	static bool getKeyDown(int key);

	static void setMousePosition(int x, int y, bool updateMouseSpeed);

	friend class Engine;
};