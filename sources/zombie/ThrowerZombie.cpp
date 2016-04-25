#include "ThrowerZombie.h"
#include "balancing_const/BALANCING_THROW_ZOMBIE.h"

ThrowerZombie::ThrowerZombie(){}
ThrowerZombie::~ThrowerZombie(){
}
void ThrowerZombie::release()
{
	stoneWeapon->release();
	delete stoneWeapon;

	Zombie::release();
}
ThrowerZombie::ThrowerZombie(b2World* _world, direction_enum _direction,CCLayer* _layer,float32 sX,float32 sY)
	:Zombie(_world,_direction,_layer,sX,sY)
{

	float _attackRange = THROWER_ATTACK_RANGE;
	_attackRange -= rand()%30;

	setTypeStat(
		thrower,
		THROWER_SPEED,
		THROWER_MAX_HP,
		THROWER_GET_SCORE,
		THROWER_DAMAGE,
		_attackRange,
		THROWER_ATTACK_SPEED,
		THROWER_GEN_ENERGE);

	ZombieResourceManager* zrm = ZombieResourceManager::getInstance();

	characterBodyInit(
		getOneTextureSize(zrm->throwZ_walkAnimTec,zrm->throwZ_walkAnimSize),
		CCSize(27,50),
		ccp(51,69),
		this,
		ZORDER_ZOMBIE,
		createAnim(zrm->throwZ_walkAnimTec,zrm->throwZ_walkAnimSize,0.1f,-1),
		TYPE_ZOMBIE
		);
	headBodyInit(CCSprite::createWithTexture(zrm->throwZ_headTec),36.0f/2,ccp(32,32)) ;
	attackBodyInit(createAnim(zrm->throwZ_skillAnimTec,zrm->throwZ_skillAnimSize,0.2f,-1),CCSize(24,31),
		ccp(23,72),3);
	deadAnim = createAnim(zrm->throwZ_deadAnimTec,zrm->throwZ_deadAnimSize,0.15f,1);
	skillAnim = createAnim(zrm->throwZ_walkAnimTec,zrm->throwZ_walkAnimSize,0.1f,-1);
	headShotAnim = createAnim(zrm->throwZ_headShotAnimTec,zrm->throwZ_headShotAnimSize,0.15f,1); 
	//b2Vec2(sX,sY)
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("weapon/bullet/stone.png");
	
	WeaponBuilder* wbStone = new WeaponBuilder(texture,damage,50,b2Vec2(0,0),50,5000,10000);
	stoneWeapon = new ThrowingStone(_world,_layer,wbStone
		->setFireTime(0)
		->setInterSection(THROWER_ATTACK_RANGE)
		->setZOrder(ZORDER_BULLET));

	delete wbStone;
	wbStone = NULL;

	skillCoolTime = THROWER_SKILL_COOLTIME;
	skillRange = attackRange-rand()%150-10;
	skillDistance = 0;
}

void ThrowerZombie::subUpdate(float dt,b2Vec2 pv)
{
	stoneWeapon->update(Timer::getDt());
	stoneWeapon->updateFirePos(this->getPosition());
}

void ThrowerZombie::attack()
{
	if(!isAlreadyAttack)
	{
		SoundManager::getInstance()->playEffect("music/zombie/throw_zombie.ogg");
		SoundManager::getInstance()->playEffect("music/zombie/mon_zombi_attack_voice.ogg");

		stoneWeapon->fire(shootTarget);
		isAlreadyAttack = true;
	}
}

void ThrowerZombie::skillStart()
{
	skillDistance = 0;
	if(zDirection == left)
		directionChange(right);
	else
		directionChange(left);
}

void ThrowerZombie::skill()
{
	
	skillDistance += speed;

	if(skillDistance>=skillRange)
	{
		skillEnd();
		return;
	}
	walk(Timer::getDt());

	
}

bool ThrowerZombie::checkSkill()
{
	float distance = (vTarget-getPosition()).Length()*PTM_RATIO;
	
	if(distance<skillRange)
	{
		return true;
	}
	
	
	return false;
}

void ThrowerZombie::cameraMoved(float dX)
{
	if(currentState==attacking)
	{
		shootTarget.x = shootTarget.x+dX;
	}
}