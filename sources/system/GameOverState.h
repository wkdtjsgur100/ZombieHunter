#pragma once

#include "cocos2d.h"
#include "ui/Buttons.h"
#include "ScoreManager.h"
#include "system/UserInfo.h"

USING_NS_CC;

class GameOverState : public CCLayer
{
	Buttons* buts;
	CCLabelTTF* moveDistLabel;
	CCLabelTTF* pointLabel;
	CCLabelTTF* totalScoreLabel;
	CCLabelTTF* bestScoreLabel;
public:
	virtual bool init();
	static CCScene* scene();

	void comeBackMenu(CCObject* pSender);
	void replay(CCObject* pSender);

	CREATE_FUNC(GameOverState);
};