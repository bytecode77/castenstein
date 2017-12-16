class AnimatedTexture
{
public:
	Texture **Frames;
	int FrameCount;

	AnimatedTexture(string path, Color fogColor, float ambientBrightness);
	AnimatedTexture(Texture *texture);
	~AnimatedTexture();
};