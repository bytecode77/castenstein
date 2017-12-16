class FPSCounter
{
private:
	int Interval, FPS, FPSCount, FrameTime, MedianFrameTime;
	clock_t LastMeasuredTime, LastFrame;
public:
	FPSCounter();
	FPSCounter(int interval);

	void Frame();

	int getInterval();
	int getFPS();
	int getFrameTime();
	int getMedianFrameTime();

	void setInterval(int interval);
};