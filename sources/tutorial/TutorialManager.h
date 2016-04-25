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

	int tutoCheck;//Ʃ�丮��ڽ����� ������ �Է¹޾Ҵ��� üũ, �Է��� �ƴϸ� 0, '��'�� -1, '�ƴϿ�'�� 1�̴�.
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