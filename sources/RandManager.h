#pragma once

#include "cocos2d.h"

USING_NS_CC;

class RandManager
{
	unsigned long flag;
	int max,min;
	int count;
private:
	void _refreshFlag();
public:
	RandManager(unsigned int min,unsigned int max);
	RandManager();

	void setRange(unsigned int min,unsigned int max);
	
	//중복이 없는 Rand를 추출해준다. 0 ~ 63 까지 지원가능.
	unsigned int getCompleteRand();
};