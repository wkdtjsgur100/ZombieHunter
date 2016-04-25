#pragma once
#include "Camera.h"

#include "cocos2d.h"
USING_NS_CC;

class ScoreManager
{
private:
	ScoreManager();
	static ScoreManager* mInstance;
public:
	unsigned long moveDistance;
	unsigned long point;
public:
	static ScoreManager* sharedInstance();
	void release();
};