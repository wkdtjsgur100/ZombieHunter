#include "GameOverState.h"
#include "MainState.h"

bool GameOverState::init()
{
	if(!CCLayer::init())
		return false;

	CCSprite* result_box = CCSprite::create("over/result_box.png");

	result_box->setPosition(ccp(getContentSize().width*0.5,getContentSize().height*0.5));

	this->addChild(result_box);

	CCMenuItemImage* ok_button = CCMenuItemImage::create("over/ok_button.png","over/ok_button.png",NULL,
		this,menu_selector(GameOverState::comeBackMenu));
	
	ok_button->getSelectedImage()->setScale(0.9f,0.9f);

	CCMenuItemImage* retry_button = CCMenuItemImage::create("over/retry_button.png","over/retry_button.png",NULL,
		this,menu_selector(GameOverState::replay));
	
	retry_button->getSelectedImage()->setScale(0.9f,0.9f);

	buts  = Buttons::create();

	buts->setMusicSrc("music/ui/ui_click.ogg");

	buts->addChild(ok_button);
	buts->addChild(retry_button);

	buts->alignItemsHorizontallyWithPadding(30.0f);

	buts->setPosition(ccp(result_box->getContentSize().width*0.5,55));

	result_box->addChild(buts);

	moveDistLabel = CCLabelTTF::create(CCString::createWithFormat("%d",ScoreManager::sharedInstance()->moveDistance)
		->m_sString.c_str(),
		"fonts/rank_font.ttf",30.0f,CCSizeMake(200,50),kCCTextAlignmentRight);

	moveDistLabel->setPosition(ccp(300,292));

	pointLabel = CCLabelTTF::create(CCString::createWithFormat("%d",ScoreManager::sharedInstance()->point)
		->m_sString.c_str(),
		"fonts/rank_font.ttf",30.0f,CCSizeMake(200,50),kCCTextAlignmentRight);
	pointLabel->setPosition(ccp(300,240));

	unsigned long totalScore = ScoreManager::sharedInstance()->point + ScoreManager::sharedInstance()->moveDistance;

	totalScoreLabel = CCLabelTTF::create(CCString::createWithFormat("%d",totalScore)
		->m_sString.c_str(),
		"fonts/rank_font.ttf",30.0f,CCSizeMake(200,50),kCCTextAlignmentRight);

	totalScoreLabel->setPosition(ccp(300,177));

	bestScoreLabel = CCLabelTTF::create(UserInfo::getInstance()->getBestScore().c_str(),
		"fonts/rank_font.ttf",30.0f,CCSizeMake(200,50),kCCTextAlignmentRight);

	bestScoreLabel->setPosition(ccp(300,107));
	bestScoreLabel->setColor(ccc3(255,255,0));

	if( totalScore >  atoi(UserInfo::getInstance()->getBestScore().c_str()))
	{
		bestScoreLabel->setString(CCString::createWithFormat("%d",totalScore)->m_sString.c_str());
		UserInfo::getInstance()->registerPreferenceBestScore(CCString::createWithFormat("%d",totalScore)->m_sString.c_str());

		CCSprite* new_record = CCSprite::create("over/new_record.png");

		CCMoveBy* moveAction = CCMoveBy::create(1.0f,ccp(0,100.0f));
		CCFadeOut* fadeAction = CCFadeOut::create(1.0f);

		CCSpawn* totalAction = CCSpawn::create(moveAction,fadeAction,NULL);

		new_record->runAction(totalAction);

		new_record->setPosition(ccp(300,107));

		result_box->addChild(new_record,1);
	}
	result_box->addChild(moveDistLabel);
	result_box->addChild(pointLabel);
	result_box->addChild(totalScoreLabel);
	result_box->addChild(bestScoreLabel);

	return true;
}

void GameOverState::comeBackMenu(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(MainState::scene());
}
void GameOverState::replay(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(GameState::scene());
}

CCScene* GameOverState::scene()
{
	CCScene *scene = CCScene::create();
	GameOverState *layer = GameOverState::create();
	
	scene->addChild(layer);

	return scene;
}