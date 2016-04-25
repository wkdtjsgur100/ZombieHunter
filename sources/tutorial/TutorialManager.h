#pragma once

#include "UserDataInBody.h"
#include "system/UserInfo.h"
#include "ui/Buttons.h"
#include "ui/UserInterface.h"


class TutorialManager : public CCObject
{
	CCLayer* target;
	CCSprite* back;
	CCMenuItemImage* check;
	int currentPage;
	CCSprite* touchIcon[5]; 
	UserInterface* targetInterface;
public:
	bool isExplainDead;
	float moveCheck;

	int tutoCheck;//튜토리얼박스에서 무엇을 입력받았는지 체크, 입력이 아니면 0, '예'면 -1, '아니오'면 1이다.
	TutorialManager(CCLayer* target);

	int getCurrentPage(){ return currentPage; }
	void setCurrentPage(int currentPage) { this->currentPage = currentPage; }
	void increasePage(){ currentPage++; }

	void tutorialEnd(CCObject* pSender);
	void deadExplainEnd(CCObject* pSender);

	void explainAgainRemoveZombie();
	
	void explainDead(UserInterface* targetInterface);

	void explainOne();
	
	void explainTwo();

	void explainThree();

	void explainFour();

	void explainFive();

	void reloadExplain();

	void removeExplain();
};