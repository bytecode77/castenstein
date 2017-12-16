class Map
{
private:
	Point Size;
	Block ***Blocks;
	float FogRange;
	Color FogColor;
	float AmbientBrightness;
	float SpawnX, SpawnY, SpawnRotation;
public:

	Map(int width, int height);
	Map(string path);
	~Map();

	Point getSize();
	Block *getBlock(int x, int y);
	float getFogRange();
	Color getFogColor();
	float getAmbientBrightness();
	float getSpawnX();
	float getSpawnY();
	float getSpawnRotation();

	void setBlock(int x, int y, Block *block);
	void setFogRange(float fogRange);
	void setFogColor(Color fogColor);
	void setAmbientBrightness(float ambientBrightness);
	void setSpawnX(float spawnX);
	void setSpawnY(float spawnY);
	void setSpawnRotation(float spawnRotation);

	friend class Engine;
	friend class Graphics;
	friend class Player;
	friend class Texture;
};