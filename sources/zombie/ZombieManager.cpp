#include "ZombieManager.h"
#include "Camera.h"

#define  PTM_RATIO 32
ZombieManager::ZombieManager(){}
ZombieManager::ZombieManager(b2World* _world,CCLayer* _layer)
	:zombies()
{
	world = _world;
	mLayer = _layer;
	setGenTime(3.0f,5.0f);
	setGenType(2,0);
	winSize = CCDirector::sharedDirector()->getWinSize();
	genNum = 1;
	currentGenEnerge = 0;
	generateMode = normal;
	hugeZombie = NULL;
}

bool ZombieManager::findZombie(Zombie* zombie)
{
	return false;
}
void ZombieManager::walkAlongGround(GroundManager* gm)
{
	list<Zombie*>::iterator it;

	for(it = zombies.begin();it!=zombies.end();it++)
	{
		Zombie* z = (*it);
		CCPoint moveDelta;
		int mark = 1;

		if(z->zDirection == Zombie::left)
			mark = -1;
		else
			mark = 1;

		moveDelta = gm->getMoveVecFromGround(z->getPositionWithCocos().x,z->speed*mark);

		z->updateMoveDelta(b2Vec2(moveDelta.x/PTM_RATIO,moveDelta.y/PTM_RATIO));
	}
}
void ZombieManager::allKillZombies()
{
	list<Zombie*>::iterator it;

	for(it=zombies.begin(); it!=zombies.end(); it++)
	{
		(*it)->die(false,b2Vec2(0,0));
	}
}
void ZombieManager::update(float dt,b2Vec2 pv)
{

	//long currentTime = GetTickCount();
	unsigned long currentTime = Timer::getCurrentTime();
	
	updateEnerge(dt);

	list<Zombie*>::iterator it;

	for(it=zombies.begin(); it!=zombies.end(); it++)
	{
		Zombie* z= *it;

		z->update(dt,pv);
		//z->directionUpdate();
		
		if(z->currentState == Zombie::dead ||
			z->getPositionWithCocos().x < 0)
		{
			deadZombies.push_back(z);
		}
	}
	if(hugeZombie) 
	{
		hugeZombie->update(dt,pv);
	}

	for(it=deadZombies.begin(); it!=deadZombies.end(); it++)
	{
		Zombie* dz = *it;
		deleteZombie(dz);
	}
	deadZombies.clear();

}
void ZombieManager::updateEnerge(float dt)
{
	plusEnerge(chargeEnerge*dt);
}
/*
void ZombieManager::killZombieOnArchitecture(Architecture* arch)
{
	list<Zombie*>::iterator it;

	for(it=zombies.begin(); it!=zombies.end(); it++)
	{
		Zombie* z = static_cast<Zombie*>(*it);
		if(z->isInArchitecture) //구조물에 올라와 있고
		{
			if(arch->isIn(z->getPositionWithCocos().x)) //해당 Architect에 포함되면
			{
				b2Filter filter;
				
				filter.categoryBits = CATEGORY_FALLING_ZOMBIE;
				filter.maskBits = CATEGORY_GROUND | CATEGORY_ARCHITECT;

				z->character->SetType(b2_dynamicBody);
				z->character->SetFixedRotation(false);
				z->character->GetFixtureList()->SetSensor(false);

				z->character->GetFixtureList()->SetFilterData(filter);
				z->character->SetGravityScale(1);

				z->changeState(Zombie::falling);
				UserDataInBody* ud = (UserDataInBody*)z->character->GetUserData();

				ud->mType = TYPE_FALLING_ZOMBIE;

				z->character->SetUserData(ud);
			}
		}
	}
}
*/

void ZombieManager::changeMode(genMode mode)
{
	if(mode == ZombieManager::normal || mode == ZombieManager::fever)
	{
		if(generateMode == mode) return;
		generateMode = mode;

		if(generateMode == ZombieManager::normal)
		{
			normalCurZCT = normalZCT;
			fastCurZCT = fastZCT;
			ThrowerCurZCT = throwerZCT;
			flyingCurZCT = flyingZCT;
		}
		else if(generateMode == ZombieManager::fever)
		{
			normalCurZCT = normalZCT/2;
			fastCurZCT = fastZCT/2;
			ThrowerCurZCT = throwerZCT/2;
			flyingCurZCT = flyingZCT/2;
		}
	}
	else return;
}

void ZombieManager::autoGenerate(CCPoint genPos)
{
	//	return;/
//	if(zombies.size()>=1)return;
//	CCLOG("autoGenerate %f %d",currentGenEnerge,generateMode);
	fastTimer.clocking();
	normalTimer.clocking();
	throwerTimer.clocking();
	flyingTimer.clocking();

	if(genTimer.isTimeOver())
	{
		///////////
		for(int i=0;i<genNum;i++)
		{ 
			Zombie* z;
			int num = -1;

			num = canGenerateZombie(genTypeMin,genTypeMax);

			switch(num)
			{
			case -1 : genTimer.updateRandClock(); return;
			case 0 :z = new NormalZombie(world,Zombie::left,mLayer,genPos.x+i*10,genPos.y); break;
			case 1 :z = new FastZombie(world,Zombie::left,mLayer,genPos.x+i*10,genPos.y); break;
			case 2 :z = new ThrowerZombie(world,Zombie::left,mLayer,genPos.x+i*10,genPos.y); break;
			case 3 :z = new FlyingZombie(world,Zombie::left,mLayer,genPos.x+i*10,genPos.y); break;
			}
			zombies.push_back(z);
			plusEnerge(-1 * z->genEnerge);
		}

		long maxGen = maxGenTime;
		if(generateMode == fever)
			maxGen/=2;
		setGenTime(minGenTime,maxGen);

		//genTime = 900000;
	}
	else
		genTimer.clocking();

}

int ZombieManager::canGenerateZombie(int genTypeMin,int genTypeMax)
{
	if(!normalTimer.isTimeOver() && !fastTimer.isTimeOver() && !throwerTimer.isTimeOver() && !flyingTimer.isTimeOver()) return -1;
	int num = rand() % (genTypeMax - genTypeMin + 1) + genTypeMin;
	if(num == 0)
	{ 
		if(!normalTimer.isTimeOver()) return -1;
		if(currentGenEnerge<NORMAL_ENERGE)return -1;//normal생산할 에너지도 없으면 걍 리턴
		normalTimer.setGenTime(normalCurZCT);
		return num;
	}
	else if(num == 1)
	{
		if(!fastTimer.isTimeOver() || currentGenEnerge<FAST_ENERGE)
		{
			return canGenerateZombie(genTypeMin,num-1);
		}
		fastTimer.setGenTime(fastCurZCT);
		return num;
	}
	else if(num == 2)
	{
		if(!throwerTimer.isTimeOver() || currentGenEnerge<THROWER_ENERGE)
		{
			return canGenerateZombie(genTypeMin,num-1);
		}
		throwerTimer.setGenTime(ThrowerCurZCT);
		return num;
	}
	else if(num == 3)
	{
		if(!flyingTimer.isTimeOver() || currentGenEnerge<FLYING_ENERGE)
		{
			return canGenerateZombie(genTypeMin,num-1);
		}
		flyingTimer.setGenTime(flyingCurZCT);	
		return num;	
	}
	else
	{
		CCLog("zombie generate exception..");
		return -1;
	}
}


void ZombieManager::generate(Zombie* z)
{
	zombies.push_back(z);
}

void ZombieManager::generateHugeZ(CCPoint genPos)
{
	hugeZombie = new HugeZombie(world,Zombie::right,mLayer,genPos.x,genPos.y);
	
	hugeZombieSoundId = SoundManager::getInstance()->playEffect("music/zombie/bulldozer_foward_loop.ogg",true);
	//hugeZombie->isInArchitecture = false;
}
void ZombieManager::moveByCamera(float dX)
{
	list<Zombie*>::iterator it;

	for(it=zombies.begin(); it!=zombies.end(); it++)
	{
		Zombie* z = *it;
		z->zombieMoveFor(b2Vec2(dX/PTM_RATIO,0));
		z->cameraMoved(dX/PTM_RATIO);
	}
	if(hugeZombie) hugeZombie->zombieMoveFor(b2Vec2(dX/PTM_RATIO,0));
}
void ZombieManager::attackZombie(Zombie* zombie,int damage,int headshot_damage,bool isHeadAttacked,b2Vec2 force)
{
	int cDamage;
	
	cDamage = damage;

	if(isHeadAttacked) 
		cDamage = headshot_damage;

	if(headshot_damage == damage)
		isHeadAttacked = false;
	
	if(zombie->typeZ == Zombie::ztype_enum::fast)	//fast좀비면
	{
		if(!isHeadAttacked)
			return;				//머리 맞아야지만 데미지입음..
	}
	if(zombie->isDeadOnDamage(cDamage))
	{
		zombie->die(isHeadAttacked,force);
	}
	else
	{
		//SoundManager::getInstance()->playEffect("music/zombie/mon_zombi_damage.ogg");
		//SoundManager::getInstance()->playEffect("music/zombie/mon_zombi_damage_voice.ogg");
	}
}

void ZombieManager::deleteZombie(Zombie* zombie)
{	
	zombie->changeState(Zombie::dead);
	zombies.remove(zombie);

	UserDataInBody* ud = (UserDataInBody *)(zombie->character->GetUserData());
	ud->mSprite->removeFromParentAndCleanup(true);
	mLayer->removeChild(ud->mSprite);
	world->DestroyBody(zombie->character);
	delete ud;
	world->DestroyBody(zombie->body);

	if(!zombie->isHeadLess)
	{
		ud = (UserDataInBody *)(zombie->head->GetUserData());
		ud->mSprite->removeFromParentAndCleanup(true);
		mLayer->removeChild(ud->mSprite);
		delete ud;
		world->DestroyBody(zombie->head);
	}
	if(!zombie->isAttackLess)
	{
		UserDataInBody* ud = (UserDataInBody*)zombie->attackBody->GetUserData();
		delete ud;
		world->DestroyBody(zombie->attackBody);
	}
	
	zombie->release();
	delete zombie;
	zombie = NULL;
	ud = NULL;
}


int ZombieManager::getSize()
{
	return zombies.size();
}

void ZombieManager::setGenTime(float max,float min)
{
	genTimer.setGenTimeRand(max,min);
}

ZombieManager::~ZombieManager(void)
{
	Camera::sharedCameraInstance()->remove(this);
	//zrm.release();
	list<Zombie*>::iterator it;
	for(it=zombies.begin(); it!=zombies.end(); it++)
		deadZombies.push_back(*it);

	for(it = deadZombies.begin();it != deadZombies.end(); it++)
		deleteZombie(*it);

	zombies.clear();
	deadZombies.clear();

	if(hugeZombie) 
	{
		deleteZombie(hugeZombie);
		SoundManager::getInstance()->stopEffect(hugeZombieSoundId);
	}
}
