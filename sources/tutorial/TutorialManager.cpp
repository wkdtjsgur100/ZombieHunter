#include "TutorialManager.h"

TutorialManager::TutorialManager(CCLayer* target)
{
	this->target = target;
	back = CCSprite::create("tutorial/tuto_back.png");
	back->setPosition(ccp(target->getContentSize().width*0.5,target->getContentSize().height*0.5));
	this->tutoCheck = 0;
	currentPage = 1;
	moveCheck = 0.0f;
	isExplainDead = false;
}
void TutorialManager::explainOne()
{
	back = CCSprite::create("tutorial/tuto_back.png");
	back->setPosition(ccp(target->getContentSize().width*0.5,target->getContentSize().height*0.5));
	
	target->addChild(back,ZORDER_TUTORIAL_BACK);

	CCSprite* messageBox = CCSprite::create("tutorial/message_popup_box01.png");

	messageBox->setPosition(ccp(480,330));

	CCSprite* touchIcon = CCSprite::create("tutorial/touch_icon.png");
	CCSprite* touchIcon2 = CCSprite::create("tutorial/touch_icon.png");

	touchIcon->setPosition(ccp(148,130));
	touchIcon2->setPosition(ccp(900,130));

	back->addChild(messageBox);
	back->addChild(touchIcon);
	back->addChild(touchIcon2);
}
void TutorialManager::removeExplain()
{
	target->removeChild(back);
}
void TutorialManager::explainTwo()
{
	back = CCSprite::create("tutorial/tuto_back.png");
	back->setPosition(ccp(target->getContentSize().width*0.5,target->getContentSize().height*0.5));
	
	back->setOpacity(0);

	target->addChild(back,ZORDER_TUTORIAL_BACK);

	CCSprite* messageBox = CCSprite::create("tutorial/message_popup_box02.png");
	
	messageBox->setPosition(ccp(480,330));

	back->addChild(messageBox);
}

void TutorialManager::explainThree()
{
	back = CCSprite::create("tutorial/tuto_back.png");
	back->setPosition(ccp(target->getContentSize().width*0.5,target->getContentSize().height*0.5));
	
	//back->setOpacity(0);

	target->addChild(back,ZORDER_TUTORIAL_BACK);

	CCSprite* messageBox = CCSprite::create("tutorial/message_popup_box03.png");
	
	messageBox->setPosition(ccp(480,330));

	CCSprite* touchIcon[5]; 
	for(int i=0;i<5;i++) 
	{
		touchIcon[i] = CCSprite::create("tutorial/touch_icon.png");
		touchIcon[i]->setPosition(ccp(310+100*i,137));
		back->addChild(touchIcon[i]);
	}
	back->addChild(messageBox);
}
void TutorialManager::explainFour()
{
	back = CCSprite::create("tutorial/tuto_back.png");
	back->setPosition(ccp(target->getContentSize().width*0.5,target->getContentSize().height*0.5));
	
	//back->setOpacity(0);

	target->addChild(back,ZORDER_TUTORIAL_BACK);

	CCSprite* messageBox = CCSprite::create("tutorial/message_popup_box05.png");
	
	messageBox->setPosition(ccp(480,430));

	back->addChild(messageBox);

	CCSprite* moveMessage = CCSprite::create("tutorial/move_watchbox.png");

	moveMessage->setPosition(ccp(464,260));
	
	back->addChild(moveMessage);
	
	CCSprite* touchIcon = CCSprite::create("tutorial/touch_icon.png");

	touchIcon->setPosition(ccp(160,110));

	back->addChild(touchIcon);
}
void TutorialManager::explainAgainRemoveZombie()
{
	back = CCSprite::create("tutorial/tuto_back.png");
	back->setPosition(ccp(target->getContentSize().width*0.5,target->getContentSize().height*0.5));
	
	back->setOpacity(0);

	target->addChild(back,ZORDER_TUTORIAL_BACK);

	CCSprite* messageBox = CCSprite::create("tutorial/message_popup_box04.png");
	
	messageBox->setPosition(ccp(480,330));

	back->addChild(messageBox);
}
void TutorialManager::explainDead(UserInterface* targetInterface)
{
	this->targetInterface = targetInterface;

	back = CCSprite::create("tutorial/tuto_back.png");
	back->setPosition(ccp(target->getContentSize().width*0.5,target->getContentSize().height*0.5));

	target->addChild(back,ZORDER_TUTORIAL_BACK);

	CCSprite* messageBox = CCSprite::create("tutorial/message_popup_box07.png");
	
	messageBox->setPosition(ccp(480,330));

	back->addChild(messageBox);

	CCMenuItemImage* confirm = CCMenuItemImage::create("tutorial/confirm.png","tutorial/confirm.png",this,
		menu_selector(TutorialManager::deadExplainEnd));

	confirm->getSelectedImage()->setScale(0.9f,0.9f);

	CCMenu* menu = CCMenu::create(confirm,NULL);

	menu->setPosition(ccp(480,220));

	back->addChild(menu);
}
void TutorialManager::deadExplainEnd(CCObject* pSender)
{
	target->removeChild(back);
	target->resumeSchedulerAndActions();

	targetInterface->gaugeBar->setZOrder(ZORDER_INTERFACE);
	targetInterface->hpGaugeCenter->setZOrder(ZORDER_INTERFACE);
	targetInterface->hpGaugeSide->setZOrder(ZORDER_INTERFACE);
	targetInterface->hpPercent->setZOrder(ZORDER_INTERFACE);
	targetInterface->characterLabel->setZOrder(ZORDER_INTERFACE);

	isExplainDead = true;

	switch(currentPage)
	{
	case 1:
	case 2: 	
		explainOne(); 
		break;
	case 5:
	case 6:
	case 8:
	case 9:
		explainTwo();
		break;
	case 7:
		reloadExplain();
		break;
	case 10:
	case 11:
		explainThree();
		break;
	case 12:
	case 13:
		explainAgainRemoveZombie();
		break;
	case 14:
	case 15:
		explainFour();
		break;
	case 16:
	case 17:
		explainFive();
		break;
	}
}
void TutorialManager::explainFive()
{
	back = CCSprite::create("tutorial/tuto_back.png");
	back->setPosition(ccp(target->getContentSize().width*0.5,target->getContentSize().height*0.5));

	target->addChild(back,ZORDER_TUTORIAL_BACK);

	CCSprite* messageBox = CCSprite::create("tutorial/message_popup_box06.png");
	
	messageBox->setPosition(ccp(480,194));

	back->addChild(messageBox);

	CCMenuItemImage* tutorialEnd = CCMenuItemImage::create("tutorial/tutorial_end_button.png","tutorial/tutorial_end_button.png",this,
		menu_selector(TutorialManager::tutorialEnd));

	tutorialEnd->getSelectedImage()->setScale(0.9f,0.9f);

	CCMenu* menu = CCMenu::create(tutorialEnd,NULL);

	menu->setPosition(ccp(480,85));

	back->addChild(menu);

	for(int i=0;i<5;i++)
	{
			touchIcon[i] = CCSprite::create("tutorial/touch_icon.png");
			touchIcon[i]->setPosition(ccp(338+100*i,470));
			target->addChild(touchIcon[i],ZORDER_TUTORIAL_BACK+2);
	}
}
void TutorialManager::reloadExplain()
{
	back = CCSprite::create("tutorial/tuto_back.png");
	back->setPosition(ccp(target->getContentSize().width*0.5,target->getContentSize().height*0.5));
	
	target->addChild(back,ZORDER_TUTORIAL_BACK);

}
void TutorialManager::tutorialEnd(CCObject* pSender)
{
	for(int i=0;i<5;i++) 
		target->removeChild(touchIcon[i]);
	target->removeChild(back);

	currentPage++;
	target->resumeSchedulerAndActions();
}
