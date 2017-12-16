class Texture
{
private:
	int Size;
	int SizeExponent;
	int ***Buffer;
	Color AverageColor;

	Texture();

	void LoadFromSDLSurface(SDL_Surface *surface, int frame, int frameCount, Color fogColor, float ambientBrightness);
public:
	Texture(string path, Color fogColor, float ambientBrightness);
	~Texture();

	friend class Engine;
	friend class Graphics;
	friend class AnimatedTexture;
};