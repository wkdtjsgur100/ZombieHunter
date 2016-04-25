#include "IntroState.h"

CCScene* IntroState::scene()
{
	CCScene *scene = CCScene::create();
	IntroState *layer = IntroState::create();
	scene->addChild(layer);
	return scene;
}

bool IntroState::init()
{
	if(!CCLayer::init())
		return false;

	this->numberOfSprites = 76;
	this->loadedNumberOfSprites = 0;

	introImage = CCSprite::create("intro_screen/main_back.png"); 

	introImage->setPosition(ccp(getContentSize().width/2 , getContentSize().height/2));
	
	this->addChild(introImage);

	CCSize size = CCDirector::sharedDirector()->getWinSize();

    m_pLabelLoading = CCLabelTTF::create("Loading...", "fonts/rank_font.ttf",50);
    m_pLabelPercent = CCLabelTTF::create("%0", "fonts/rank_font.ttf", 50);

	//m_pLabelLoading->setColor(ccc3(0,0,0));
	//m_pLabelPercent->setColor(ccc3(0,0,0));

    m_pLabelLoading->setPosition(ccp(721,106));
    m_pLabelPercent->setPosition(ccp(721,46));

    this->addChild(m_pLabelLoading);
    this->addChild(m_pLabelPercent);

	//////////////Sound
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("music/background/game_bgm_loop.ogg");
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("music/background/main_bgm_loop.ogg");
	SimpleAudioEngine::sharedEngine()->preloadEffect("music/zombie/mon_zombi_attack.ogg");
	SimpleAudioEngine::sharedEngine()->preloadEffect("music/zombie/mon_zombi_attack_voice.ogg");
	SimpleAudioEngine::sharedEngine()->preloadEffect("music/zombie/mon_zombi_damage.ogg");
	SimpleAudioEngine::sharedEngine()->preloadEffect("music/zombie/mon_zombi_damage_voice.ogg");
	SimpleAudioEngine::sharedEngine()->preloadEffect("music/zombie/mon_zombi_death.ogg");
	SimpleAudioEngine::sharedEngine()->preloadEffect("music/zombie/mon_zombi_death_voice.ogg");
	SimpleAudioEngine::sharedEngine()->preloadEffect("music/weapon/wpn_firegun_loop.ogg");
	SimpleAudioEngine::sharedEngine()->preloadEffect("music/weapon/wpn_handgun.ogg");
	SimpleAudioEngine::sharedEngine()->preloadEffect("music/weapon/wpn_machinegun.ogg");
	SimpleAudioEngine::sharedEngine()->preloadEffect("music/weapon/wpn_rifle.ogg");
	SimpleAudioEngine::sharedEngine()->preloadEffect("music/ui/ui_click.ogg");
	SimpleAudioEngine::sharedEngine()->preloadEffect("music/ui/ui_reload.ogg");
	SimpleAudioEngine::sharedEngine()->preloadEffect("music/ui/ui_weapon_change.ogg");

	/////arch - 6
	CCTextureCache::sharedTextureCache()->addImageAsync("architecture/drum01.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("architecture/drum02.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("architecture/pan01.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("architecture/pan02.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("architecture/poly01.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("architecture/poly02.png", this, callfuncO_selector(IntroState::loadingCallback));
	
	//background
	CCTextureCache::sharedTextureCache()->addImageAsync("background/01/bush01.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/01/bush02.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/01/bush03.png", this, callfuncO_selector(IntroState::loadingCallback));

	CCTextureCache::sharedTextureCache()->addImageAsync("background/02/back11.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/02/back12.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/02/back13.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/02/back14.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/02/back15.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/02/back16.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/02/back17.png", this, callfuncO_selector(IntroState::loadingCallback));

	CCTextureCache::sharedTextureCache()->addImageAsync("background/03/wall01.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/03/wall02.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/03/wall03.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/03/wall04.png", this, callfuncO_selector(IntroState::loadingCallback));

	CCTextureCache::sharedTextureCache()->addImageAsync("background/04/util01.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/04/util02-0.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/04/util02-1.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/04/util02-2.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/04/util02-3.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/04/util03.png", this, callfuncO_selector(IntroState::loadingCallback));

	CCTextureCache::sharedTextureCache()->addImageAsync("background/05/back02.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/05/back03.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/05/back04.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/05/back05.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/05/back06.png", this, callfuncO_selector(IntroState::loadingCallback));

	CCTextureCache::sharedTextureCache()->addImageAsync("background/06/back07.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/06/back08.png", this, callfuncO_selector(IntroState::loadingCallback));

	CCTextureCache::sharedTextureCache()->addImageAsync("background/07/back09.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/07/back10.png", this, callfuncO_selector(IntroState::loadingCallback));

	CCTextureCache::sharedTextureCache()->addImageAsync("background/cloud/cloud.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/cloud/cloud02.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/cloud/cloud03.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/cloud/cloud04.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/cloud/cloud05.png", this, callfuncO_selector(IntroState::loadingCallback));
	
	CCTextureCache::sharedTextureCache()->addImageAsync("background/particle/star01.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/particle/star02.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("background/particle/star03.png", this, callfuncO_selector(IntroState::loadingCallback));

	CCTextureCache::sharedTextureCache()->addImageAsync("background/background_1.png", this, callfuncO_selector(IntroState::loadingCallback));

	//////////////////////////////////fonts

	
	////////////////////////////////ground
	CCTextureCache::sharedTextureCache()->addImageAsync("ground/base_ground_1.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("ground/base_ground_2.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("ground/ground_1.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("ground/ground_2.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("ground/ground_3.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("ground/ground_4.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("ground/ground_5.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("ground/shop.png", this, callfuncO_selector(IntroState::loadingCallback));
	
	////////////////////////////////main_screen
	CCTextureCache::sharedTextureCache()->addImageAsync("main_screen/main_back.png", this, callfuncO_selector(IntroState::loadingCallback));

	CCTextureCache::sharedTextureCache()->addImageAsync("main_screen/button/game_start.png", this, callfuncO_selector(IntroState::loadingCallback));
	//////////////////////////pause

	CCTextureCache::sharedTextureCache()->addImageAsync("pause/pause_background.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("pause/resume.png", this, callfuncO_selector(IntroState::loadingCallback));

	CCTextureCache::sharedTextureCache()->addImageAsync("player/walk.png", this, callfuncO_selector(IntroState::loadingCallback));

	////////////////////////UI

	CCTextureCache::sharedTextureCache()->addImageAsync("UI/hp_bar.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("UI/move_left_normal.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("UI/move_right_normal.png", this, callfuncO_selector(IntroState::loadingCallback));
	
	CCTextureCache::sharedTextureCache()->addImageAsync("UI/pause.png", this, callfuncO_selector(IntroState::loadingCallback));
	
	CCTextureCache::sharedTextureCache()->addImageAsync("UI/reload_call.png", this, callfuncO_selector(IntroState::loadingCallback));

	/////////////////////weapon
	CCTextureCache::sharedTextureCache()->addImageAsync("weapon/bullet/bullet_1.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("weapon/bullet/fire.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("weapon/bullet/stone.png", this, callfuncO_selector(IntroState::loadingCallback));

	CCTextureCache::sharedTextureCache()->addImageAsync("weapon/select_weapon/weapon1.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("weapon/select_weapon/weapon2_1.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("weapon/select_weapon/weapon3.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("weapon/select_weapon/weapon4.png", this, callfuncO_selector(IntroState::loadingCallback));
	
	/////////////////////zombie
	CCTextureCache::sharedTextureCache()->addImageAsync("zombie_fast/attacking_anim.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("zombie_fast/walking_anim.png", this, callfuncO_selector(IntroState::loadingCallback));

	CCTextureCache::sharedTextureCache()->addImageAsync("zombie_huge/walk.png", this, callfuncO_selector(IntroState::loadingCallback));

	CCTextureCache::sharedTextureCache()->addImageAsync("zombie_normal/attacking_anim.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("zombie_normal/dead_anim.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("zombie_normal/walking_anim.png", this, callfuncO_selector(IntroState::loadingCallback));
	CCTextureCache::sharedTextureCache()->addImageAsync("zombie_normal/head.png", this, callfuncO_selector(IntroState::loadingCallback));
	
	return true;
}

void IntroState::loadingCallback(CCObject* pSender)
{
	++loadedNumberOfSprites;
    char tmp[10];
    sprintf(tmp,"%%%d", (int)(((float)loadedNumberOfSprites / numberOfSprites) * 100));
    m_pLabelPercent->setString(tmp);

    if (loadedNumberOfSprites == numberOfSprites)
    {
		this->removeChild(introImage,true);
        this->removeChild(m_pLabelLoading, true);
        this->removeChild(m_pLabelPercent, true);
		CCDirector::sharedDirector()->replaceScene(MainState::scene());
    }
}