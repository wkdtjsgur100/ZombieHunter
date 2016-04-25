#pragma once
#include <list>
#include "NormalZombie.h"
#include "HugeZombie.h"
#include "FastZombie.h"
#include "ThrowerZombie.h"
#include "FlyingZombie.h"
#include "ZombieResourceManager.h"
#include "Timer.h"
#include "Player.h"
#include "MoveObjectByCamera.h"
#include "util/EffectManager.h"

#define NORMAL_ENERGE 5
#define FAST_ENERGE 10
#define THROWER_ENERGE 50               // 젠 에너지
#define FLYING_ENERGE 100              // 젠 에너지

using std::list;

class ZombieManager :public MoveObjectByCamera
{
public:
	void moveByCamera(float dX);
public:

	enum genMode{normal=01000,fever=02000};

	ZombieManager();
	ZombieManager(b2World* _world, CCLayer* _layer);
	~ZombieManager(void);

	void allKillZombies();
	void walkAlongGround(GroundManager* gm);
	void autoGenerate(CCPoint genPos);
	void generate(Zombie* z);
	void generateHugeZ(CCPoint genPos);
	void attackZombie(Zombie* zombie,int damage,int headshot_damage,bool isHeadAttacked,b2Vec2 force);
	void deleteZombie(Zombie* zombie);
	bool findZombie(Zombie* zombie);
	void update(float dt,b2Vec2 pv);
	void updateEnerge(float dt);
	void plusEnerge(float plusDelta)
	{
		currentGenEnerge+=plusDelta;
		if(currentGenEnerge>=maxGenEnerge)currentGenEnerge = maxGenEnerge;
	}
	void setGenTime(float max,float min);
	void setGenType(int max,int min)
	{
		if(max < min)
		{
			genTypeMax = min;
			genTypeMin = max;
		}
		else
		{
			genTypeMax = max;
			genTypeMin = min;
		}
	}
	void setGenNum(int genNum)
	{
		this->genNum = genNum;
	}
	void setEnerges(float chargeEnerge,float maxGenEnerge)
	{
		this->chargeEnerge = chargeEnerge;
		this->maxGenEnerge = maxGenEnerge;
	}
	void setGenTime(long minTime,long maxTime)
	{
		minGenTime = minTime;
		maxGenTime = maxTime;
	}
	int canGenerateZombie(int genTypeMin,int genTypeMax);
	void setCoolTimes(long normalZ,long fastZ,long throwerZ, long flyingZ)
	{
		normalCurZCT = normalZCT = normalZ;
		fastCurZCT = fastZCT = fastZ;
		ThrowerCurZCT = throwerZCT = throwerZ;
		flyingCurZCT = flyingZCT = flyingZ;
		
	}
	void changeMode(genMode mode);

	int getSize();
	
	list<Zombie*> zombies;
	list<Zombie*> deadZombies;
	HugeZombie* hugeZombie;

	float currentGenEnerge;
	float maxGenEnerge;

	genMode generateMode;

private:
	Timer genTimer;
	Timer fastTimer,flyingTimer,normalTimer,throwerTimer;
	int genTypeMax,genTypeMin;
	int genNum;
	int hugeZombieSoundId;

	float chargeEnerge;

	//생산주기 최소/최대시간.
	long minGenTime,maxGenTime;

	//각 좀비 쿨타임(현재값. 변동 가능)
	long normalCurZCT, fastCurZCT, flyingCurZCT, ThrowerCurZCT; 

	//각 좀비 쿨타임(스테이지별 상수)
	long flyingZCT,fastZCT,normalZCT,throwerZCT;
	 
	b2World* world;
	CCLayer* mLayer;
	CCSize winSize;
};

