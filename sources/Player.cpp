#include "Player.h"
#include "Camera.h"
#include "balancing_const/BALANCING_PLAYER.h"
#include "balancing_const/BALANCING_WEAPON.h"

Player::Player(void)
{
}
Player::Player(b2World* _world,CCLayer* _layer,float32 sX,float32 sY)
	:Character(_world,_layer,sX,sY)
{
	world = _world;
	speed = normalSpeed = PLAYER_SPEED;
	hp = PLAYER_MAX_HP;
	maxHp = PLAYER_MAX_HP;
	CCTexture2D* walkT = CCTextureCache::sharedTextureCache()->addImage("player/walk.png");
	CCTexture2D* hittedT = CCTextureCache::sharedTextureCache()->addImage("player/walk.png");
	CCTexture2D* deadT = CCTextureCache::sharedTextureCache()->addImage("player/death.png");
	CCTexture2D* stopT = CCTextureCache::sharedTextureCache()->addImage("player/stop.png");
	hittedAnim = createAnim(hittedT,CCSize(1,1),1,1);
	deadAnim = createAnim(deadT,CCSize(6,1),0.2f,1);
	stopAnim = createAnim(stopT,CCSize(1,1),0.2f,1);

	characterBodyInit(
		getOneTextureSize(walkT,CCSize(6,1)),
		CCSize(44,65),
		ccp(34,51),
		this,
		ZORDER_PLAYER,
		createAnim(walkT,CCSize(6,1),0.2f,-1),
		TYPE_PLAYER
		);
	currentState = walking;
	walkT = NULL;
	hittedT = NULL;
	deadT = NULL;
	
	
	isShootRight = true;
	shootAngle = 0;
	lastFireTime = 0;
	isLastShooting = false;
	actionManager = CCDirector::sharedDirector()->getActionManager();

	bindStartTime = bindTotalTime = 0;
	weaponInit();
	moveDistance = 0.0f;
	nonDieTime = PLAYER_NONDIE_TIME;
}
Player::~Player()
{
	weaponManager->release();
	delete weaponManager;
	if(characterS!=NULL)
		mLayer->removeChild(characterS);
	UserDataInBody* ud = (UserDataInBody*)character->GetUserData();
	delete ud;

	world->DestroyBody(character);
	if(walkAnim!=NULL)walkAnim->release();
}
void Player::damaged(int dam)
{
	if(nonDieTime > 0)
		return;

	CCBlink *blink = CCBlink::create(PLAYER_NONDIE_TIME,5);
	CCShow* show = CCShow::create();
	
	CCSequence* totalAction = CCSequence::create(blink,show,NULL);

	characterS->runAction(totalAction);

	if(currentState==dead || currentState==dying) return;
	if(hp - dam <= 0)
		hp = 0;
	else hp -= dam;
	
	if(hp<=0)
	{
		SoundManager::getInstance()->playEffect("music/zombie/bulldozer_action.ogg");

		changeState(dying);
	}
	hpBar->setScaleX(( float)hp/(float)maxHp );

	nonDieTime = PLAYER_NONDIE_TIME;
}
void Player::update(float dt,GroundManager *gm)
{
	nonDieTime -= dt;

	if(nonDieTime <= 0)
	{
		characterS->setVisible(true);//무적상태가 아니면 무조건 보이게..
	}
	else
	{
		if(currentState == dying || currentState == dead)
			characterS->setVisible(true);
	}
	if(currentState == dying || currentState == dead)
	{
		if(characterS->getActionByTag(TAG_CURRENT_ANIM)==NULL ) //지정한 액션(애니메이션)의 loop가 끝나면 release해서 NULL로 만드는듯.. 딥빡! 일단 임시방편.
		{
			if(currentState==dead) return;
			changeState(dead);
		}
		
		return; 
	}
	if(currentState == binded)
	{
		binding();
		return;
	}

	walk(dt,gm);
	weaponManager->update(dt);
	weaponManager->updateFirePos(getPosition());
	if(Timer::getCurrentTime() - lastFireTime > 3000 && isLastShooting ) //3초 지나면 총 각도 원상복귀
	{
		weaponManager->clearHandsRotate();
		shootAngle = 0;
		isLastShooting = false;
	}


	CCArray *childArray = weaponManager->weaponButtons->getChildren();
	CCObject* it = NULL;
	CCARRAY_FOREACH(childArray,it)
	{
		CCMenuItemImage* itemImage = dynamic_cast<CCMenuItemImage*>(it);
		if(itemImage != NULL)
		{
			CCArray *fonts = itemImage->getChildren();
			CCObject* fontIt = NULL;
			CCARRAY_FOREACH(fonts,fontIt)
			{
				CCLabelTTF* font = dynamic_cast<CCLabelTTF*>(fontIt);

				if(font != NULL)
				{
					char str[100];
					memset(str,0,100);

					if(font->getTag() == 0)
						sprintf(str,"%d",weaponManager->weapons.at(itemImage->getTag())->getCurrentRemainBulletCnt());
					else
						sprintf(str,"/%d",weaponManager->weapons.at(itemImage->getTag())->getMaxRemainBulletCnt());
					
					font->setString(str);
				}
			}

			if(itemImage->isSelected())
			{
				if(weaponManager->changeWeapon(itemImage->getTag()))
					changeHands();
			}

		}
	}

}

void Player::changeState(pState_enum pState)
{
	if(currentState==pState)return;

	CCAnimate* cam = NULL;

	if(pState == stopping)
	{
		cam = stopAnim;
		if(cam == NULL) return;
	}
	if(pState==walking)
	{
		cam = walkAnim;
		if(cam==NULL)return;
	}
	else if(pState==binded)
	{
		currentState = pState; // binded된 이미지가 현재 없으므로 이렇게함.
		return;
	}
	else if(pState==hitted)
	{
		
	}
	else if(pState==dying)
	{
		cam = deadAnim;
		if(cam==NULL)return;
	}
	else if(pState==dead)
	{
		currentState = pState;
		return;
	}
	currentState = pState;
	actionManager->pauseTarget(characterS->getActionByTag(TAG_CURRENT_ANIM));
	actionManager->resumeTarget(cam);
	
	characterS->stopAllActions();
	characterS->runAction(cam);
}

void Player::walk(float dt,GroundManager* gm)
{
	if(currentDirection==stop)
	{
		changeState(stopping);
		return;
	}
	else
	{
		if(currentState != binded)
			changeState(walking);
	}
	int mark = 1;
	if(currentDirection==left) mark = -1;

	CCPoint moveVec = gm->getMoveVecFromGround(getPositionWithCocos().x,speed * mark);
	b2Vec2 mv(moveVec.x/PTM_RATIO,moveVec.y/PTM_RATIO);

	if(getPositionWithCocos().x > CCDirector::sharedDirector()->getWinSize().width*0.5
		&& mark == 1)
	{
		moveForPlayer(b2Vec2(0,mv.y));
		Camera::sharedCameraInstance()->move(moveVec.x);
		moveDistance += 0.1f;
		if(moveDistance >= 20.0f)
		{
			ScoreManager::sharedInstance()->moveDistance += 20;
			moveDistance = 0.0f;
		}
	}
	else
	{
		if(mark == -1 && getPositionWithCocos().x<0)return; // 왼쪽 밖으론 못나감
		moveForPlayer(mv);
	}

}
void Player::moveForPlayer(b2Vec2 tv)
{
	moveFor(tv);

}

void Player::bind(long time,int damage)
{
	changeState(binded);
	damaged(damage);
	bindTotalTime = time;
	bindStartTime = Timer::getCurrentTime();
}

void Player::binding()
{
	if(Timer::getCurrentTime()-bindStartTime < bindTotalTime)return;
	changeState(walking);
}


void Player::fire(b2Vec2 tv)
{
	if(currentState == binded || currentState == dying || currentState == dead)
		return;

	float f;
	if(tv.x < getPosition().x) // 플레이어보다 왼쪽 쏜다.
	{
		CCPoint normalVec = getPositionWithCocos()-ccp(tv.x*PTM_RATIO,tv.y*PTM_RATIO);
		//팔 회전 각도.
		f = -CC_RADIANS_TO_DEGREES(normalVec.getAngle());
		
		if(isShootRight)
		{
			characterS->setFlipX(true);
			weaponManager->getLeftHand()->setFlipX(true);
			weaponManager->getLeftHand()->setAnchorPoint(ccp(1-weaponManager->getLeftHand()->getAnchorPoint().x,weaponManager->getLeftHand()->getAnchorPoint().y));
			weaponManager->getLeftHand()->setPositionX(characterS->getContentSize().width-weaponManager->getLeftHand()->getPositionX());
			if(!weaponManager->currentWeapon->isDoubleHand)
			{
				weaponManager->getRightHand()->setFlipX(true);
				weaponManager->getRightHand()->setAnchorPoint(ccp(1-weaponManager->getRightHand()->getAnchorPoint().x,weaponManager->getRightHand()->getAnchorPoint().y));
				weaponManager->getRightHand()->setPositionX(characterS->getContentSize().width-weaponManager->getRightHand()->getPositionX());
			}
		isShootRight = false;
		}
		
	}
	else // 오른쪽 쏜다.
	{
		CCPoint normalVec = ccp(tv.x*PTM_RATIO,tv.y*PTM_RATIO)-getPositionWithCocos();
		//팔 회전 각도.
		f = -CC_RADIANS_TO_DEGREES(normalVec.getAngle());
		if(!isShootRight)
		{
			characterS->setFlipX(false);
			weaponManager->getLeftHand()->setFlipX(false);	
			weaponManager->getLeftHand()->setAnchorPoint(ccp(1-weaponManager->getLeftHand()->getAnchorPoint().x,weaponManager->getLeftHand()->getAnchorPoint().y));
			weaponManager->getLeftHand()->setPositionX(characterS->getContentSize().width-weaponManager->getLeftHand()->getPositionX());
			if(!weaponManager->currentWeapon->isDoubleHand)
			{
				weaponManager->getRightHand()->setFlipX(false);	
				weaponManager->getRightHand()->setAnchorPoint(ccp(1-weaponManager->getRightHand()->getAnchorPoint().x,weaponManager->getRightHand()->getAnchorPoint().y));
				weaponManager->getRightHand()->setPositionX(characterS->getContentSize().width-weaponManager->getRightHand()->getPositionX());
			}
			
		}
		isShootRight = true;
	}
	weaponManager->fire(tv);
	shootAngle = f;
	weaponManager->getLeftHand()->setRotation(f);
	weaponManager->getRightHand()->setRotation(f);

	lastFireTime = Timer::getCurrentTime();
	isLastShooting = true;
}
void Player::weaponInit()
{
	////////////////////////////////////////////////////Weapon
	float sX = getPosition().x;
	float sY = getPosition().y;

	weaponManager = new WeaponManager(mLayer,ccp(240,-5),ZORDER_BUTTON);

	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("weapon/bullet/bullet_1.png");

	WeaponBuilder* wbNormalGun = new WeaponBuilder(texture,P_DAMAGE,P_PIERCING,b2Vec2(sX,sY),P_BULLET_SPEED,P_MAX_BULLET,P_RELOADABLE_NUM);
	Weapon* normalGun = new NormalGun(world,mLayer,wbNormalGun
		->setFireTime(P_COOLTIME)
		->setInterSection(P_RANGE)
		->setZOrder(ZORDER_BULLET)
		->setHeadShotDamage(P_HEADSHOT_DAMAGE)
		->setLeftHand(CCSprite::create("player/pistol_right.png"),ccp(8,34),ccp(18,76),characterS->getContentSize().height)
		->setRightHand(CCSprite::create("player/pistol_left.png"),ccp(3,5),ccp(45,20),characterS->getContentSize().height)
		);

	delete wbNormalGun;
	wbNormalGun = NULL;

	CCMenuItemImage* normalImg = CCMenuItemImage::create("weapon/select_weapon/weapon1.png",
		"weapon/select_weapon/weapon1.png");

	normalImg->getSelectedImage()->setScale(0.9f,0.9f);

	weaponManager->addWeapon(normalGun,normalImg);

	WeaponBuilder* wbShotGun = new WeaponBuilder(texture,SG_DAMAGE,SG_PIERCING,b2Vec2(sX,sY),SG_BULLET_SPEED,SG_MAX_BULLET,SG_RELOADABLE_NUM);

	Weapon* shotGun = new ShotGun(world,mLayer,
		wbShotGun->setBulletNum(SG_BULLETNUM)
		->setInterSection(SG_RANGE)
		->setZOrder(ZORDER_BULLET)
		->setFireTime(SG_COOLTIME)
		->setRandomVariable(SG_THETA)
		->setLeftHand(CCSprite::create("player/shotgun.png"),ccp(12,20),ccp(18,49),characterS->getContentSize().height)
		->setRightHand(CCSprite::create("player/hand.png"),ccp(12,20),ccp(30,65),characterS->getContentSize().height)
		);

	delete wbShotGun;
	wbShotGun = NULL;
	CCMenuItemImage* shotgunImg = CCMenuItemImage::create("weapon/select_weapon/weapon2_1.png",
		"weapon/select_weapon/weapon2_1.png");

	shotgunImg->getSelectedImage()->setScale(0.9f,0.9f);
	
	weaponManager->addWeapon(shotGun,shotgunImg);

	WeaponBuilder* wbDDabal = new WeaponBuilder(texture,DD_DAMAGE,DD_PIERCING,b2Vec2(sX,sY),DD_BULLET_SPEED,DD_MAX_BULLET,DD_RELOADABLE_NUM);

	Weapon* ddabal = new NormalGun(world,mLayer,
		wbDDabal
		->setInterSection(DD_RANGE)
		->setZOrder(ZORDER_BULLET)
		->setFireTime(DD_COOLTIME)
		->setSoundDelay(0.5f)
		->setLeftHand(CCSprite::create("player/ddabal.png"),ccp(13,17),ccp(18,49),characterS->getContentSize().height)
		->setRightHand(CCSprite::create("player/hand.png"),ccp(12,20),ccp(30,65),characterS->getContentSize().height)
		);

	delete wbDDabal;
	wbDDabal = NULL;

	CCMenuItemImage* ddabalImg = CCMenuItemImage::create("weapon/select_weapon/weapon3.png",
		"weapon/select_weapon/weapon3.png");

	ddabalImg->getSelectedImage()->setScale(0.9f,0.9f);

	weaponManager->addWeapon(ddabal,ddabalImg);

	CCTexture2D* fireTex = CCTextureCache::sharedTextureCache()->addImage("weapon/bullet/fire.png");

	WeaponBuilder* wbFireGun = new WeaponBuilder(fireTex,FG_DAMAGE,FG_PIERCING,b2Vec2(sX,sY),FG_BULLET_SPEED,FG_MAX_BULLET,FG_RELOADABLE_NUM);

	Weapon* fireGun = new FireGun(world,mLayer,wbFireGun
		->setBulletNum(FG_BULLET_NUM)
		->setFireTime(FG_COOLTIME)
		->setInterSection(FG_RANGE)
		->setRandomVariable(FG_THETA)
		->setAnimationCount(4)
		->setSoundDelay(0.5f)
		->setZOrder(ZORDER_BULLET)
		->setLeftHand(CCSprite::create("player/firegun.png"),ccp(13,24),ccp(18,49),characterS->getContentSize().height)
		->setRightHand(CCSprite::create("player/hand.png"),ccp(12,20),ccp(30,65),characterS->getContentSize().height)
		);

	delete wbFireGun;
	wbFireGun =  NULL;

	CCMenuItemImage* fireImg = CCMenuItemImage::create("weapon/select_weapon/weapon4.png",
		"weapon/select_weapon/weapon4.png");

	fireImg->getSelectedImage()->setScale(0.9f,0.9f);

	weaponManager->addWeapon(fireGun,fireImg);
	////////////
	
	WeaponBuilder* wbRifle = new WeaponBuilder(texture,RIFLE_DAMAGE,RIFLE_PIERCING,b2Vec2(sX,sY),RIFLE_BULLET_SPEED,RIFLE_MAX_BULLET,RIFLE_RELOADABLE_NUM);

	Weapon* rifle = new NormalGun(world,mLayer,
		wbRifle
		->setInterSection(RIFLE_RANGE)
		->setZOrder(ZORDER_BULLET)
		->setFireTime(RIFLE_COOLTIME)
		->setHeadShotDamage(RIFLE_HEADSHOT_DAMAGE)
		->setLeftHand(CCSprite::create("player/rifle.png"),ccp(13,17),ccp(20,40),characterS->getContentSize().height)
		->setRightHand(CCSprite::create("player/hand.png"),ccp(12,20),ccp(30,65),characterS->getContentSize().height)
		);

	delete wbRifle;
	wbRifle = NULL;

	CCMenuItemImage* rifleImg = CCMenuItemImage::create("weapon/select_weapon/weapon5.png",
		"weapon/select_weapon/weapon5.png");

	rifleImg->getSelectedImage()->setAnchorPoint(ccp(0,0));
	rifleImg->getSelectedImage()->setScale(0.9f,0.9f);

	weaponManager->addWeapon(rifle,rifleImg);

	weaponManager->changeWeapon(0);
	changeHands();
}

void Player::changeHands()
{
	Weapon* wp = weaponManager->currentWeapon;
	characterS->removeAllChildrenWithCleanup(true);
	characterS->addChild(weaponManager->getLeftHand(),1,TAG_LEFT_HAND);	
	
	float x = wp->getLeftWeaponPoint().x/weaponManager->getLeftHand()->getContentSize().width;
	float y = wp->getLeftWeaponPoint().y/weaponManager->getLeftHand()->getContentSize().height;
	CCPoint v = wp->getLeftShoulderPoint();
	weaponManager->getLeftHand()->setFlipX(false);
	if(!isShootRight)
	{
		x = 1-x;
		v.x = characterS->getContentSize().width - v.x;
		weaponManager->getLeftHand()->setFlipX(true);
	}
	weaponManager->getLeftHand()->setAnchorPoint(ccp(x,y));
	weaponManager->getLeftHand()->setPosition(v);
	weaponManager->getLeftHand()->setRotation(shootAngle);
	
	if(wp->isDoubleHand) return; // 밑은 한손무기용

	characterS->addChild(weaponManager->getRightHand(),-1,TAG_RIGHT_HAND);
	x = wp->getRightWeaponPoint().x/weaponManager->getRightHand()->getContentSize().width;
	y = wp->getRightWeaponPoint().y/weaponManager->getRightHand()->getContentSize().height;
	v = wp->getRightShoulderPoint();
	weaponManager->getRightHand()->setFlipX(false);
	if(!isShootRight)
	{
		x = 1-x;
		v.x = characterS->getContentSize().width - v.x;
		weaponManager->getRightHand()->setFlipX(true);
	}

	weaponManager->getRightHand()->setAnchorPoint(ccp(x,y));
	weaponManager->getRightHand()->setPosition(v);
	weaponManager->getRightHand()->setRotation(shootAngle);

}

void Player::changeDirection(direction_enum pd)
{
	if(currentDirection==pd) return;
	currentDirection = pd;

	if(currentDirection==stop)
	{
		if(currentState==walking)
			characterS->stopAllActions();
	}
	else
	{
		if(currentState==walking)
		{
			if(characterS->getActionByTag(TAG_CURRENT_ANIM)==NULL)
				characterS->runAction(walkAnim);
		}
	}
}

bool Player::isDead()
{
	if(currentState==dead)
		return true;
	return false;
}
