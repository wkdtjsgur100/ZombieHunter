#pragma once

#include "GameState.h"

class TutorialState : public GameState
{
	CCSprite* move;
public:
	virtual bool init();
	static CCScene* scene();

	void tutorialUpdate(float dt);
	void deadExplainEnd(CCObject* pSender);
	CREATE_FUNC(TutorialState);
};