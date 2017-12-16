class Sprite
{
private:
	float Depth;

	bool Render;
	Texture *RenderTexture;
	int RenderStartX;
	int RenderEndX;
	int RenderStartY;
	int RenderEndY;
	int RenderShadowLevel;
	int RenderFogLevel;
	float RenderU;
	float RenderV;
	float RenderStepUV;
public:
	float X, Y, Rotation;
	AnimatedTexture *SpriteTexture;

	Sprite(float x, float y, Texture *texture);
	Sprite(float x, float y, AnimatedTexture *texture, float rotation);

	friend class Engine;
};