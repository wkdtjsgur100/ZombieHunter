#pragma once

#include <cocos2d.h>
#include <Box2D/Box2D.h>
#include <list>
#include "Ground.h"
#include "Architecture.h"
#include "zombie/ZombieManager.h"
#include "Background.h"

//#define BODY_DEBUG

using namespace std;
USING_NS_CC;

struct Range
{
	float rangeMax;
	float rangeMin;

	Range()
		:rangeMax(0.0f),rangeMin(0.0f)
	{
	}
};
class StageInfo
{
public:
	float stageTime;

	Range groundGenTime;
	Range groundGenType;

	Range zombieGenTime;
	Range zombieGenType;
	
	Range architectGenTime;
	Range architectGenType;

	Range zombieOnArchitectNumber;
	Range zombieOnArchitectType;

	Range shopGenTime;

	float chargeEnerge;
	float maxEnerge;
	
	long normalZCool,fastZCool,throwerZCool,flyingZCool;
	
	StageInfo();
	~StageInfo();
};

class StageManager : public MoveObjectByCamera
{
	vector<StageInfo> stages;

	b2World* world;
	CCLayer* targetLayer;
	
	int currentStage;
	StageInfo currentStageInfo;

	GroundManager *groundMng;
	ArchitectManager *archMng;
	ZombieManager* zombieManager;
	Background* background;
	ShopManager* shop;
	Player* player;
public:
	
	GroundManager* getGroundMng() { return groundMng; }
	ArchitectManager *getArchMng() { return archMng; }
	ZombieManager* getZombieManager() { return zombieManager; }
	Background* getBackground() { return background; }
	ShopManager* getShop() { return shop; }
	Player* getPlayer() { return player; }

	StageManager(b2World* world,CCLayer* targetLayer);
	~StageManager();

	void playerInit();
	void backgroundInit();
	void shopInit();
	void groundInit();
	void zombieInit();
	void archInit();

	bool isInit();

	void loadStage();
	void setStage(int stage);
	void stageUp();
	void stageDown();
	void update(float dt);
	void moveByCamera(float dX);
};

