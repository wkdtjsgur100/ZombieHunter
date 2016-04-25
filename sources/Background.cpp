#include "Background.h"
#include "balancing_const/BALANCING_ETC.h"

CloudsManager::CloudsManager(CCLayer* _target)
	:BackgroundObjectManager(_target),
	speed(0.5f)
{
}
void CloudsManager::setMoveSpeed(float speed)
{
	this->speed = speed;
}
void CloudsManager::update()
{
	int randHeight = rand() % (500-300+1) + 300;
	setInitHeight(randHeight);
	BackgroundObjectManager::moveByCamera(-speed);
}
StarManager::StarManager(CCLayer* _target)
	:BackgroundObjectManager(_target),
	delta(10),
	startAlpha(255)
{
}
void StarManager::setAlphaDelta(int delta)
{
	this->delta = delta;
}
void StarManager::setStartAlpha(unsigned int startAlpha)
{
	this->startAlpha = startAlpha;
}
void StarManager::update()
{
	list<BackgroundObject*>::iterator it;
	int randHeight = rand() % (540-300+1) + 300;
	setInitHeight(randHeight);
	for(it = objects.begin();it != objects.end();it++)
	{
		int opacity = (*it)->getSprite()->getOpacity();
		if( (opacity-delta) <= 0 || (opacity-delta) >= 255)
		{
			if( (opacity-delta) <= 0 )
				(*it)->getSprite()->setOpacity(0);
			else
				(*it)->getSprite()->setOpacity(255);
			delta *= -1;
		}
		else
			(*it)->getSprite()->setOpacity(opacity-delta);
	}
}

ShopManager::ShopManager(CCLayer* target)
	:BackgroundObjectManager(target)
{
	shopBox = CCSprite::create("UI/shopbox.png");

	shopBox->setPosition(ccp(480,400));

	shopBox->setVisible(false);

	target->addChild(shopBox,ZORDER_INTERFACE+1);

	CCMenuItemImage* pistol = CCMenuItemImage::create("weapon/select_weapon/weapon1.png","weapon/select_weapon/weapon1.png",this,menu_selector(ShopManager::buyPistol));
	CCMenuItemImage* shotgun = CCMenuItemImage::create("weapon/select_weapon/weapon2_1.png","weapon/select_weapon/weapon2_1.png",this,menu_selector(ShopManager::buyShotGun));
	CCMenuItemImage* rifle = CCMenuItemImage::create("weapon/select_weapon/weapon3.png","weapon/select_weapon/weapon3.png",this,menu_selector(ShopManager::buyDDabal));
	CCMenuItemImage* ddabal = CCMenuItemImage::create("weapon/select_weapon/weapon4.png","weapon/select_weapon/weapon4.png",this,menu_selector(ShopManager::buyFireGun));
	CCMenuItemImage* firegun = CCMenuItemImage::create("weapon/select_weapon/weapon5.png","weapon/select_weapon/weapon5.png",this,menu_selector(ShopManager::buyRifle));

	pistol->getSelectedImage()->setScale(0.9f,0.9f);
	shotgun->getSelectedImage()->setScale(0.9f,0.9f);
	rifle->getSelectedImage()->setScale(0.9f,0.9f);
	ddabal->getSelectedImage()->setScale(0.9f,0.9f);
	firegun->getSelectedImage()->setScale(0.9f,0.9f);
	
	Buttons* itemList = Buttons::create();
	
	itemList->setMusicSrc("music/ui/ui_reload.ogg");
	itemList->setTouchEnabled(true);

	itemList->addChild(pistol);
	itemList->addChild(shotgun);
	itemList->addChild(rifle);
	itemList->addChild(ddabal);
	itemList->addChild(firegun);

	itemList->alignItemsHorizontally();

	itemList->setPosition(ccp(252,126));

	shopBox->addChild(itemList);
	
	
	CCLabelTTF* sellPoints[5];
	CCLabelTTF* sellBulletNum[5];

	for(int i=0;i<5;i++)
	{
		char buf[256];

		if(i == 0)
			sprintf(buf,"%d",PRICE_PISTOL);
		else if(i == 1)
			sprintf(buf,"%d",PRICE_SHOTGUN);
		else if(i == 2)
			sprintf(buf,"%d",PRICE_DDABAL);
		else if(i == 3)
			sprintf(buf,"%d",PRICE_FIREGUN);
		else if(i == 4)
			sprintf(buf,"%d",PRICE_RIFLE);

		sellPoints[i] = CCLabelTTF::create(buf,"fonts/rank_font.ttf",20.0f,CCSizeMake(42,20),kCCTextAlignmentRight);
		sellPoints[i]->setPosition(ccp(76+92*i,76));
		sellPoints[i]->setHorizontalAlignment(kCCTextAlignmentRight);

		if(i == 0)
			sprintf(buf,"X%d",SHOP_PISTOL_BULLET_NUM);
		else if(i == 1)
			sprintf(buf,"X%d",SHOP_SHOTGUN_BULLET_NUM);
		else if(i == 2)
			sprintf(buf,"X%d",SHOP_DDABAL_BULLET_NUM);
		else if(i == 3)
			sprintf(buf,"X%d",SHOP_FIREGUN_BULLET_NUM);
		else if(i == 4)
			sprintf(buf,"X%d",SHOP_RIFLE_BULLET_NUM);
		
		sellBulletNum[i] = CCLabelTTF::create(buf,"fonts/rank_font.ttf",20.0f,CCSizeMake(42,20),kCCTextAlignmentRight);

		sellBulletNum[i]->setPosition(ccp(76+92*i,50));
		sellBulletNum[i]->setHorizontalAlignment(kCCTextAlignmentRight);
		
		shopBox->addChild(sellPoints[i]);
		shopBox->addChild(sellBulletNum[i]);
	}
}
void ShopManager::buyWeapon(int index,int point,int bulletNum)
{
	if(	(int)ScoreManager::sharedInstance()->point - point < 0)
	{
		///포인트가 부족합니다..
		return;
	}
	ScoreManager::sharedInstance()->point -= point;
	weaponTarget->buyWeapon(index,bulletNum);
}
void ShopManager::buyPistol(CCObject* pSender)
{
	buyWeapon(0,PRICE_PISTOL,SHOP_PISTOL_BULLET_NUM);
}
void ShopManager::buyShotGun(CCObject* pSender)
{

	buyWeapon(1,PRICE_SHOTGUN,SHOP_SHOTGUN_BULLET_NUM);
}
void ShopManager::buyDDabal(CCObject* pSender)
{

	buyWeapon(2,PRICE_DDABAL,SHOP_DDABAL_BULLET_NUM);
}
void ShopManager::buyFireGun(CCObject* pSender)
{
	buyWeapon(3,PRICE_FIREGUN,SHOP_FIREGUN_BULLET_NUM);
}
void ShopManager::buyRifle(CCObject* pSender)
{

	buyWeapon(4,PRICE_RIFLE,SHOP_RIFLE_BULLET_NUM);
}
void ShopManager::setWeaponManagerTarget(WeaponManager* weaponTarget)
{
	this->weaponTarget = weaponTarget;
}
void ShopManager::update(CCPoint playerPos)
{
	list<BackgroundObject*>::iterator it;

	for(it = objects.begin(); it != objects.end(); it++)
	{
		float minX = (*it)->getPosition().x - (*it)->getContentSize().width*0.5;
		float maxX = (*it)->getPosition().x + (*it)->getContentSize().width*0.5;
		
		if(playerPos.x >= minX && playerPos.x <= maxX)
		{
			shopBox->setVisible(true);
		}
		else
		{
			shopBox->setVisible(false);
		}
	}
}
void ShopManager::setObjectZOrder(int zOrder)
{
	list<BackgroundObject*>::iterator it;

	for(it = objects.begin(); it != objects.end(); it++)
	{
		(*it)->getSprite()->setZOrder(zOrder);
	}
}
void ShopManager::setShopZOrder(int zOrder)
{
	shopBox->setZOrder(zOrder);
}
Background::Background(CCLayer* target)
{
	
	baseground = new BackgroundObjectManager(target);
	baseground->setZOrderOnCreate(ZORDER_BASEGROUND);

	CCTexture2D* basegroundTex1 = CCTextureCache::sharedTextureCache()->addImage("ground/base_ground_1.png");
	CCTexture2D* basegroundTex2= CCTextureCache::sharedTextureCache()->addImage("ground/base_ground_2.png");

	baseground->addTexturePattern(basegroundTex1);
	baseground->addTexturePattern(basegroundTex2);

	baseground->createRandomBackGroundObject(basegroundTex1->getContentSize().width*0.5);
	
	////////////////////////////////////////////////////////////////////

	int layerZOrder = -1;
	int initHeight = 145;

	/////////////////////////////////////////

	layer1 = new BackgroundObjectManager(target);

	layer1->setInitHeight(initHeight);
	layer1->setZOrderOnCreate(--layerZOrder);
	layer1->setSpeedRatioOnCreate(1);
	layer1->setGenTime(0.4,1.0);

	layer1->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/01/bush01.png"));
	layer1->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/01/bush02.png"));
	layer1->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/01/bush03.png"));

	layer1->createRandomBackGroundObject(250);
	layer1->createRandomBackGroundObject(600);
	//////////////////////////////////////////////////////
	
	layer2 = new BackgroundObjectManager(target);

	layer2->setInitHeight(initHeight);
	layer2->setZOrderOnCreate(--layerZOrder);
	layer2->setSpeedRatioOnCreate(1);
	layer2->setGenTime(1.5,0.7);

	layer2->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/02/back11.png"));
	layer2->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/02/back12.png"));
	layer2->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/02/back13.png"));
	layer2->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/02/back14.png"));
	layer2->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/02/back15.png"));
	layer2->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/02/back16.png"));
	layer2->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/02/back17.png"));

	layer2->createRandomBackGroundObject(400);
	layer2->createRandomBackGroundObject(800);

	//////////////////////////////////////////////////////
	layer3 = new BackgroundObjectManager(target);

	layer3->setInitHeight(initHeight);
	layer3->setZOrderOnCreate(--layerZOrder);
	layer3->setSpeedRatioOnCreate(1);
	layer3->setGenTime(4.0,2.0);

	layer3->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/03/wall01.png"));
	layer3->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/03/wall02.png"));
	layer3->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/03/wall03.png"));
	layer3->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/03/wall04.png"));

	layer3->createRandomBackGroundObject(200);
	layer3->createRandomBackGroundObject(700);
	//////////////////////////////////////////////////////
	layer4 = new BackgroundObjectManager(target);

	layer4->setInitHeight(initHeight);
	layer4->setZOrderOnCreate(--layerZOrder);
	layer4->setSpeedRatioOnCreate(0.9);
	layer4->setGenTime(5.0,4.0);

	layer4->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/04/util01.png"));
	layer4->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/04/util02-1.png"));
	layer4->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/04/util02-2.png"));
	layer4->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/04/util02-3.png"));
	layer4->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/04/util03.png"));

	layer4->createRandomBackGroundObject(target->getContentSize().width);

	//////////////////////////////////////////////////////
	layer5 = new BackgroundObjectManager(target);

	layer5->setInitHeight(initHeight);
	layer5->setZOrderOnCreate(--layerZOrder);
	layer5->setSpeedRatioOnCreate(0.8);
	layer5->setGenTime(3.0,2.0);

	layer5->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/05/back02.png"));
	layer5->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/05/back03.png"));
	layer5->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/05/back04.png"));
	layer5->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/05/back05.png"));
	layer5->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/05/back06.png"));

	layer5->createRandomBackGroundObject(300);
	layer5->createRandomBackGroundObject(900);
	//////////////////////////////////////////////////////
	layer6 = new BackgroundObjectManager(target);

	layer6->setInitHeight(initHeight);
	layer6->setZOrderOnCreate(--layerZOrder);
	layer6->setSpeedRatioOnCreate(0.7);

	layer6->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/06/back07.png"));
	layer6->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/06/back08.png"));

	layer6->createRandomBackGroundObject(target->getContentSize().width*0.5);
	//////////////////////////////////////////////////////
	layer7 = new BackgroundObjectManager(target);

	layer7->setInitHeight(initHeight);
	layer7->setZOrderOnCreate(--layerZOrder);
	layer7->setSpeedRatioOnCreate(0.6);

	layer7->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/07/back09.png"));
	layer7->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/07/back10.png"));

	layer7->createRandomBackGroundObject(target->getContentSize().width*0.5);

	//////////////////////////////////////////////////////
	clouds = new CloudsManager(target);

	clouds->setInitHeight(initHeight+250);
	clouds->setZOrderOnCreate(--layerZOrder);
	clouds->setSpeedRatioOnCreate(0.4);
	clouds->setGenTime(4.0,2.0);
	clouds->setMoveSpeed(1.0f);

	clouds->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/cloud/cloud.png"));
	clouds->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/cloud/cloud02.png"));
	clouds->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/cloud/cloud03.png"));

	clouds->createRandomBackGroundObject(600);

	///////////////////////////////////////////
	clouds2 = new CloudsManager(target);

	clouds2->setInitHeight(initHeight+180);
	clouds2->setZOrderOnCreate(layerZOrder);
	clouds2->setSpeedRatioOnCreate(0.4);
	clouds2->setGenTime(5.0,4.0);

	clouds2->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/cloud/cloud.png"));
	clouds2->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/cloud/cloud02.png"));
	clouds2->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/cloud/cloud05.png"));

	clouds2->createRandomBackGroundObject(300);
	//////////////////////////////////////////
	star = new StarManager(target);

	star->setInitHeight(initHeight+180);
	star->setZOrderOnCreate(--layerZOrder);
	star->setSpeedRatioOnCreate(0.4);
	star->setGenTime(1.0,0.3);
	star->setAlphaDelta(2);

	star->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/particle/star01.png"));
	star->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/particle/star02.png"));
	star->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("background/particle/star03.png"));

	for(int i=0;i<20;i++)
	{
		int randAlpha = rand()%255;
		star->setStartAlpha(randAlpha);
		int randHeight = rand()%(540-300+1)+300;
		star->setInitHeight(randHeight);
		int randWidth = rand() % (int)(target->getContentSize().width);

		star->createRandomBackGroundObject(randWidth);
	}
	
}
Background::~Background(void)
{
	layer1->release(); delete layer1; 
	layer2->release(); delete layer2;
	layer3->release(); delete layer3;
	layer4->release(); delete layer4;
	layer5->release(); delete layer5;
	layer6->release(); delete layer6;
	layer7->release(); delete layer7;
	clouds->release(); delete clouds;
	clouds2->release(); delete clouds2;
	baseground->release(); delete baseground;
	star->release(); delete star;
}