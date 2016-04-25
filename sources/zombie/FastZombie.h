#pragma once
#include "Zombie.h"

class FastZombie : public Zombie
{
public:
	FastZombie();
	~FastZombie();
	FastZombie(b2World* _world, direction_enum _direction,CCLayer* _layer,float32 sX,float32 sY);

};