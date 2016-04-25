#include "FastZombie.h"
#include "balancing_const/BALANCING_FAST_ZOMBIE.h"

FastZombie::FastZombie(){}
FastZombie::~FastZombie(){
}
FastZombie::FastZombie(b2World* _world, direction_enum _direction,CCLayer* _layer,float32 sX,float32 sY)
	:Zombie(_world,_direction,_layer,sX,sY)
{
	setTypeStat(
		fast,
		FAST_SPEED,
		FAST_MAX_HP,
		FAST_GET_SCORE,
		FAST_DAMAGE,
		FAST_ATTACK_RANGE,
		FAST_ATCK_COOLTIME,
		FAST_GEN_ENERGE);

	ZombieResourceManager* zrm = ZombieResourceManager::getInstance();

	characterBodyInit(
		getOneTextureSize(zrm->fastZ_walkAnimTec,zrm->fastZ_walkAnimSize),
		CCSize(18,76),		
		ccp(63,93),
		this,
		ZORDER_ZOMBIE,
		createAnim(zrm->fastZ_walkAnimTec,zrm->fastZ_walkAnimSize,0.1f,-1),
		TYPE_ZOMBIE
		);
	headBodyInit(CCSprite::createWithTexture(zrm->fastZ_headTec),50.0f/2,ccp(50,34));
	attackBodyInit(createAnim(zrm->fastZ_attackAnimTec,zrm->fastZ_attackAnimSize,FAST_ANIM_INTERVAL_TIME,-1),CCSize(24,80),
		ccp(30,92),5);
	deadAnim = NULL;
	headShotAnim = createAnim(zrm->fastZ_deadAnimTec,zrm->fastZ_deadAnimSize,0.2f,1);
	skillAnim = NULL;

}

