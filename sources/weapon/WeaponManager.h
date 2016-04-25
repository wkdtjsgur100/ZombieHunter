#pragma  once

#include "Weapon.h"
#include <list>
#include <vector>
#include <cocos2d.h>
#include "ui/Buttons.h"
#include "system/SoundManager.h"

USING_NS_CC;
using namespace std;

class WeaponManager : public CCObject
{
	
	CCLayer* target;
	
	CCTexture2D* reloadCallTex;
	Buttons* reloadButtons;
public:

	CCSprite* reloadCallSpr;

	WeaponManager(CCLayer* target,CCPoint initPos,int zOrder);
	
	void release();
	
	void fire(b2Vec2 vTarget);
	
	void update(float dt);
	
	void updateFirePos(b2Vec2 vFirePos);
	
	bool changeWeapon(int index);

	void addWeapon(Weapon* weapon,CCMenuItemImage* item);

	bool containPointWithReloadSpr(CCPoint pos)
	{	
		return reloadCallSpr->boundingBox().containsPoint(pos);
	}
	void pressingReloadSpr()
	{
		reloadCallSpr->setScale(0.9f);
	}
	void nonPressingReloadSpr()
	{
		reloadCallSpr->setScale(1.0f);
	}
	void buyWeapon(int index,int addCnt)
	{
		weapons.at(index)->setMaxRemainBulletCnt(weapons.at(index)->getMaxRemainBulletCnt()+addCnt);
	}

	CCSprite* getLeftHand();
	CCSprite* getRightHand();
	void clearHandsRotate();
	
	vector<Weapon*> weapons;				//무기들. 무기 바꾸는걸 많이 할테니 vector를 쓰자.
	Buttons* weaponButtons;
	Weapon* currentWeapon;
	int currentWeaponTag;
};
