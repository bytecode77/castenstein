struct Color
{
public:
	int R, G, B;

	Color();
	Color(int r, int g, int b);
	Color(int rgb);

	int getRGB();
};