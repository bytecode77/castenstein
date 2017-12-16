#include "Castenstein.h"

Map::Map(int width, int height)
{
	SpawnX = 0;
	SpawnY = 0;
	SpawnRotation = 0;
	FogColor = Color(0, 0, 0);
	AmbientBrightness = .5f;

	Size = Point(width, height);
	Blocks = new Block**[Size.Y];

	for (int y = 0; y < Size.Y; y++)
	{
		Blocks[y] = new Block*[Size.X];
		for (int x = 0; x < Size.X; x++) Blocks[y][x] = new Block();
	}
}
Map::Map(string path)
{
	SpawnX = 0;
	SpawnY = 0;
	SpawnRotation = 0;
	FogColor = Color(0, 0, 0);
	AmbientBrightness = .5f;
	Blocks = nullptr;

	int mapWidth = -1, mapHeight = -1;
	string str, str2, section = "";
	vector<char> textureChars = vector<char>();
	vector<bool> textureWalls = vector<bool>();
	vector<Texture*> textures = vector<Texture*>();
	vector<string> spriteNames = vector<string>();
	vector<bool> spritesAnim = vector<bool>();
	vector<AnimatedTexture*> spriteTextures = vector<AnimatedTexture*>();

	ifstream file;
	file.open(path);

	while (!file.eof())
	{
		file >> str;
		if (str[0] == '[' && str[str.length() - 1] == ']')
		{
			section = str.substr(1, str.length() - 2);
			if (section != "map" && section != "ceiling") continue;
		}

		if (section == "properties")
		{
			if (str == "width")
			{
				file >> mapWidth;
				if (mapWidth < 1) throw;
			}
			else if (str == "height")
			{
				file >> mapHeight;
				if (mapHeight < 1) throw;
			}
			else if (str == "sky")
			{
				file >> str;
				Engine::setSky(new Sky(str));
			}
			else if (str == "fogrange")
			{
				file >> FogRange;
			}
			else if (str == "fogcolor")
			{
				file >> FogColor.R >> FogColor.G >> FogColor.B;
			}
			else if (str == "ambient")
			{
				file >> AmbientBrightness;
			}
			else if (str == "spawn")
			{
				file >> SpawnX >> SpawnY >> SpawnRotation;
			}
			else
			{
				throw;
			}
			if (Blocks == nullptr && mapWidth > -1 && mapHeight > -1)
			{
				Size = Point(mapWidth, mapHeight);
				Blocks = new Block**[Size.Y];
				for (int y = 0; y < Size.Y; y++)
				{
					Blocks[y] = new Block*[Size.X];
				}
			}
		}
		else if (section == "textures.map")
		{
			textureChars.push_back(str[0]);
			file >> str >> str2;
			if (str == "wall")
			{
				textureWalls.push_back(true);
			}
			else if (str == "floor")
			{
				textureWalls.push_back(false);
			}
			else
			{
				throw;
			}
			textures.push_back(new Texture(str2, FogColor, AmbientBrightness));
		}
		else if (section == "textures.sprites")
		{
			spriteNames.push_back(str);
			file >> str >> str2;
			if (str == "static") spritesAnim.push_back(false);
			else if (str == "anim") spritesAnim.push_back(true);
			else throw;
			spriteTextures.push_back(new AnimatedTexture(str2, FogColor, AmbientBrightness));
		}
		else if (section == "map" || section == "ceiling")
		{
			getline(file, str);
			for (int y = 0; y < Size.Y; y++)
			{
				getline(file, str);
				if (str.length() != Size.X) throw;
				for (int x = 0; x < Size.X; x++)
				{
					bool found = false;
					for (int i = 0; i < int(textureChars.size()); i++)
					{
						if (str[x] == textureChars[i])
						{
							if (section == "map") Blocks[y][x] = new Block(textureWalls[i], textures[i], nullptr);
							else Blocks[y][x]->CeilingTexture = textures[i];
							found = true;
							break;
						}
					}
					if (section == "map" && !found) Blocks[y][x] = new Block();
				}
			}
		}
		else if (section == "sprites")
		{
			string::size_type sz;
			float spriteX = stof(str.c_str(), &sz), spriteY, rotation = 0;
			file >> spriteY >> str;
			bool found = false;
			for (int i = 0; i < int(spriteNames.size()); i++)
			{
				if (str == spriteNames[i])
				{
					if (spritesAnim[i]) file >> rotation;
					Engine::AddSprite(new Sprite(spriteX, spriteY, spriteTextures[i], rotation));
					found = true;
					break;
				}
			}
			if (!found) throw;
		}
		else
		{
			throw;
		}
	}
	file.close();
}
Map::~Map()
{
	delete[] Blocks;
}

Point Map::getSize()
{
	return Size;
}
Block *Map::getBlock(int x, int y)
{
	return Blocks[y][x];
}
float Map::getFogRange()
{
	return FogRange;
}
Color Map::getFogColor()
{
	return FogColor;
}
float Map::getAmbientBrightness()
{
	return AmbientBrightness;
}
float Map::getSpawnX()
{
	return SpawnX;
}
float Map::getSpawnY()
{
	return SpawnY;
}
float Map::getSpawnRotation()
{
	return SpawnRotation;
}

void Map::setBlock(int x, int y, Block *block)
{
	Blocks[y][x] = block;
}
void Map::setFogRange(float fogRange)
{
	FogRange = fogRange;
}
void Map::setFogColor(Color fogColor)
{
	FogColor = fogColor;
}
void Map::setAmbientBrightness(float ambientBrightness)
{
	AmbientBrightness = ambientBrightness;
}
void Map::setSpawnX(float spawnX)
{
	SpawnX = spawnX;
}
void Map::setSpawnRotation(float spawnRotation)
{
	SpawnRotation = spawnRotation;
}
void Map::setSpawnY(float spawnY)
{
	SpawnY = spawnY;
}