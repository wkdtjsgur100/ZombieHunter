#pragma once
#include "Zombie.h"

class HugeZombie :public Zombie
{
	float init_y;
	float strength_y;
public:
	HugeZombie();
	~HugeZombie();
	HugeZombie(b2World* _world, direction_enum _direction,CCLayer* _layer,float32 sX,float32 sY);
	virtual bool checkAttack();
	virtual void walk(float dt);
	virtual void directionUpdate();
	void update(float dt,b2Vec2 pv);
};