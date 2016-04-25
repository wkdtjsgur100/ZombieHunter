#include "NormalZombie.h"
#include "balancing_const/BALANCING_NORMAL_ZOMBIE.h"

NormalZombie::NormalZombie(){}
NormalZombie::~NormalZombie(){
}
NormalZombie::NormalZombie(b2World* _world, direction_enum _direction,CCLayer* _layer,float32 sX,float32 sY)
	:Zombie(_world,_direction,_layer,sX,sY)
{
	setTypeStat(
		normal,
		NORMAL_SPEED,
		NORMAL_MAX_HP,
		NORMAL_GET_SCORE,
		NORMAL_DAMAGE,
		NORMAL_ATTACK_RANGE,
		NORMAL_ATCK_COOLTIME,
		NORMAL_GEN_ENERGE);

	ZombieResourceManager* zrm = ZombieResourceManager::getInstance();

	characterBodyInit(
		getOneTextureSize(zrm->normalZ_bodyWalkAnimTec,zrm->normalZ_bodyWalkAnimSize),
		CCSize(22.0f,49.0f),
		ccp(50.0f,61.0f),
		this,
		ZORDER_ZOMBIE,
		createAnim(zrm->normalZ_bodyWalkAnimTec,zrm->normalZ_bodyWalkAnimSize,0.1f,-1),
		TYPE_ZOMBIE
		);
	headBodyInit(CCSprite::createWithTexture(zrm->normalZ_headTec),38.0f/2,ccp(26,25));
	attackBodyInit(createAnim(zrm->normalZ_bodyAttackAnimTec,zrm->normalZ_bodyAttackAnimSize,0.1f,-1),CCSize(22.0f,55.0f),
		ccp(13,55),3);
	deadAnim = createAnim(zrm->normalZ_deadAnimTec,zrm->normalZ_deadAnimSize,0.2f,1);
	headShotAnim = createAnim(zrm->normalZ_headShotAnimTec,zrm->normalZ_headShotAnimSize,0.2f,1);

	skillAnim = NULL;

}
