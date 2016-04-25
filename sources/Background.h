#pragma once

#include "MoveObjectByCamera.h"
#include <list>
#include <vector>
#include "Timer.h"
#include "RandManager.h"
#include "UserDataInBody.h"
#include "ui/Buttons.h"
#include "weapon/WeaponManager.h"
#include "ScoreManager.h"

using namespace std;

/*
	뒷 배경을 제어하는 곳.
*/
class BackgroundObject
{
	CCSprite* spr;

	float speedRatio; //움직이는 속도를 speedRatio로 곱해서 움직인다. speedRatio가 
					  //0~1.0f 는 더 느린속도고, 1.0f~ 은 더 빠른속도로 움직인다.
					   //(카메라가 움직이는 속도 기준)
	CCLayer* target;
public:
	CCSprite* getSprite() { return spr; }

	CCPoint getPosition() { return spr->getPosition();}
	CCSize getContentSize() { return spr->getContentSize(); }

	BackgroundObject(CCLayer* target,CCTexture2D* texture,CCPoint initPoint,
		int zOrder=0,float speedRatio = 1.0f)
	{
		this->target = target;
		spr = CCSprite::createWithTexture(texture);
	
		spr->setPosition(ccp(initPoint.x,initPoint.y));

		target->addChild(spr,zOrder);
		this->speedRatio = speedRatio;
	}
	void release()
	{
		target->removeChild(spr);
		target = NULL;
		spr = NULL;
	}
	void moveByCamera(float dX)
	{
		spr->setPosition(ccp(spr->getPositionX()+dX*speedRatio,spr->getPositionY()));				
	}
};

class BackgroundObjectManager  : public MoveObjectByCamera
{
	
protected:
	CCLayer* target;
	vector<CCTexture2D*> texture;
	
	Timer randTimer;
	RandManager randForCreate;

	int zOrder;
	float speedRatio;
	int initHeight;
	list<BackgroundObject*> objects;
public:
	BackgroundObjectManager(CCLayer* target)
		:zOrder(0),
		speedRatio(1.0f),
		initHeight(0)
	{
		randTimer.setGenTimeRand(0,0);
		this->target = target;
	}
	void release()
	{
		list<BackgroundObject*>::iterator it;

		for(it = objects.begin();it != objects.end();it++)
		{
			(*it)->release();
			delete (*it);
			(*it) = NULL;
		}
		objects.clear();
		target = NULL;
	}
	void setGenTime(float max,float min)
	{
		randTimer.setGenTimeRand(max,min);
	}
	void addTexturePattern(CCTexture2D* texture)
	{
		this->texture.push_back(texture);
		randForCreate.setRange(0,this->texture.size()-1);
	}
	void setInitHeight(int initHeight)
	{
		this->initHeight = initHeight;
	}
	void setZOrderOnCreate(int zOrder)
	{
		this->zOrder = zOrder;
	}
	void setSpeedRatioOnCreate(float speedRatio)
	{
		this->speedRatio = speedRatio;
	}
	void moveByCamera(float dX)
	{
		if(texture.empty())
		{
			CC_ASSERT("texture empty");
			return;
		}
		
		if(objects.empty())
		{
			if(randTimer.isTimeOver())
			{
				CCTexture2D* randTex = texture.at(randForCreate.getCompleteRand());

				CCPoint initPoint;
				initPoint.x = target->getContentSize().width + randTex->getContentSize().width*0.5;
				initPoint.y = randTex->getContentSize().height*0.5 + initHeight;

				objects.push_back(new BackgroundObject(target,randTex,initPoint,zOrder,speedRatio));
				randTimer.updateRandClock();
			}
			else
				randTimer.clocking();
			return;
		}

		if(objects.back()->getPosition().x +
			objects.back()->getContentSize().width*0.5 + dX <= target->getContentSize().width)
		{
			if(randTimer.isTimeOver())
			{
				CCTexture2D* randTex = texture.at(randForCreate.getCompleteRand());

				CCPoint initPoint;
				initPoint.x = target->getContentSize().width + randTex->getContentSize().width*0.5;
				initPoint.y = randTex->getContentSize().height*0.5 + initHeight;

				createBackGroundObject(initPoint,randTex);
				randTimer.updateRandClock();
			}
			else
				randTimer.clocking();
		}

		list<BackgroundObject*>::iterator it;
		for(it = objects.begin();it != objects.end();it++)
			(*it)->moveByCamera(dX);

		if(objects.front()->getPosition().x <= -objects.front()->getContentSize().width*0.5)
		{
			objects.front()->release();
			delete objects.front();	
			objects.pop_front();
		}
	}

	virtual void createBackGroundObject(CCPoint initPos,CCTexture2D* texture)
	{
		objects.push_back(new BackgroundObject(target,texture,initPos,zOrder,speedRatio));
	}
	void createRandomBackGroundObject(float x)
	{
		if(texture.empty())
		{
			CC_ASSERT("texture empty..");
			return;
		}
		CCTexture2D* randTex = texture.at(randForCreate.getCompleteRand());
		createBackGroundObject(ccp(x,initHeight+randTex->getContentSize().height*0.5),randTex);
	}
};
class CloudsManager : public BackgroundObjectManager
{
	float speed;
public:
	CloudsManager(CCLayer* _target);
	void setMoveSpeed(float speed);
	void update();
};
class StarManager : public BackgroundObjectManager
{
	int delta;
	unsigned int startAlpha;
public:
	StarManager(CCLayer* _target);
	void setAlphaDelta(int delta);
	void setStartAlpha(unsigned int startAlpha);
	void update();
	virtual void createBackGroundObject(CCPoint initPos,CCTexture2D* texture)
	{
		BackgroundObject* obj = new BackgroundObject(target,texture,initPos,zOrder,speedRatio);
		obj->getSprite()->setOpacity(startAlpha);
		objects.push_back(obj);
	}
};

class ShopManager : public BackgroundObjectManager,CCObject
{
	CCLayer* mTarget;
	CCSprite* shopBox;
	WeaponManager* weaponTarget;
	
	void buyWeapon(int index,int point,int bulletNum);
public:
	ShopManager(CCLayer* target);

	void buyPistol(CCObject* pSender);
	void buyShotGun(CCObject* pSender);
	void buyDDabal(CCObject* pSender);
	void buyFireGun(CCObject* pSender);
	void buyRifle(CCObject* pSender);

	void setWeaponManagerTarget(WeaponManager* weaponTarget);
	void update(CCPoint playerPos);

	void setObjectZOrder(int zOrder);
	void setShopZOrder(int zOrder);
	bool isPlayerInShop(){ return shopBox->isVisible(); } 
};

class Background
{
	BackgroundObjectManager* layer1;
	BackgroundObjectManager* layer2;
	BackgroundObjectManager* layer3;
	BackgroundObjectManager* layer4;
	BackgroundObjectManager* layer5;
	BackgroundObjectManager* layer6;
	BackgroundObjectManager* layer7;
	CloudsManager* clouds;
	CloudsManager* clouds2;
//	BackgroundObjectManager* clouds;		//구름1
//	BackgroundObjectManager* clouds2;	   //구름2
//	BackgroundObjectManager* background_2; //뒷배경2
//	BackgroundObjectManager* background_3; //뒷배경3
	BackgroundObjectManager* baseground; //base_ground	
	StarManager* star;
public:
	Background(CCLayer* target);
	~Background(void);

	void update()
	{
		
		clouds->update();
		clouds2->update();
		star->update();
		
	}
};

