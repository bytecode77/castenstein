class Graphics
{
public:
	static void FillRectangle(int x, int y, int width, int height, Color color);
	static void FillRectangle(int x, int y, int width, int height, Color color, float opacity);
	static void DrawString(int x, int y, string text, Font *font);
	static void DrawString(int x, int y, string text, Font *font, Color color);
	static void DrawMinimap(int x, int y, int size, float relativeBlockSize, float opacity, float shadedAreaBrightness);
};