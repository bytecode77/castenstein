#include "Castenstein.h"

Sprite::Sprite(float x, float y, Texture *texture)
{
	X = x;
	Y = y;
	Rotation = 0;
	SpriteTexture = new AnimatedTexture(texture);
}
Sprite::Sprite(float x, float y, AnimatedTexture *texture, float rotation)
{
	X = x;
	Y = y;
	Rotation = rotation;
	SpriteTexture = texture;
}