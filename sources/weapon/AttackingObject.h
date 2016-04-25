#pragma once

#include<Box2D/Box2D.h>
#include "UserDataInBody.h"

#define PTM_RATIO 32

class AttackingObject
{
public:
	int headshot_damage;
	int damage;
	int piercing;

	b2Body *body;
	bool isDead;
	b2Vec2 vCurrentDir;
	CCSprite* bulSpr;

	list<void*> pierceList;
public:
	AttackingObject(b2World* world,b2Vec2 vFirePos,b2Vec2 vTargetPos,
		int bulletSpeed,int dam,int headshot_damage,int _piercing,CCTexture2D* bulletTexture,int aniNum=1,
		bool doesBelongToPlayer=true);
	virtual void release();
	
	bool find(void* f);
	void minusPiercing();
};

