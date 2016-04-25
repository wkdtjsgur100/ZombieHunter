#pragma once

#include "GameState.h"

class GameState;

class PauseState : public CCLayer
{
	bool isPauseOn;

	CCSprite* background;
	CCMenu* menu;
	GameState* resumeTarget;
	static PauseState* instance;

	PauseState();
	~PauseState(){}

	void popScene(CCObject* pSender);
	void goMenu(CCObject* pSender);
public:
	static PauseState* getInstance();
	void pause();
	bool isInGame(){ return (resumeTarget) ? true : false ; }
	void setResumeTarget(GameState* gameState);
	void end();
};