#pragma once
#include "cocos2d.h"
#include "MainState.h"
#include "RankData.h"

USING_NS_CC;

class IntroState : public CCLayer
{
	int numberOfSprites;
	int loadedNumberOfSprites;

	cocos2d::CCLabelTTF *m_pLabelLoading;
    cocos2d::CCLabelTTF *m_pLabelPercent;

	CCSprite* introImage;
public:
	virtual bool init();
	static CCScene* scene();

	CREATE_FUNC(IntroState);
	void loadingCallback(CCObject* pSender);

};