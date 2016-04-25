#pragma once

#include "UserDataInBody.h"
#include "ScoreManager.h"
#include "system/UserInfo.h"

class UserInterface
{
public:
	CCSprite* hpGaugeCenter;
	CCSprite* hpGaugeSide;

	CCSprite* mitterBox;
	CCSprite* gaugeBar;
	CCSprite* bestScoreLabel;
	CCSprite* scoreLabel;
	CCSprite* pointLabel;
	CCSprite* characterLabel;
	CCLabelTTF* meterLabel;
	CCLabelTTF* score;
	CCLabelTTF* bestScore;
	CCLabelTTF* movingDist;
	CCLabelTTF* savingPoint;
	CCLabelTTF* hpPercent;
public:
	float hpRate;

	UserInterface(CCLayer* target);
	void update(float dt);
	//void init(CCLayer* target);
};