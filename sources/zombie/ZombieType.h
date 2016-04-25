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
	virtual bool checkBehavior()=0; //개체마다 특수한 행동을 체크
	void checkAttacking(float dt,b2Vec2 pv,b2Vec2 zv);// attackrankge와 pv,zv를 사용한 checkAttacking


	//////기본설정
	int zHp;
	int zScore;
	int zSpeed;
	int zDamage;
	int zAttackRange;
	Zombie::ztype_enum zType; 

	///////////body, 애니메이션
	b2Vec2 bodyToHeadVec; // 몸의 중점과 머리의 중점과의 벡터
	b2Vec2 bodyToHeadRJVec; // 몸의 중점과 머리joint와의 벡터

	//////////디자이너에게 body, head, rj의 좌표를 요청.

	CCTexture2D* bodyMoveAnimTec;
	int bodyMoveAnimSize;
	CCTexture2D* headTec;

	CCSprite* headS;
	CCSprite* bodyS;
};

