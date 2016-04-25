#include "ZombieResourceManager.h"

ZombieResourceManager* ZombieResourceManager::instance = NULL;

ZombieResourceManager::ZombieResourceManager()
{///////////normalZombie
	normalZ_headTec = addTexture("zombie_normal/head.png");

	normalZ_bodyWalkAnimTec = addTexture("zombie_normal/walking_anim.png");
	normalZ_bodyWalkAnimSize.setSize(8,1);
	
	normalZ_bodyAttackAnimTec = addTexture("zombie_normal/attacking_anim.png");
	normalZ_bodyAttackAnimSize.setSize(5,1);

	normalZ_deadAnimTec = addTexture("zombie_normal/dead_anim.png");
	normalZ_deadAnimSize.setSize(7,1);

	normalZ_headShotAnimTec = addTexture("zombie_normal/headshot_dead_anim.png");
	normalZ_headShotAnimSize.setSize(8,1);


	///////////HugeZombie
	hugeZ_bodyWalkAnimTec = addTexture("zombie_huge/walk.png");
	hugeZ_bodyWalkAnimSize.setSize(2,1);

	//////fast
	fastZ_headTec = addTexture("zombie_fast/head.png");

	fastZ_walkAnimTec = addTexture("zombie_fast/walking_anim.png");
	fastZ_walkAnimSize.setSize(6,1);

	fastZ_attackAnimTec = addTexture("zombie_fast/attacking_anim.png");
	fastZ_attackAnimSize.setSize(11,1);

	fastZ_deadAnimTec = addTexture("zombie_fast/dying_anim.png");
	fastZ_deadAnimSize.setSize(6,1);

	//flying

	flyingZ_walkAnimTec = addTexture("zombie_fly/walking_anim.png");
	flyingZ_walkAnimSize.setSize(5,1);

	flyingZ_skillAnimTec = addTexture("zombie_fly/skilling_anim.png");
	flyingZ_skillAnimSize.setSize(5,1);

	flyingZ_deadAnimTec = addTexture("zombie_fly/dying_anim.png");
	flyingZ_deadAnimSize.setSize(5,1);

	//throw
	throwZ_headTec = addTexture("zombie_throw/head.png");
	
	throwZ_walkAnimTec = addTexture("zombie_throw/walking_anim.png");
	throwZ_walkAnimSize.setSize(8,1);

	throwZ_skillAnimTec = addTexture("zombie_throw/skilling_anim.png");
	throwZ_skillAnimSize.setSize(6,1);

	throwZ_deadAnimTec = addTexture("zombie_throw/dying_anim.png");
	throwZ_deadAnimSize.setSize(6,1);

	throwZ_headShotAnimTec = addTexture("zombie_throw/headshot_dying_anim.png");
	throwZ_headShotAnimSize.setSize(8,1);
}
void ZombieResourceManager::release()
{
	//if(normalZ_headTec) normalZ_headTec->release();
	//if(normalZ_bodyWalkAnimTec) normalZ_bodyWalkAnimTec->release();
	//if(normalZ_bodyAttackAnimTec) normalZ_bodyAttackAnimTec->release();
	//if(normalZ_deadAnimTec) normalZ_deadAnimTec->release();
	//if(jumpZ_bodyWalkAnimTec) jumpZ_bodyWalkAnimTec->release();
	//if(jumpZ_bodySkillAnimTec) jumpZ_bodySkillAnimTec->release();
	//if(fastZ_walkAnimTec) fastZ_walkAnimTec->release();
	//if(fastZ_attackAnimTec) fastZ_attackAnimTec->release();
}
ZombieResourceManager* ZombieResourceManager::getInstance()
{
	if(!instance)
		instance = new ZombieResourceManager();
	return instance;
}
CCTexture2D*  ZombieResourceManager::addTexture(char* fileName)
{
	return CCTextureCache::sharedTextureCache()->addImage(fileName);
}
