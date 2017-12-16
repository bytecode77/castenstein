class Font
{
private:
	int Width, Height, CharacterSpacing;
	unsigned char *Buffer;
	Point *CharacterDimensions;
public:
	Font(string path);
	~Font();

	int getCharacterSpacing();

	void setCharacterSpacing(int characterSpacing);

	friend class Graphics;
};