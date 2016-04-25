#include "Stone.h"

Stone::Stone(b2World* world,b2Vec2 vFirePos,b2Vec2 vTargetPos,
	int StoneSpeed,int dam,int _piercing,CCTexture2D* StoneTexture,float variable,int aniNum)
	: AttackingObject(world,vFirePos,vTargetPos,StoneSpeed,dam,dam,_piercing,StoneTexture,aniNum, false)
{
	b2Vec2 bv = b2Vec2(vCurrentDir.x,vCurrentDir.y);

	b2Vec2 newVec;
	newVec.x = bv.x;
	newVec.y = bv.y;

	float r = CC_DEGREES_TO_RADIANS(variable);

	float cs = cos(r);
	float sn = sin(r);

	newVec.x = cs*bv.x - sn*bv.y;
	newVec.y = sn*bv.x + cs*bv.y;

	newVec.x*=StoneSpeed; // ÅºÈ¯ ½ºÇÇµå
	newVec.y*=StoneSpeed;

	body->SetLinearVelocity(newVec);
	body->SetGravityScale(1);
}
