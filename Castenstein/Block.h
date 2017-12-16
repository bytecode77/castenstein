class Block
{
private:
	Block();
public:
	bool IsWall;
	Texture *BlockTexture;
	Texture *CeilingTexture;

	Block(bool isWall, Texture *blockTexture, Texture *ceilingTexture);

	friend class Map;
};