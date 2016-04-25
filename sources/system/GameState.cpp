#include "GameState.h"
#include "MainState.h"

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

#ifdef __cplusplus
extern "C"{
#endif
	void Java_good_games_mygame_GoodGame_pauseAll(JNIEnv* env,jobject obj)
	{
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		SimpleAudioEngine::sharedEngine()->pauseAllEffects();

		PauseState::getInstance()->pause();
	}
	void Java_good_games_mygame_GoodGame_resumeAll(JNIEnv* env,jobject obj)
	{
		SimpleAudioEngine::sharedEngine()->resumeAllEffects();
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}
#ifdef __cplusplus
}
#endif

#endif

CCScene* GameState::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameState *layer = GameState::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameState::init()
{
   //////////////////////////////
	// 1. super init first
	///LayerColor->Layer로 바꿔야됨. 성능차이 많이나는듯
	
	if ( !CCLayer::init() )
	{
		return false;
	}
	Camera::sharedCameraInstance()->release();
	
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object

	// create menu, it's an autorelease object

	this->setTouchEnabled(true);

	for(int i=0;i<2;i++)
	{
		CCMenuItemImage* buttonLeftImg = CCMenuItemImage::create("UI/move_left_normal.png","UI/move_left_normal.png");
		CCMenuItemImage* buttonRightImg = CCMenuItemImage::create("UI/move_right_normal.png","UI/move_right_normal.png");

		CCSprite *pressImg = CCSprite::create("UI/move_left_normal.png");
		pressImg->setOpacity(100);
		buttonLeftImg->setSelectedImage(pressImg);

		CCSprite *pressRightImg = CCSprite::create("UI/move_right_normal.png");
		pressRightImg->setOpacity(100);
		buttonRightImg->setSelectedImage(pressRightImg);

		if(i == 0)	{
			buttonLeftImg->setPosition(ccp(60,60));
			buttonRightImg->setPosition(ccp(160,60));
		}
		else	{
			buttonLeftImg->setPosition(ccp(800,60));
			buttonRightImg->setPosition(ccp(900,60));
		}
		buttonLeftImg->setTag(LEFT_BUTTON);
		buttonRightImg->setTag(RIGHT_BUTTON);

		buttons[i] = Buttons::create();

		buttons[i]->setMusicSrc("music/ui/ui_click.ogg");

		buttons[i]->addChild(buttonLeftImg);
		buttons[i]->addChild(buttonRightImg);

		buttons[i]->setPosition(CCPointZero);

		this->addChild(buttons[i],ZORDER_BUTTON);
	}

	CCMenuItemFont* pMenuItem1 = CCMenuItemFont::create("Http Post",this,menu_selector(GameState::sendPostHttp));
	CCMenuItemFont* pMenuItem2 = CCMenuItemFont::create("Http Get",this,menu_selector(GameState::sendGetHttp));

	pMenuItem2->setColor(ccc3(255,255,255));
	pMenuItem1->setColor(ccc3(255,255,255));
	
	CCMenu* pMenu = CCMenu::create(pMenuItem1,pMenuItem2,NULL);
	pMenu->setPosition(240,160);
	pMenu->alignItemsVertically();
	//this->addChild(pMenu);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);

	// position the label on the center of the screen
	pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
		origin.y + visibleSize.height - pLabel->getContentSize().height));

	// add the label as a child to this layer
	//this->addChild(pLabel, 1);

	// add "GameState" splash screen"
	CCSprite* pSprite = CCSprite::create("background/background_1.png");

	// position the sprite on the center of the screen
	pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	// add the sprite as a child to this layer
#ifndef BODY_DEBUG
	this->addChild(pSprite, -20);
#endif
	//pChracImg = CCSprite::create("chracter/move_ani.png");

	//box2d///

	b2Vec2 gravity = b2Vec2(0.0f,-30.0f);
	_world = new b2World(gravity);
	_world->SetAllowSleeping(true);
	_world->SetContinuousPhysics(true);

	
	contactListener = new MyContactListener();
	_world->SetContactListener((b2ContactListener*)contactListener);

	b2BodyDef groundBodyDef;

	groundBodyDef.position.Set(0,0);

	b2Body *groundBody = _world->CreateBody(&groundBodyDef);

	b2EdgeShape groundEdge;
	b2FixtureDef boxShapeDef;
	boxShapeDef.shape = &groundEdge;
	
	CCSize s = this->getContentSize();
	//////////////////////////////////////// Debug
#ifdef BODY_DEBUG
	b2Draw *debugDraw = new GLESDebugDraw(PTM_RATIO); //PTM_RATIO 를 1~3 정도로 작게 두니까 맞던데?

	debugDraw->SetFlags(GLESDebugDraw::e_shapeBit);

	_world->SetDebugDraw(debugDraw);
#endif

	//doSnow();

	pressing = false;

	//doRain();

	////////////////////////pause
	pauseButton = Buttons::create();

	CCMenuItemImage* pauseImage = CCMenuItemImage::create("UI/pause.png","UI/pause.png",NULL,this,
		menu_selector(GameState::pauseGame));

	pauseImage->getSelectedImage()->setScale(0.9f,0.9f);

	CCSize pauseSize = pauseImage->getContentSize();

	pauseButton->addChild(pauseImage);

	pauseButton->setPosition(ccp(getContentSize().width-pauseSize.width*0.5,getContentSize().height-pauseSize.height*0.5));

	this->addChild(pauseButton,ZORDER_INTERFACE);

	//////////////////////////////Stage

	stageManager = new StageManager(_world,this);

	//////////////Sound
	SoundManager::getInstance()->playBackgroundMusic("music/background/game_bgm_loop.ogg",true);
	SoundManager::getInstance()->setBackgroundMusicVolume(0.3f);

	PauseState::getInstance()->setResumeTarget(this);
	EffectManager::getInstance()->setTarget(this);

	//fade
	fade = CCSprite::create("over/fade.png");
	fade->setOpacity(0);
	fade->setPosition(ccp(visibleSize.width/2,visibleSize.height/2));
	fade->retain();

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	this->setKeypadEnabled(true);					//키패드 활성.
#endif

	userInterface = new UserInterface(this);


	doRain();

	isPressReload = false;

	tutorialManager = new TutorialManager(this);
	tutorialManager->setCurrentPage(100);

	this->schedule(schedule_selector(GameState::update));
	this->schedule(schedule_selector(Timer::update));
	this->schedule(schedule_selector(GameState::box2DTest));

	stageManager->stageUp();

	ScoreManager::sharedInstance()->moveDistance = 0;
	ScoreManager::sharedInstance()->point = 0;

	return true;
}

void GameState::keyBackClicked()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	PauseState::getInstance()->pause();		
#endif
}
void GameState::sendGetHttp(CCObject* sender)
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl("http://deploydjango1.herokuapp.com/?text=32");

	request->setRequestType(CCHttpRequest::kHttpGet);

	CCHttpClient::getInstance()->send(request); //보내고 나서

	request->setResponseCallback(this,callfuncND_selector(GameState::onHttpRequestCompleted));//받습니다

	request->release();
}
void GameState::pauseGame(CCObject* pSender)
{
	PauseState::getInstance()->pause();
}
void GameState::sendPostHttp(CCObject* sender)
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl("http://deploydjango1.herokuapp.com");

	request->setRequestType(CCHttpRequest::kHttpPost);
	std::vector<std::string> headers;
	
	headers.push_back("Content-Type: application/json; charset=utf-8");
	request->setHeaders(headers);
	
	const char* postData = "text=12";
	request->setRequestData(postData,strlen(postData));
	request->setTag("text");

	CCHttpClient::getInstance()->send(request); //보내고 나서
	request->setResponseCallback(this,callfuncND_selector(GameState::onHttpRequestCompleted));//받습니다
	
	request->release();
}
void GameState::onHttpRequestCompleted(CCNode* sender,void *data)
{
	CCHttpResponse *response = (CCHttpResponse*)data;
	if(!response)
		return;

	if( NULL != strlen(response->getHttpRequest()->getTag()))
	{
		CCLog("%s complete.",response->getHttpRequest()->getTag());
	}
	int statusCode = response->getResponseCode();
	CCLog("response code : %d",statusCode);

	if(!response->isSucceed())
	{
		CCLog("response failed");
		CCLog("errorbuffer: %s",response->getErrorBuffer());
		return;
	}
	std::vector<char>* buffer = response->getResponseData();

	char str[200] = {""};
	unsigned int i;

	for(i=0;i<buffer->size();i++)
	{
		str[i] = (*buffer)[i];
	}
	str[i] = '\0';
	CCLog("%s",str);
}
void GameState::doRain()
{
	m_emitter = CCParticleRain::create();
	m_emitter->retain();
	CCPoint p = m_emitter->getPosition();
	m_emitter->setTotalParticles(1000);
	m_emitter->setPosition(ccp(p.x,p.y-100));
	m_emitter->setLife(4);
	m_emitter->setSpeed(400);
	m_emitter->setSpeedVar(100);

	m_emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle/snow.png"));
	if(m_emitter != NULL)
	{
		CCSize s = CCDirector::sharedDirector()->getWinSize();
		m_emitter->setPosition(ccp(s.width/2,s.height));
		this->addChild(m_emitter,ZORDER_INTERFACE-1);
	}
}
void GameState::doSnow()
{
	//snow Particle
	m_emitter = CCParticleSnow::create();
	m_emitter->retain();

	CCPoint p = m_emitter->getPosition();
	m_emitter->setTotalParticles(800);
	m_emitter->setPosition(ccp(p.x,p.y-110));
	m_emitter->setLife(5);
	m_emitter->setLifeVar(1);

	m_emitter->setGravity(ccp(0,-10));
	//m_emitter->setScale(3.0);
	m_emitter->setSpeed(100);
	m_emitter->setSpeedVar(30);

	ccColor4F startColor = m_emitter->getStartColor();
	startColor.r = 0.9f;
	startColor.g = 0.9f;
	startColor.b = 0.9f;

	m_emitter->setStartColor(startColor);

	ccColor4F startColorVar = m_emitter->getStartColorVar();
	startColorVar.b = 0.1f;
	m_emitter->setStartColorVar(startColorVar);
	m_emitter->setEmissionRate(m_emitter->getTotalParticles()/m_emitter->getLife());

	m_emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle/snow.png"));
	

	if(m_emitter != NULL)
	{
		CCSize s = CCDirector::sharedDirector()->getWinSize();
		m_emitter->setPosition(ccp(s.width/2,s.height));
		this->addChild(m_emitter);
	}
}
void GameState::box2DTest(float dt)
{

	//물리세계를 시뮬레이션한다.
	_world->Step(dt,8,3);
}
void GameState::addNewSpriteInWorld(CCPoint location)
{
	int tW = texture->getContentSize().width;
	int tH = texture->getContentSize().height;
	//CCSprite *spr = CCSprite::createWithTexture(texture,CCRectMake(0,0,tW,tH));
	//spr->setPosition(ccp(location.x,location.y));
	//this->addChild(spr,2);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(location.x/PTM_RATIO,location.y/PTM_RATIO);
	//bodyDef.userData = spr;
	b2Body* body = _world->CreateBody(&bodyDef);
	
	b2CircleShape circle;
	circle.m_radius = 0.55;
	
	b2FixtureDef fixtureDef;
	
	//모양지정
	fixtureDef.shape = &circle;
	
	//밀도
	fixtureDef.density = 1.0f;

	//마찰력
	fixtureDef.friction = 0.2f;

	//반발력
	fixtureDef.restitution = 0.3f;
	
	body->CreateFixture(&fixtureDef);
}
void GameState::draw()
{
	CCLayer::draw();
	_world->DrawDebugData();
}
void GameState::onExit()
{
	this->removeChild(pChracImg);
	
	delete stageManager;
	delete contactListener;
	//weaponManager->release();
	delete _world;
	
	/*SimpleAudioEngine::sharedEngine()->end();
	CCDirector::sharedDirector()->end();
	PauseState::getInstance()->end();
	//UserInfo::getInstance()->end();
	
	ScoreManager::sharedInstance()->release();
	*/
	//ScoreManager::sharedInstance()->resetScore();
	//PauseState::getInstance()->end();
	EffectManager::getInstance()->release();

	fade->release();
	m_emitter->release();
	delete userInterface;
	
	CCLayer::onExit();
}
void GameState::update(float dt)
{
	if(stageManager->getPlayer()->isDead())
	{
		if(tutorialManager->getCurrentPage() >= 100)
		{
			//화면 점점 검게 만들다가, 끝나면 게임오버화면으로 넘어가면됨.
			if(this->getChildByTag(TAG_FADE)==NULL)
			{
				fade->setTag(TAG_FADE);
				this->addChild(fade,ZORDER_FADE);
			}

			float plusDt = 400.0f*Timer::getDt();

			if(fade->getOpacity()>=255) CCDirector::sharedDirector()->replaceScene(GameOverState::scene());

			if(fade->getOpacity()+plusDt>=255)
			{
				fade->setOpacity(255);
			}
			else
				fade->setOpacity(fade->getOpacity()+plusDt);
		}
		//return;
	}
	int speed = 10;
	stageManager->getArchMng()->updateAll(stageManager->getPlayer()->getPosition());
	stageManager->getShop()->setInitHeight(stageManager->getGroundMng()->getYGroundPos(getContentSize().width+85.0f)-10.0f);

	if(tutorialManager->getCurrentPage() >= 5 && tutorialManager->getCurrentPage() != 11 && tutorialManager->getCurrentPage() != 17)
	{
		if(pressing)
		{
			if(touchPoint.y > stageManager->getGroundMng()->getBaseGroundHeight()- BASEGROUND_DELTA)
				stageManager->getPlayer()->fire(b2Vec2(touchPoint.x/PTM_RATIO,touchPoint.y/PTM_RATIO));
		}
	}
	Character::direction_enum pd = Character::stop;

	for(int i=0;i<2;i++)
	{
		if(buttons[i]->isSelected(RIGHT_BUTTON))
			pd = Character::right;
		if(buttons[i]->isSelected(LEFT_BUTTON))
			pd = Character::left;
	}
	stageManager->getPlayer()->changeDirection(pd);
	stageManager->getPlayer()->update(dt,stageManager->getGroundMng());

	stageManager->getZombieManager()->walkAlongGround(stageManager->getGroundMng());

	CCPoint genPos;
	genPos.x = getContentSize().width - 50.0f;
	genPos.y = stageManager->getGroundMng()->getYGroundPos(genPos.x);

	stageManager->getZombieManager()->autoGenerate(genPos);

	if( tutorialManager->getCurrentPage() != 2 && tutorialManager->getCurrentPage() != 11 && tutorialManager->getCurrentPage() != 15 &&
		tutorialManager->getCurrentPage() != 7 && tutorialManager->getCurrentPage() != 17)
		stageManager->getZombieManager()->update(dt,stageManager->getPlayer()->getPosition());
	
	for(b2Body* b=_world->GetBodyList(); b; b = b->GetNext())
	{
		UserDataInBody* ud = (UserDataInBody*)(b->GetUserData());
		if(ud!=NULL){
			if(ud->mSprite!=NULL)
			{
				if(!(ud->mType == TYPE_GROUND))
				{
					ud->mSprite->setPosition(ccp(b->GetPosition().x * PTM_RATIO , b->GetPosition().y*PTM_RATIO));
					ud->mSprite->setRotation(-1*CC_RADIANS_TO_DEGREES(b->GetAngle()));
				}
				if(ud->mType == TYPE_ZOMBIE || ud->mType == TYPE_HUGE_ZOMBIE)
				{
					ud->mSprite->setRotation(-1*CC_RADIANS_TO_DEGREES(b->GetAngle()));
				}
			}
		}
		else // ud를 안쓰는 경우. 안쓰는 경우가 없도록 할것이므로 추후 삭제
		{
			if(b->GetUserData()!=NULL)
			{
				(  (CCSprite*)(  b->GetUserData()  )  )->setPosition(ccp(b->GetPosition().x * PTM_RATIO , b->GetPosition().y*PTM_RATIO));
				(  (CCSprite*)(  b->GetUserData()  )  )->setRotation(-1*CC_RADIANS_TO_DEGREES(b->GetAngle()));
			}
		}
	}
	///////Collision//////
	for(int i=0;i<contactListener->contacts.size();i++)
	{
		MyContact contact = contactListener->contacts[i];
		
		b2Body* bodyA = contact.fixtureA->GetBody();
		b2Body* bodyB = contact.fixtureB->GetBody();

		UserDataInBody* usrDataA = (UserDataInBody*)bodyA->GetUserData();
		UserDataInBody* usrDataB = (UserDataInBody*)bodyB->GetUserData();

		if(usrDataA != NULL && usrDataB != NULL)
		{	
			if((usrDataB->mType == TYPE_GROUND && usrDataA->mType == TYPE_BULLET) ||
				(usrDataB->mType == TYPE_BULLET && usrDataA->mType == TYPE_GROUND))
			{
				Bullet* bullet;
				if(usrDataA->mType == TYPE_BULLET)
					bullet = (Bullet*)usrDataA->mChr;
				else
					bullet = (Bullet*)usrDataB->mChr;

				bullet->isDead = true;
			}
			if(usrDataA->mType == TYPE_ARCHITECT && usrDataB->mType == TYPE_BULLET)
			{
				Architecture* arch = (Architecture*)usrDataA->mChr;
				Bullet* bullet = (Bullet*)usrDataB->mChr;

				arch->attack(bullet->damage);
				if(arch->getHP() <= 0)
				{
					arch->killZombieOnArchitecture();

					//b2Vec2 force = b2Vec2(bullet->bv.x*10000,bullet->bv.y*10000);
					b2Vec2 force = bullet->vCurrentDir;
					force *= 100;

					b2Vec2 point = bodyB->GetPosition();

					point *= PTM_RATIO;

					bodyA->ApplyForce(force,point);
				}

				bullet->isDead = true;
			}
			else if(usrDataA->mType == TYPE_BULLET && usrDataB->mType == TYPE_ARCHITECT)
			{
				Architecture* arch = (Architecture*)usrDataB->mChr;
				Bullet* bullet = (Bullet*)usrDataA->mChr;

				arch->attack(bullet->damage);
				
				if(arch->getHP() <= 0)
				{
					arch->killZombieOnArchitecture();

					b2Vec2 force = bullet->vCurrentDir;
					force *= 100;

					b2Vec2 point = bodyA->GetPosition();

					point *= PTM_RATIO;

					bodyB->ApplyForce(force,point);
				}
				//b2Vec2 force = b2Vec2(bullet->bv.x*10000,bullet->bv.y*10000);
				//arch->applyForceToCenter(force);
				bullet->isDead = true;
			}
			if(usrDataA->mType == TYPE_BULLET && usrDataB->mType==TYPE_ZOMBIE)
			{
				Zombie* z = (Zombie*)(usrDataB->mChr);
				Bullet* b = (Bullet*)(usrDataA->mChr);

				if(z!=NULL && z->currentState!=Zombie::dead && z->currentState!=Zombie::dying) //죽은건 호출 안함
				{
					if(b->find(z)) return;
					
					bool isHeadAttacked=false;
					if(!z->isHeadLess && bodyB == z->head)
						isHeadAttacked = true;

					b2Vec2 force = b->vCurrentDir;
					force *= 500;
					stageManager->getZombieManager()->attackZombie(z,b->damage,b->headshot_damage,isHeadAttacked,force); //총알 뎀지만큼 피해
					b->pierceList.push_back(z);
					b->minusPiercing();
				}
				//player->weaponManager->removeBullet((Bullet*)(usrDataA->mChr));//뷸렛도 죽여야지
			}
			else if(usrDataA->mType ==TYPE_ZOMBIE && usrDataB->mType ==TYPE_BULLET) //A와 B가 바뀌는 경우도 있나봄.. 대비용. 확실치는 않음.
			{
				Zombie* z = (Zombie*)(usrDataA->mChr);
				Bullet* b = (Bullet*)(usrDataB->mChr);

				if(z!=NULL && z->currentState!=Zombie::dead&& z->currentState!=Zombie::dying)
				{
					if(b->find(z)) return;

					bool isHeadAttacked=false;
					if(!z->isHeadLess && bodyA == z->head)
						isHeadAttacked = true;

					b2Vec2 force = b->vCurrentDir;
					force *= 500;
					stageManager->getZombieManager()->attackZombie(z,b->damage,b->headshot_damage,isHeadAttacked,force); //총알 뎀지만큼 피해
					b->pierceList.push_back(z);
					b->minusPiercing();
				}
					//player->weaponManager->removeBullet((Bullet*)(udB->mChr));
			}
			if(usrDataA->mType ==TYPE_PLAYER && usrDataB->mType ==TYPE_ZOMBIEBULLET)
			{
				AttackingObject* b = (AttackingObject*)(usrDataB->mChr);
				Player* p = (Player*)(usrDataA->mChr);

				if(b->find(p))return;

				p->damaged(b->damage);
				b->pierceList.push_back(p);
				b->isDead=true;

			}
			else if(usrDataA->mType ==TYPE_ZOMBIEBULLET&& usrDataB->mType ==TYPE_PLAYER)
			{
				AttackingObject* b = (AttackingObject*)(usrDataA->mChr);
				Player* p = (Player*)(usrDataB->mChr);

				if(b->find(p))return;

				p->damaged(b->damage);
				b->pierceList.push_back(p);
				b->isDead=true;
			}
			if(usrDataA->mType ==TYPE_ZOMBIEBULLET&& usrDataB->mType ==TYPE_BULLET)
			{
				CCLOG("collision zb&b");
				AttackingObject* zb = (AttackingObject*)(usrDataA->mChr);
				Bullet* b = (Bullet*)(usrDataB->mChr);

				if(b->find(zb))return;

				zb->isDead=true;
				b->minusPiercing();
				b->pierceList.push_back(zb);

			}
			else if(usrDataA->mType ==TYPE_BULLET&& usrDataB->mType ==TYPE_ZOMBIEBULLET)
			{
				AttackingObject* zb = (AttackingObject*)(usrDataB->mChr);
				Bullet* b = (Bullet*)(usrDataA->mChr);

				if(b->find(zb))return;

				zb->isDead=true;
				b->minusPiercing();
				b->pierceList.push_back(zb);
			}
	
			if(usrDataA->mType == TYPE_ZOMBIE_ATTACK && usrDataB->mType==TYPE_PLAYER)
			{
				if(tutorialManager->getCurrentPage() >= 100)
				{
					Zombie* z = (Zombie*)(usrDataA->mChr);
					stageManager->getPlayer()->damaged(z->damage);
					z->isAlreadyAttack = true;
				}
			}

			else if(usrDataB->mType == TYPE_ZOMBIE_ATTACK && usrDataA->mType==TYPE_PLAYER)
			{
				if(tutorialManager->getCurrentPage() >= 100)
				{
					Zombie* z = (Zombie*)(usrDataB->mChr);
					stageManager->getPlayer()->damaged(z->damage);
					z->isAlreadyAttack = true;
				}
			} 
			if(usrDataA->mType == TYPE_ZOMBIE_SKILL && usrDataB->mType==TYPE_PLAYER)
			{
				Zombie* z = (Zombie*)(usrDataA->mChr);
				if(z->typeZ == Zombie::flying)
				{
					if(stageManager->getPlayer()->currentState == Player::binded)return;

					FlyingZombie* fz = (FlyingZombie*)z;
					stageManager->getPlayer()->bind(fz->bindingTime,fz->bindingDamage);
					fz->skillBody->SetActive(false);
					fz->changeState(Zombie::dying);
				}
			
			}
			else if(usrDataB->mType == TYPE_ZOMBIE_SKILL && usrDataA->mType==TYPE_PLAYER)
			{
				Zombie* z = (Zombie*)(usrDataB->mChr);
				if(z->typeZ == Zombie::flying)
				{
					if(stageManager->getPlayer()->currentState == Player::binded)return;
					FlyingZombie* fz = (FlyingZombie*)z;
					stageManager->getPlayer()->bind(fz->bindingTime,fz->bindingDamage);
					fz->skillBody->SetActive(false);
					fz->changeState(Zombie::dying);
				}
			
			} 
			if(usrDataA->mType == TYPE_FALLING_ZOMBIE ||
				usrDataB->mType == TYPE_FALLING_ZOMBIE)
			{
				Zombie* z;
				if(usrDataA->mType == TYPE_FALLING_ZOMBIE)
					z = (Zombie*)(usrDataA->mChr);
				else if(usrDataB->mType == TYPE_FALLING_ZOMBIE)
					z = (Zombie*)(usrDataB->mChr);

				z->die(false,b2Vec2(0,0));
			}
		}
	}
	stageManager->update(dt);

	userInterface->hpRate = (float)stageManager->getPlayer()->hp/stageManager->getPlayer()->maxHp;
	userInterface->update(dt);

	EffectManager::getInstance()->update();
}
void GameState::pauseSchedulerAndActions()
{
	CCLayer::pauseSchedulerAndActions();
	for(int i=0;i<2;i++)
		buttons[i]->unSelected();
	stageManager->getPlayer()->weaponManager->weaponButtons->unSelected();
}
void GameState::setInterfaceEnabled(bool enable)
{
	this->pauseButton->setTouchEnabled(enable);
	this->stageManager->getPlayer()->weaponManager->weaponButtons->setTouchEnabled(enable);
	
	for(int i=0;i<2;i++) this->buttons[i]->setTouchEnabled(enable);
}
void GameState::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch* touch = (CCTouch*)( pTouches->anyObject() );
	CCPoint location = touch->getLocation();

	touchPoint = location;
	pressing = true;

	if(stageManager->getPlayer()->weaponManager->containPointWithReloadSpr(touchPoint))
	{
		isPressReload = true;
		SoundManager::getInstance()->playEffect("music/ui/ui_reload.ogg");
		stageManager->getPlayer()->weaponManager->pressingReloadSpr();
	}
	if(tutorialManager->getCurrentPage() == 5)
	{
		this->resumeSchedulerAndActions();
		tutorialManager->increasePage();
	}
}
void GameState::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);
	CCPoint location = touch->getLocation();

	this->touchPoint = location;
	if(isPressReload &&
		stageManager->getPlayer()->weaponManager->containPointWithReloadSpr(touchPoint))
	{
		stageManager->getPlayer()->weaponManager->pressingReloadSpr();
	}
	else
		stageManager->getPlayer()->weaponManager->nonPressingReloadSpr();
}
void GameState::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);
	CCPoint location = touch->getLocation();

	this->touchPoint = location;

	pressing = false;

	if(isPressReload)
	{
		stageManager->getPlayer()->weaponManager->currentWeapon->reload();
		if(tutorialManager->getCurrentPage() == 7)
			tutorialManager->increasePage();
		isPressReload = false;
	}
	//addNewSpriteInWorld(touchPoint);
}


void GameState::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
