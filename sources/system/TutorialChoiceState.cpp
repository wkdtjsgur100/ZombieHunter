#include "TutorialChoiceState.h"

bool TutorialChoiceState::init()
{
	if(!CCLayer::init())
		return false;

	back = CCSprite::create("tutorial/tuto_back.png");
	back->setPosition(ccp(this->getContentSize().width*0.5,this->getContentSize().height*0.5));
	
	this->addChild(back,ZORDER_TUTORIAL_BACK);

	CCSprite* box_back = CCSprite::create("tutorial/message_popup_box00.png");

	box_back->setPosition(ccp(491,320));

	CCMenuItemImage* confirm = CCMenuItemImage::create("tutorial/yes_button.png","tutorial/yes_button.png",this,menu_selector(TutorialChoiceState::tutorialContinue));
	CCMenuItemImage* cancel = CCMenuItemImage::create("tutorial/no_button.png","tutorial/no_button.png",this,menu_selector(TutorialChoiceState::tutorialStop));
	
	confirm->getSelectedImage()->setScale(0.9f,0.9f);
	cancel->getSelectedImage()->setScale(0.9f,0.9f);

	Buttons* but = Buttons::create();

	but->setMusicSrc("music/ui/ui_click.ogg");

	confirm->setPosition(ccp(120,42));
	cancel->setPosition(ccp(270,42));

	but->addChild(confirm);
	but->addChild(cancel);

	but->setPosition(0,0);
	
	CCSprite* nontuto_box = CCSprite::create("tutorial/nontutorial_box.png");

	nontuto_box->setPosition(ccp(253,-25));

	check = CCMenuItemImage::create("tutorial/check_in.png","tutorial/check_in.png",this,menu_selector(TutorialChoiceState::checkIn));

	check->setPosition(ccp(0,0));
	CCMenu* check_menu = CCMenu::create(check,NULL);

	check->setOpacity(0);

	check_menu->setPosition(345,-25);

	box_back->addChild(nontuto_box);
	box_back->addChild(check_menu);
	box_back->addChild(but);

	back->addChild(box_back);
	return true;
}
void TutorialChoiceState::checkIn(CCObject* pSender)
{
	if(check->getOpacity() == 0)
		check->setOpacity(255);
	else
		check->setOpacity(0);
}
void TutorialChoiceState::tutorialContinue(CCObject* pSender)
{
	this->removeChild(back);

	if(check->getOpacity() == 255)
		UserInfo::getInstance()->setTutorialCheck(false);

	CCDirector::sharedDirector()->replaceScene(TutorialState::scene());
}
void TutorialChoiceState::tutorialStop(CCObject* pSender)
{
	this->removeChild(back);
	
	if(check->getOpacity() == 255)
		UserInfo::getInstance()->setTutorialCheck(false);

	CCDirector::sharedDirector()->replaceScene(GameState::scene());
}
CCScene* TutorialChoiceState::scene()
{
	CCScene *scene = CCScene::create();
	TutorialChoiceState *layer = TutorialChoiceState::create();
	
	scene->addChild(layer);

	return scene;
}