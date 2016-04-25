#include "PauseState.h"
#include "MainState.h"

PauseState* PauseState::instance = NULL;

void PauseState::pause()
{
	if(!isPauseOn)
	{
		if(resumeTarget)
		{
			resumeTarget->pauseSchedulerAndActions();
			resumeTarget->setInterfaceEnabled(false);
			///레이어추가 
			//this->setTouchPriority(-300);
			CCDirector::sharedDirector()->getRunningScene()->addChild(instance,1);
			
			isPauseOn = true;
		}
	}
}
void PauseState::popScene(CCObject* pSender)
{
	if(resumeTarget)
	{
		resumeTarget->resumeSchedulerAndActions();
		resumeTarget->setInterfaceEnabled(true);
		
		CCDirector::sharedDirector()->getRunningScene()->removeChild(instance);

		isPauseOn = false;
	}
}
void PauseState::setResumeTarget(GameState* gameState)
{
	resumeTarget = gameState;
}
PauseState::PauseState()
{
	CCLayer::init();

	background = CCSprite::create("pause/pause_background.png");

	background->setPosition(ccp(background->getContentSize().width*0.5,background->getContentSize().height*0.5));

	this->addChild(background);

	CCMenuItemImage* resumeQuestionImage = CCMenuItemImage::create("pause/resume.png","pause/resume.png",
		NULL,this,menu_selector(PauseState::popScene));

	resumeQuestionImage->getSelectedImage()->setScale(0.9f,0.9f);

	resumeQuestionImage->setPosition(ccp(0,0));

	CCMenuItemImage* stopQuestionImage = CCMenuItemImage::create("pause/stop.png","pause/stop.png",NULL,this,
		menu_selector(PauseState::goMenu));

	stopQuestionImage->getSelectedImage()->setScale(0.9f,0.9f);

	menu = CCMenu::create();

	menu->addChild(resumeQuestionImage);
	menu->addChild(stopQuestionImage);

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	menu->setPosition(ccp(size.width*0.5,size.height*0.5));

	menu->alignItemsVertically();

	this->addChild(menu);

	isPauseOn = false;
}

void PauseState::goMenu(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(MainState::scene());
}
PauseState* PauseState::getInstance()
{
	if(!instance)
		instance = new PauseState();
	return instance;
}

void PauseState::end()
{
	if(instance)
	{
		instance->removeAllChildrenWithCleanup(true);
		CCDirector::sharedDirector()->getRunningScene()->removeChild(instance);
	}
	instance = NULL;
	resumeTarget = NULL;
}