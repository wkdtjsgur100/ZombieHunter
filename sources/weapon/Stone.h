#pragma once

#include "weapon/AttackingObject.h"

#define PTM_RATIO 32

class Stone : public AttackingObject
{
public:
	Stone(b2World* world,b2Vec2 vFirePos,b2Vec2 vTargetPos,
	int StoneSpeed,int dam,int _piercing,CCTexture2D* StoneTexture,float variable=0.0f,int aniNum=1);
};
