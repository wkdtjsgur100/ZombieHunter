#pragma once
#include "Box2D/Box2D.h"
#include "MoveObjectByCamera.h"
#include "weapon/Bullet.h"
#include "weapon/Stone.h"
#include <cocos2d.h>
#include "system/SoundManager.h"


USING_NS_CC;

class WeaponBuilder		//Bulider ���Ϥ�
{
private:
	int damage;
	int headshot_damage;
	int piercing;
	CCTexture2D* bulletTexture;
	b2Vec2 vFirePos;		//
	int bulletSpeed;		//�Ѿ˼ӵ�
	unsigned long fireTime; // ����ð�
	int bulletNum;			//�ѹ��� ������ �Ѿ� ����.
	float intersection;     //��Ÿ�.
	float randomVariable;     //������ �ִ� ������ �ִ밪.(������ �ִ밪)?
	int zOrder;
	int aniCount;		//�ִϸ��̼��� ����.
	int maxRemainBulletCnt;		//��ü �Ѿ� ��
	int remainBulletCnt;		//���� �Ѿ� ��
	CCSprite* leftHand;
	CCSprite* rightHand;
	CCPoint leftWPoint;
	CCPoint leftSPoint;
	CCPoint rightWPoint;
	CCPoint rightSPoint;
	float soundDelay;
public:
	///			getter
	int getAnimationCount() { return aniCount; }
	int getDamage() { return damage; }
	int getHeadShotDamage(){ return headshot_damage; }
	int getPiercing() { return piercing; }
	int getZOrder(){ return zOrder; }
	CCTexture2D* getBulletTexture() { return bulletTexture; }
	b2Vec2 getFirePos() { return vFirePos; }
	int getBulletSpeed() { return bulletSpeed; }
	unsigned long getFireTime() { return fireTime; } 
	float getIntersection() { return intersection; }
	float getRandomVariable() { return randomVariable; }
	int getBulletNum() { return bulletNum; } 
	int getMaxRemainBulletCnt() { return maxRemainBulletCnt; }
	int getRemainBulletCnt() { return remainBulletCnt; } 
	CCSprite* getLeftHand(){return leftHand;}
	CCSprite* getRightHand(){return rightHand;}
	CCPoint getLeftHandPointW(){return leftWPoint;}
	CCPoint getLeftHandPointS(){return leftSPoint;}
	CCPoint getRightHandPointW(){return rightWPoint;}
	CCPoint getRightHandPointS(){return rightSPoint;}
	float getSoundDelay(){ return soundDelay; }

	WeaponBuilder(CCTexture2D* _bulletTexture,int _damage,int _piercing,b2Vec2 _vFirePos,int _bulletSpeed,
		int _maxRemainBulletCnt,int _remainBulletCnt)
		: fireTime(0),
		zOrder(0),
		bulletNum(1),
		intersection(10000.0f),
		randomVariable(0),
		aniCount(1),
		bulletTexture(_bulletTexture),
		damage(_damage),
		piercing(_piercing),
		vFirePos(_vFirePos),
		bulletSpeed(_bulletSpeed),
		maxRemainBulletCnt(_maxRemainBulletCnt),
		remainBulletCnt(_remainBulletCnt),
		soundDelay(0.0f)
	{
		headshot_damage = damage;
		leftHand = NULL;
		rightHand = NULL;
	}
public: 
	///		setter
	WeaponBuilder* setAnimationCount(int aniCount);
	WeaponBuilder* setZOrder(int zOrder);
	WeaponBuilder* setRandomVariable(float randomVariable);
	WeaponBuilder* setInterSection(float intersection);
	WeaponBuilder* setBulletNum(int bulletNum);
	WeaponBuilder* setFireTime(unsigned long fireTime);	
	WeaponBuilder* setLeftHand(CCSprite* left,CCPoint weaponPoint,CCPoint shoulderPoint,float cHeight);
	WeaponBuilder* setRightHand(CCSprite* right,CCPoint weaponPoint,CCPoint shoulderPoint,float cHeight);
	WeaponBuilder* setSoundDelay(float soundDelay);
	WeaponBuilder* setHeadShotDamage(int headshot_damage);
};
class Weapon : public MoveObjectByCamera
{
public: 
	bool isDoubleHand;
	CCPoint getLeftShoulderPoint(){return leftHandPoint_S;}
	CCPoint getLeftWeaponPoint(){return leftHandPoint_W;}
	CCPoint getRightShoulderPoint(){return rightHandPoint_S;}
	CCPoint getRightWeaponPoint(){return rightHandPoint_W;}
protected:
	list<AttackingObject*> bullets;
	list<AttackingObject*> deadBullets;

	b2World* world;
	CCLayer *targetLayer;

	CCTexture2D* bulletTexture; //�Ѿ� �ϳ��� �ؽ�ó
	b2Vec2 vFirePos;        //�Ѿ��� ������ ��ġ.
	int damage;		 //�Ѿ� ������.
	int headshot_damage;
	int piercing;	 //�����
	int bulletSpeed; //�Ѿ� �ӵ�.
	int zOrder; //�Ѿ��� zOrder;
	int remainBulletCnt;
	int maxRemainBulletCnt;
	int aniCount;		//�Ѿ� �ִϸ��̼� ����
	int currentRemainBulletCnt;
	bool reloading;
	bool isFire;

	float intersection; //�Ѿ��� ��Ÿ�.
	float lastFireTime; //for ����ð�����
	float fireTime; //����ð�. ������ ������.
	float soundDelay;
	float soundTimer; //�ð��� ��� ����

	CCSprite* leftHand;
	CCSprite* rightHand;
	//weapon������ ��ǥ
	CCPoint leftHandPoint_W;
	//character������ shoulder��ǥ
	CCPoint leftHandPoint_S;

	//weapon������ ��ǥ
	CCPoint rightHandPoint_W;
	//character������ shoulder��ǥ
	CCPoint rightHandPoint_S;

protected:

	Weapon(b2World* world,CCLayer* targetLayer,WeaponBuilder* wb)
	{
		this->world = world;
		this->targetLayer = targetLayer;
		this->bulletSpeed = wb->getBulletSpeed();
		this->bulletTexture = wb->getBulletTexture();
		this->damage = wb->getDamage();
		this->piercing = wb->getPiercing();
		this->vFirePos = wb->getFirePos();
		this->zOrder = wb->getZOrder();
		this->remainBulletCnt = wb->getRemainBulletCnt();
		this->maxRemainBulletCnt = wb->getMaxRemainBulletCnt();
		this->currentRemainBulletCnt = wb->getRemainBulletCnt();
		this->aniCount = wb->getAnimationCount();
		this->soundDelay = wb->getSoundDelay();
		this->headshot_damage = wb->getHeadShotDamage();
		soundTimer = 0.0f;

		isFire= true;

		this->intersection = wb->getIntersection();
		this->fireTime = wb->getFireTime()/1000.0;
		this->lastFireTime = wb->getFireTime()/1000.0;

		this->leftHand = wb->getLeftHand();
		this->rightHand = wb->getRightHand();
		isDoubleHand = false;
		if(this->rightHand==NULL)isDoubleHand=true;//�μչ�����.
		this->leftHandPoint_W = wb->getLeftHandPointW();
		this->leftHandPoint_S = wb->getLeftHandPointS();

		this->rightHandPoint_W = wb->getRightHandPointW();
		this->rightHandPoint_S = wb->getRightHandPointS();
		reloading = false;
		soundId = -1;
	}
	
	void removeBullet(AttackingObject* attackObject);
public:
	int soundId;

	bool isSoundOn()
	{
		if(soundTimer <= 0.0f)
			return true;
		return false;
	}
	void updateSoundTimer()
	{
		soundTimer = soundDelay;
	}

	bool getIsFire() { return isFire; }
	bool isReloading() { return reloading; }

	float getFireTime() { return fireTime; }
	void setHands(CCSprite* left, CCSprite* right);

		CCSprite* getLeftHand();
	CCSprite* getRightHand();

	//firePos update
	int getCurrentRemainBulletCnt() { return currentRemainBulletCnt; }
	int getMaxRemainBulletCnt() { return maxRemainBulletCnt; }

	void setRemainBulletCnt(int remainBulletCnt)	{	this->remainBulletCnt = remainBulletCnt;	}
	void setMaxRemainBulletCnt(int maxRemainBulletCnt)	{	this->maxRemainBulletCnt = maxRemainBulletCnt;   }
	b2Vec2 getFirePos() { return vFirePos; }
	void updateFirePos(b2Vec2 firePos)	{		vFirePos = firePos;			}

	void reload(){
		if(maxRemainBulletCnt <= 0)
			return;

		maxRemainBulletCnt -= (remainBulletCnt - currentRemainBulletCnt);
		currentRemainBulletCnt = remainBulletCnt;
		reloading = false;
	}
	//��!��!
	void fire(b2Vec2 vTarget);
	virtual void fireBullet(b2Vec2 vTarget) = 0;
	//by MoveObjectByCamera
	virtual void release(){
		list<AttackingObject*>::iterator it;
		for(it=bullets.begin(); it!=bullets.end(); it++)
			addDeadBullet(*it);
		for(it=deadBullets.begin();it!=deadBullets.end();it++)
			removeBullet(*it);
		
		deadBullets.clear();
		bullets.clear();
		if(leftHand != NULL)
			leftHand->removeAllChildrenWithCleanup(true);
		if(rightHand != NULL)rightHand->removeAllChildrenWithCleanup(true);
		if(leftHand != NULL) leftHand->release();
		if(rightHand != NULL) rightHand->release();
		leftHand = NULL;
		rightHand = NULL;
	}
	void update(float dt);

	void addDeadBullet(AttackingObject* attackObject);
	void moveByCamera(float dX);
};

class ThrowingStone : public Weapon
{
protected:
public:
	ThrowingStone(b2World* world,CCLayer* target,WeaponBuilder* wb);
	void release();
	
	void fireBullet(b2Vec2 vTarget);
};

class NormalGun : public Weapon
{
protected:
public:
	NormalGun(b2World* world,CCLayer* target,WeaponBuilder* wb);
	void release();
	//virtual 
	void fireBullet(b2Vec2 vTarget);
};

class ShotGun : public NormalGun
{
protected:
	int bulletNum;
	float randomVariable;
public:
	ShotGun(b2World* world,CCLayer* target,WeaponBuilder* wb);

	virtual void fireBullet(b2Vec2 vTarget);
};

class FireGun : public ShotGun
{
public:
	FireGun(b2World* world,CCLayer* target,WeaponBuilder* wb);

	void fireBullet(b2Vec2 vTarget);
};
