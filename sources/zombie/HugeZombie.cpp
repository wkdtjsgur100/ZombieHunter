#include "HugeZombie.h"
#include "balancing_const/BALANCING_HUGE_ZOMBIE.h"

HugeZombie::HugeZombie(){}

HugeZombie::HugeZombie(b2World* _world, direction_enum _direction,CCLayer* _layer,float32 sX,float32 sY)
	:Zombie(_world,_direction,_layer,sX,sY)
{
	setTypeStat(
		huge,
		HUGE_SPEED,
		9999999,
		500,
		1000,
		0,
		1500,
		0);

	ZombieResourceManager* zrm = ZombieResourceManager::getInstance();

	characterBodyInit(
		getOneTextureSize(zrm->hugeZ_bodyWalkAnimTec,zrm->hugeZ_bodyWalkAnimSize),
		CCSize(0.0f,0.0f),
		ccp(0.0f,0.0f),
		this,
		ZORDER_HUGEZOMBIE,
		createAnim(zrm->hugeZ_bodyWalkAnimTec,zrm->hugeZ_bodyWalkAnimSize,0.1f,-1),
		TYPE_HUGE_ZOMBIE
		);
	attackBodyInit(CCSize(62,153),ccp(426,191),true);

	deadAnim = NULL;
	skillAnim = NULL;
	headShotAnim = NULL;

	init_y = character->GetPosition().y;
	strength_y = 4;
//	hpBar->setVisible(false);
}

void HugeZombie::walk(float dt)
{
	//directionChange(Zombie::right);
	//if(currentState!=walking)return; //걷는중 아니면 안걸음

	zombieMoveFor(b2Vec2((float)speed/PTM_RATIO,0/PTM_RATIO));
}

void HugeZombie::update(float dt,b2Vec2 pv)
{
	Zombie::update(dt,pv);

	int random = rand() % 2;

	setPosition(b2Vec2(character->GetPosition().x,init_y+strength_y/PTM_RATIO*random));
}

bool HugeZombie::checkAttack(){return false;}
void HugeZombie::directionUpdate(){}