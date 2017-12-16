#include "Castenstein.h"

Block::Block()
{
	IsWall = false;
	BlockTexture = nullptr;
	CeilingTexture = nullptr;
}
Block::Block(bool isWall, Texture *blockTexture, Texture *ceilingTexture)
{
	IsWall = isWall;
	BlockTexture = blockTexture;
	CeilingTexture = ceilingTexture;
}