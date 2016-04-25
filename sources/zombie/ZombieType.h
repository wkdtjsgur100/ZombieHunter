#pragma once

#include "Box2D/Box2D.h"
#include "cocos2d.h"
#include "ZombieResourceManager.h"
#include "Zombie.h"

#define PTM_RATIO 32

USING_NS_CC;

class ZombieType
{
public:


	
	ZombieType();
	~ZombieType(); 

	virtual void update(float dt,b2Vec2 pv,b2Vec2 zv)=0;
	virtual bool checkBehavior()=0; //��ü���� Ư���� �ൿ�� üũ
	void checkAttacking(float dt,b2Vec2 pv,b2Vec2 zv);// attackrankge�� pv,zv�� ����� checkAttacking


	//////�⺻����
	int zHp;
	int zScore;
	int zSpeed;
	int zDamage;
	int zAttackRange;
	Zombie::ztype_enum zType; 

	///////////body, �ִϸ��̼�
	b2Vec2 bodyToHeadVec; // ���� ������ �Ӹ��� �������� ����
	b2Vec2 bodyToHeadRJVec; // ���� ������ �Ӹ�joint���� ����

	//////////�����̳ʿ��� body, head, rj�� ��ǥ�� ��û.

	CCTexture2D* bodyMoveAnimTec;
	int bodyMoveAnimSize;
	CCTexture2D* headTec;

	CCSprite* headS;
	CCSprite* bodyS;
};

