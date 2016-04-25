#include "OptionState.h"

bool OptionState::init()
{
	if(!CCLayer::init())
		return false;

	CCSprite* back = CCSprite::create("main_screen/option/option_back.png");
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	back->setPosition(ccp(winSize.width*0.5,winSize.height*0.5));

	this->addChild(back);

	CCMenuItemImage* image = CCMenuItemImage::create("main_screen/option/go_home.png","main_screen/option/go_home_press.png",NULL,this,
		menu_selector(OptionState::popScene));

	CCMenuItemImage* modify = CCMenuItemImage::create("main_screen/option/modify_name.png","main_screen/option/modify_name_press.png",
		NULL,this,
		menu_selector(OptionState::modifyName));

	menu = CCMenu::create(image,modify,NULL);
	
	menu->alignItemsVertically();

	this->addChild(menu);

	return true;
}
void OptionState::modifyName(CCObject* pSender)
{
	menu->setTouchEnabled(false);

	this->addChild(inputBox,2);
}
void OptionState::editBoxEditingDidEnd(CCEditBox* editBox)
{
	menu->setTouchEnabled(true);

	UserInfo::getInstance()->registerPreferenceName(editBox->getText());

	this->removeChild(editBox);
}
CCScene* OptionState::scene()
{
	CCScene *scene = CCScene::create();
	OptionState *layer = OptionState::create();

	scene->addChild(layer);
	
	return scene;
}

void OptionState::popScene(CCObject* pSender)
{
	CCDirector::sharedDirector()->popScene();
}
