#pragma once
#include "Zombie.h"

class FlyingZombie:public Zombie
{
public :
	FlyingZombie();
	~FlyingZombie();
	FlyingZombie(b2World* _world, direction_enum _direction,CCLayer* _layer,float32 sX,float32 sY);
	void release();
	virtual void attack();
	virtual void skillStart();
	virtual void skill();
	virtual bool checkSkill();
	virtual bool checkAttack();
	virtual void cameraMoved(float dX);
	virtual void walk(float dt);

	void skillBodyInit(CCAnimate* cam);

	long bindingTime;
	int bindingDamage;
private:
	float skillRange;

	float flyHeight;
	int direction;
	//getYonLine전에 반드시 호출시켜야함.
	void skillCalculator(CCPoint a, CCPoint b);
	float getYonLine(float x);
	float con;
	float multiplyCon;
	unsigned long skillStartTime;
	b2Vec2 skillTarget;
	int skillSpeed;

	
};