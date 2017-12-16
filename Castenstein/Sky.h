class Sky
{
private:
	int Width;
	int Height;
	int WidthExponent;
	int *Buffer;

	Sky();
public:
	Sky(string path);
	~Sky();

	static Sky* CreateDummySky();

	friend class Engine;
};