#pragma  once
#include "cocos2d.h"
USING_NS_CC;

class ZombieResourceManager
{
	static ZombieResourceManager* instance;

	ZombieResourceManager();
	~ZombieResourceManager(){};
public:
	static ZombieResourceManager* getInstance();
	void release();

	CCTexture2D* addTexture(char* fileName);

	CCTexture2D* normalZ_bodyWalkAnimTec;
	CCSize normalZ_bodyWalkAnimSize;

	CCTexture2D* normalZ_headTec;

	CCTexture2D* normalZ_bodyAttackAnimTec;
	CCSize normalZ_bodyAttackAnimSize;

	

	CCTexture2D* normalZ_deadAnimTec;
	CCSize normalZ_deadAnimSize;

	CCTexture2D* normalZ_headShotAnimTec;
	CCSize normalZ_headShotAnimSize;
	
	//////////////////////
	CCTexture2D* hugeZ_bodyWalkAnimTec;
	CCSize hugeZ_bodyWalkAnimSize;

	///////////////////fast
	CCTexture2D* fastZ_headTec;

	CCTexture2D* fastZ_walkAnimTec;
	CCSize fastZ_walkAnimSize;

	CCTexture2D* fastZ_attackAnimTec;
	CCSize fastZ_attackAnimSize;

	CCTexture2D* fastZ_deadAnimTec;
	CCSize fastZ_deadAnimSize;

	CCTexture2D* fastZ_headShotAnimTec;
	CCSize fastZ_headShotAnimSize;



	//////flying
	CCTexture2D* flyingZ_walkAnimTec;
	CCSize flyingZ_walkAnimSize;

	CCTexture2D* flyingZ_skillAnimTec;
	CCSize flyingZ_skillAnimSize;

	CCTexture2D* flyingZ_deadAnimTec;
	CCSize flyingZ_deadAnimSize;

	CCTexture2D* flyingZ_headShotAnimTec;
	CCSize flyingZ_headShotAnimSize;
	//thrower
	CCTexture2D* throwZ_headTec;

	CCTexture2D* throwZ_walkAnimTec;
	CCSize throwZ_walkAnimSize;

	CCTexture2D* throwZ_skillAnimTec;
	CCSize throwZ_skillAnimSize;

	CCTexture2D* throwZ_deadAnimTec;
	CCSize throwZ_deadAnimSize;

	CCTexture2D* throwZ_headShotAnimTec;
	CCSize throwZ_headShotAnimSize;
};
