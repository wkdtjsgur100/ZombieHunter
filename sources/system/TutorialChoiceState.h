#pragma once

#include "UserDataInBody.h"
#include "ui/Buttons.h"
#include "system/UserInfo.h"
#include "GameState.h"
#include "TutorialState.h"

class TutorialChoiceState : public CCLayer
{
	CCSprite* back;
	CCMenuItemImage* check;
public:
	virtual bool init();
	static CCScene* scene();
	
	void checkIn(CCObject* pSender);

	void tutorialContinue(CCObject* pSender);
	void tutorialStop(CCObject* pSender);

	CREATE_FUNC(TutorialChoiceState);
};