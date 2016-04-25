#include "Timer.h"

float Timer::dt = 0.0f;
unsigned long Timer::currentTime = 0;

void Timer::update(float dt)
{
	currentTime += dt*1000;
	this->dt = dt;
}
void Timer::setGenTimeRand(float genMaxTime,float genMinTime)
{
	if(genMaxTime - genMinTime < 0)
	{
		this->genMaxTime = genMinTime;
		this->genMinTime = genMaxTime;
	}
	else
	{
		this->genMaxTime = genMaxTime;
		this->genMinTime = genMinTime;
	}
	updateRandClock();
}
void Timer::setGenTime(long time)
{
	clock = time;
	clock/=1000;
}

void Timer::updateRandClock()
{
	if(genMaxTime == 0)
	{
		clock = 0.0f;
		return;
	}
	unsigned long re = ( (int)(genMaxTime*1000) - (int)(genMinTime*1000) + 1);

	if(re == 0)
	{
		clock = 0.0f;
		return;
	}
	clock = (float)(rand() % re + (int)genMinTime*1000);
	clock /= 1000.0f;
}
unsigned long Timer::getCurrentTime()
{

	return currentTime;
}

float Timer::getDt()
{
	return dt;
}