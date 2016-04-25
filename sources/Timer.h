#pragma once

#include <cocos2d.h>

class Timer
{
	float genMaxTime,genMinTime;
	float clock;
public:
	Timer()
	{
		clock = 0.0f;
	}
	void clocking()
	{
		clock -= Timer::getDt();
	}
	bool isTimeOver()
	{
		if(clock <= 0)
			return true;
		return false;
	}
	void setGenTimeRand(float genMaxTime,float genMinTime);
	void setGenTime(long time);
	void updateRandClock();
	void update(float dt);

	static unsigned long getCurrentTime();
	static float getDt();

private :
	static unsigned long currentTime;
	static float dt;

};