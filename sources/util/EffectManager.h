#pragma once

#include "cocos2d.h"
#include "MoveObjectByCamera.h"
#include "UserDataInBody.h"

USING_NS_CC;

struct LabelEffectInfo
{
	ccColor3B color; 
	CCPoint deltaPosition;
	float fontSize;
	float duration;

	LabelEffectInfo()
	{
		duration = 1.0f;
		color = ccc3(255,255,255);
		deltaPosition=ccp(0,100.0f);
		fontSize = 30.0f;
	}
};

class EffectManager : public MoveObjectByCamera
{
	static EffectManager* instance;
	std::list<CCLabelTTF*> moveLabels;
	std::list<CCLabelTTF*> interfaceLabels;
	std::list<CCSprite*> sprs;

	CCLayer* target;
private:
	EffectManager()
	{
	}
	
	CCLabelTTF* labelEffect(std::string str,CCPoint startPos,LabelEffectInfo ei);
public:
	void update();
	void release();
	static EffectManager* getInstance();
	void setTarget(CCLayer* target);
	
	void moveLabelEffect(std::string str,CCPoint startPos,LabelEffectInfo ei);
	void InterfaceLabelEffect(std::string str,CCPoint startPos,LabelEffectInfo ei);

	void moveByCamera(float dX);
};

