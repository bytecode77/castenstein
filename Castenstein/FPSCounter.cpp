#include "Castenstein.h"

FPSCounter::FPSCounter()
{
	Interval = 1000;
	FPS = 0;
	FPSCount = 0;
	FrameTime = 0;
	MedianFrameTime = 0;
	LastMeasuredTime = clock();
}
FPSCounter::FPSCounter(int interval)
{
	Interval = interval;
	FPS = 0;
	FPSCount = 0;
	FrameTime = 0;
	MedianFrameTime = 0;
	LastMeasuredTime = clock();
}

void FPSCounter::Frame()
{
	clock_t time = clock();
	if (time - LastMeasuredTime > Interval)
	{
		FPS = FPSCount * 1000 / Interval;
		MedianFrameTime = FPSCount == 0 ? 0 : Interval / FPSCount;
		FPSCount = 0;
		LastMeasuredTime = time;
	}
	FrameTime = time - LastFrame;
	LastFrame = time;
	FPSCount++;
}

int FPSCounter::getInterval()
{
	return Interval;
}
int FPSCounter::getFPS()
{
	return FPS;
}
int FPSCounter::getFrameTime()
{
	return FrameTime;
}
int FPSCounter::getMedianFrameTime()
{
	return MedianFrameTime;
}

void FPSCounter::setInterval(int interval)
{
	Interval = interval;
}