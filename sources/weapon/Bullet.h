#pragma once

#include "AttackingObject.h"

#define PTM_RATIO 32

class Bullet : public AttackingObject
{
public:
	Bullet(b2World* world,b2Vec2 weaponVec,b2Vec2 tv,int bulletSpeed,int dam,int headshot_damage, int _piercing,
		CCTexture2D* bulletTexture,float variable=0.0f,int aniNum=1);
	virtual void release();
};

class Fire : public Bullet
{
public:
	Fire(b2World* world,b2Vec2 weaponVec,b2Vec2 tv,int bulletSpeed,int dam, int _piercing,
		CCTexture2D* bulletTexture,float variable=0.0f,int aniNum=1);
	void release();
};