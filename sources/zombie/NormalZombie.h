#pragma once
#include "Zombie.h"

class NormalZombie:public Zombie
{
public:
	NormalZombie();
	~NormalZombie();
	NormalZombie(b2World* _world, direction_enum _direction,CCLayer* _layer,float32 sX,float32 sY);


};