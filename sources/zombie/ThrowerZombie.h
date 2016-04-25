#pragma once

#include "Zombie.h"
#include "weapon/Weapon.h"

class ThrowerZombie:public Zombie
{
private:
	Weapon* stoneWeapon;
	int skillRange;
	float skillDistance;
public:
	ThrowerZombie();
	~ThrowerZombie();
	ThrowerZombie(b2World* _world, direction_enum _direction,CCLayer* _layer,float32 sX,float32 sY);
	void release();

	virtual void attack();
	virtual void skillStart();
	virtual void skill();
	virtual bool checkSkill();
	virtual void cameraMoved(float dX);
	virtual void subUpdate(float dt,b2Vec2 pv);
};