#include "MainState.h"

RankTable::RankTable()
{
	createTable();
}
CCSize RankTable::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(389,94);
}
CCTableView* RankTable::getTableView()
{
	return tableView;
}
void RankTable::createTable()
{
	tableView = CCTableView::create(this,CCSizeMake(389,289));
	
	tableView->setDirection(kCCScrollViewDirectionVertical);

	tableView->setVerticalFillOrder(kCCTableViewFillTopDown); //아래부분에서 추가됨..

	tableView->setPosition(ccp(67,33));//기준점

	tableView->setDelegate(this);
	
	RankData::getInstance()->loadRank("장장장",
		"9999999","123123123");
}
CCTableViewCell* RankTable::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	RankCell* cell = new RankCell();
	cell->autorelease();

	if(RankData::getInstance()->IsLoading)
		return cell;

	CCLabelTTF *label = CCLabelTTF::create(RankData::getInstance()->getRank(idx).name.c_str(), "fonts/rank_font.ttf", 30.0);
	label->setColor(ccc3(0,0,0));
	label->setPosition(ccp(120,50));
	label->setAnchorPoint(CCPointZero);


	CCLabelTTF *score_label = CCLabelTTF::create(RankData::getInstance()->getRank(idx).score.c_str(),"fonts/rank_font.ttf",30.0);
	score_label->setColor(ccc3(0,0,0));
	score_label->setPosition(ccp(120,20));
	score_label->setAnchorPoint(CCPointZero);

	char str[10]={0};

	sprintf(str,"%d",idx+1);

	if(idx >= 1)
	{
		int score1 = atoi(RankData::getInstance()->getRank(idx).score.c_str());
		int score2 = atoi(RankData::getInstance()->getRank(idx-1).score.c_str());
		
		if(score1 == score2)
			sprintf(str,"%d",idx);
	}
	
	CCLabelTTF *rank_label = CCLabelTTF::create(str,"fonts/rank_font.ttf",45.0);

	rank_label->setColor(ccc3(0,0,0));
	rank_label->setPosition(ccp(20,30));
	rank_label->setAnchorPoint(CCPointZero);

	CCSprite* cellBack = CCSprite::create("main_screen/table/cell.png");

	cellBack->setPosition(CCPointZero);
	cellBack->setAnchorPoint(CCPointZero);

	cell->addChild(cellBack);
	cell->addChild(rank_label);
	cell->addChild(label);
	cell->addChild(score_label);
	
	CCLabelTTF *small_label = NULL;

	if(atoi(str) == 1)
		small_label = CCLabelTTF::create("st","fonts/rank_font.ttf",30.0);
	else if(atoi(str) == 2)
		small_label = CCLabelTTF::create("nd","fonts/rank_font.ttf",30.0);
	else if(atoi(str) == 3)
		small_label = CCLabelTTF::create("rd","fonts/rank_font.ttf",30.0);
	
	if(small_label)
	{
		small_label->setPosition(ccp(80,48));
		small_label->setColor(ccc3(0,0,0));
		cell->addChild(small_label);
	}
	cell->setZOrder(ZORDER_MAIN);
	
	return cell;
}
unsigned int RankTable::numberOfCellsInTableView(CCTableView *table)
{
	return 100;
}

void RankTable::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
}

CCScene* MainState::scene()
{
	CCScene *scene = CCScene::create();
	MainState *layer = MainState::create();
	scene->addChild(layer);
	return scene;
}

bool MainState::init()
{
	if(!CCLayer::init())
		return false;
	
	Camera::sharedCameraInstance()->release();
	
	loading_icon = CCSprite::create("main_screen/table/loading.png");
	
	loading_icon->setPosition(ccp(260,200));

	CCRotateBy* rotate = CCRotateBy::create(0.8f,360.0f);

	CCRepeatForever* repeat = CCRepeatForever::create(rotate);

	loading_icon->runAction(repeat);

	CCSprite* back = CCSprite::create("background/background_1.png");
	
	CCPoint centerPos= ccp(back->getContentSize().width*0.5,back->getContentSize().height*0.5);
	back->setPosition(centerPos);

	this->addChild(back,-100);
	this->setAnchorPoint(ccp(0,getContentSize().height));

	background = new Background(this);

	CCSprite* rankTableBack = CCSprite::create("main_screen/table/ranking_box.png");

	rankTableBack->setPosition(ccp(243,getContentSize().height-277));

	this->addChild(rankTableBack,ZORDER_MAIN);

	this->addChild(loading_icon,ZORDER_MAIN);

	addUserTitle();

	CCMenuItemImage* start_button = CCMenuItemImage::create("main_screen/button/game_start.png",
		"main_screen/button/game_start.png",NULL,this,menu_selector(MainState::gameStartCallback));

	start_button->getSelectedImage()->setScale(0.9f,0.9f);

	start_button->setPosition(717,getContentSize().height-447);

	if(UserInfo::getInstance()->getIsEffectSoundOn())
	{
		sfxToggle = CCMenuItemImage::create("main_screen/button/sound_box_switch_on.png",
			"main_screen/button/sound_box_switch_on.png",NULL,this,menu_selector(MainState::sfxCallBack));
		sfxToggle->setTag(0);
	}
	else
	{
		sfxToggle = CCMenuItemImage::create("main_screen/button/sound_box_switch_off.png",
		"main_screen/button/sound_box_switch_off.png",NULL,this,menu_selector(MainState::sfxCallBack));
		sfxToggle->setTag(1);
	}

	if(UserInfo::getInstance()->getIsBackgroundSoundOn())
	{
		bgmToggle = CCMenuItemImage::create("main_screen/button/sound_box_switch_on.png",
			"main_screen/button/sound_box_switch_on.png",NULL,this,menu_selector(MainState::bgmCallBack));
		bgmToggle->setTag(0);
	}
	else
	{
		bgmToggle = CCMenuItemImage::create("main_screen/button/sound_box_switch_off.png",
		"main_screen/button/sound_box_switch_off.png",NULL,this,menu_selector(MainState::bgmCallBack));
		bgmToggle->setTag(1);
	}
	sfxToggle->setPosition(839,getContentSize().height-221);
	bgmToggle->setPosition(632,getContentSize().height-221);

	menu = Buttons::create();
	
	menu->addChild(start_button);
	menu->addChild(sfxToggle);
	menu->addChild(bgmToggle);

	menu->setMusicSrc("music/ui/ui_click.ogg");

	//menu->alignItemsHorizontally();
	
	menu->setAnchorPoint(ccp(0,getContentSize().height));
	menu->setPosition(CCPointZero);

	this->addChild(menu,ZORDER_MAIN);
	this->setKeypadEnabled(true);

	CCSprite* inputBoxSpr = CCSprite::create("main_screen/id_input/input_box.png");

	inputBoxSpr->setPosition(ccp(713,getContentSize().height-320));

	this->addChild(inputBoxSpr);
	
	CCMenuItemImage* invite_button = CCMenuItemImage::create("main_screen/button/invite_button.png",
		"main_screen/button/invite_button.png",NULL,this,menu_selector(MainState::inviteCallBack));

	invite_button->getSelectedImage()->setScale(0.9f,0.9f);

	invite_button->setPosition(ccp(640,465));
	menu->addChild(invite_button);

	CCSprite* sfxBackSpr = CCSprite::create("main_screen/sfx_box.png");
	sfxBackSpr->setPosition(ccp(615,getContentSize().height-200));
	
	this->addChild(sfxBackSpr);

	CCSprite* bgmBackSpr = CCSprite::create("main_screen/bgm_box.png");
	bgmBackSpr->setPosition(ccp(826,getContentSize().height-200));
	
	this->addChild(bgmBackSpr);

	table = new RankTable();

	this->addChild(table->getTableView(),ZORDER_MAIN);

	inputBox = UserInfo::getInstance()->createEditBox();
	inputBox->setDelegate(this);
	inputBox->setPosition(ccp(150,25));
	inputBox->setAnchorPoint(CCPointZero);

	inputBoxSpr->addChild(inputBox,ZORDER_MAIN);

	this->schedule(schedule_selector(MainState::update));
	this->schedule(schedule_selector(Timer::update));
	
	SoundManager::getInstance()->playBackgroundMusic("music/background/main_bgm_loop.ogg",true);

	return true;

}
void MainState::inviteCallBack(CCObject* pSender)
{

}
void MainState::update(float dt)
{
	background->update();
	if(RankData::getInstance()->IsLoading)
	{
		loading_icon->setVisible(true);
		table->getTableView()->setVisible(false);
	}
	else
	{
		table->getTableView()->setVisible(true);
		loading_icon->setVisible(false);
	}
	if(RankData::getInstance()->bLoadComplete)
	{
		table->getTableView()->reloadData();

		RankData::getInstance()->bLoadComplete = false;
	}
	Camera::sharedCameraInstance()->move(3.0f);
}
void MainState::addUserTitle()
{
	name_label = CCLabelTTF::create(UserInfo::getInstance()->getName().c_str(), "fonts/rank_font.ttf", 30.0);
	name_label->setColor(ccc3(0,0,0));
	name_label->setPosition(ccp(195,344));
	name_label->setAnchorPoint(CCPointZero);

	CCLabelTTF *score_label = CCLabelTTF::create(UserInfo::getInstance()->getBestScore().c_str(),"fonts/rank_font.ttf",30.0);
	score_label->setColor(ccc3(0,0,0));
	score_label->setPosition(ccp(187,371));
	score_label->setAnchorPoint(CCPointZero);

	std::string best_rank = RankData::getInstance()->getBestRank().c_str();

	CCLabelTTF *rank_label = CCLabelTTF::create(best_rank.c_str(),"fonts/rank_font.ttf",45.0);

	rank_label->setColor(ccc3(0,0,0));
	rank_label->setPosition(ccp(20,30));

	this->addChild(rank_label,ZORDER_MAIN);
	this->addChild(name_label,ZORDER_MAIN);
	this->addChild(score_label,ZORDER_MAIN);
	
	CCLabelTTF *small_label = NULL;

	if(!best_rank.compare("1"))
		small_label = CCLabelTTF::create("st","fonts/rank_font.ttf",30.0);
	else if(!best_rank.compare("2"))
		small_label = CCLabelTTF::create("nd","fonts/rank_font.ttf",30.0);
	else if(!best_rank.compare("3"))
		small_label = CCLabelTTF::create("rd","fonts/rank_font.ttf",30.0);
	
	if(small_label)
	{
		small_label->setPosition(ccp(60,48));
		small_label->setColor(ccc3(0,0,0));
		this->addChild(small_label,ZORDER_MAIN);
	}
}
void MainState::sfxCallBack(CCObject* pSender)
{
	if(sfxToggle->getTag() == 0)
	{
		sfxToggle->setNormalImage(CCSprite::create("main_screen/button/sound_box_switch_off.png"));
		sfxToggle->setSelectedImage(CCSprite::create("main_screen/button/sound_box_switch_off.png"));

		sfxToggle->setTag(1);
		UserInfo::getInstance()->setEffectSound(false);
	}
	else
	{
		sfxToggle->setNormalImage(CCSprite::create("main_screen/button/sound_box_switch_on.png"));
		sfxToggle->setSelectedImage(CCSprite::create("main_screen/button/sound_box_switch_on.png"));
		
		sfxToggle->setTag(0);
		UserInfo::getInstance()->setEffectSound(true);
	}
}
void MainState::bgmCallBack(CCObject* pSender)
{
	if(bgmToggle->getTag() == 0)
	{
		bgmToggle->setNormalImage(CCSprite::create("main_screen/button/sound_box_switch_off.png"));
		bgmToggle->setSelectedImage(CCSprite::create("main_screen/button/sound_box_switch_off.png"));

		bgmToggle->setTag(1);
		UserInfo::getInstance()->setBackgroundSound(false);
	}
	else
	{
		bgmToggle->setNormalImage(CCSprite::create("main_screen/button/sound_box_switch_on.png"));
		bgmToggle->setSelectedImage(CCSprite::create("main_screen/button/sound_box_switch_on.png"));
		
		bgmToggle->setTag(0);
		
		UserInfo::getInstance()->setBackgroundSound(true);
	}
}
void MainState::editBoxEditingDidEnd(CCEditBox* editBox)
{
	if(!strcmp(editBox->getText(),""))
	{
		CCMessageBox("아이디를 입력해주세요","아이디 입력");
		return;
	}
	UserInfo::getInstance()->registerPreferenceName(editBox->getText());
	name_label->setString(editBox->getText());
	editBox->setText("");

	RankData::getInstance()->loadRank(UserInfo::getInstance()->getName(),UserInfo::getInstance()->getUUID(),"");
}
void MainState::keyBackClicked()
{

}
void MainState::inputID()
{
}
void MainState::onExit()
{
	/*
	CCDirector::sharedDirector()->end();
	SimpleAudioEngine::sharedEngine()->end();
	CCDirector::sharedDirector()->end();
	UserInfo::getInstance()->end();
	ScoreManager::sharedInstance()->release();
	*/
	CCLayer::onExit();
	delete background;
}
void MainState::gameStartCallback(CCObject* pSender)
{
	if(UserInfo::getInstance()->getTutorialCheck())
		CCDirector::sharedDirector()->replaceScene(TutorialChoiceState::scene());
	else
		CCDirector::sharedDirector()->replaceScene(GameState::scene());
}