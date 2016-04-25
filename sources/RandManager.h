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
	
	//�ߺ��� ���� Rand�� �������ش�. 0 ~ 63 ���� ��������.
	unsigned int getCompleteRand();
};