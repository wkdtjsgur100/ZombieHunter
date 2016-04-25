#include "FlyingZombie.h"
#include "balancing_const/BALANCING_FLY_ZOMBIE.h"

FlyingZombie::FlyingZombie(){}
FlyingZombie::~FlyingZombie(){}
void FlyingZombie::release()
{
	world->DestroyBody(skillBody);
	Zombie::release();
}

FlyingZombie::FlyingZombie(b2World* _world, direction_enum _direction,CCLayer* _layer,float32 sX,float32 sY)
	:Zombie(_world,_direction,_layer,sX,sY+100)
{

	setTypeStat(
		flying,
		FLYING_SPEED,
		FLYING_MAX_HP,
		FLYING_GET_SCORE,
		FLYING_DAMAGE,
		FLYING_ATTACK_RANGE,
		FLYING_ATK_COOLTIME,
		FLYING_GEN_ENERGE);

	ZombieResourceManager* zrm = ZombieResourceManager::getInstance();

	characterBodyInit(
		getOneTextureSize(zrm->flyingZ_walkAnimTec,zrm->flyingZ_walkAnimSize),
		CCSize(33,38),
		ccp(18,43),
		this,
		ZORDER_ZOMBIE,
		createAnim(zrm->flyingZ_walkAnimTec,zrm->flyingZ_walkAnimSize,0.1f,-1),
		TYPE_ZOMBIE
		);
	skillBodyInit(createAnim(zrm->flyingZ_skillAnimTec,zrm->flyingZ_skillAnimSize,0.1f,-1));
	deadAnim = createAnim(zrm->flyingZ_deadAnimTec,zrm->flyingZ_deadAnimSize,0.15f,1);
	headShotAnim = NULL;

	flyHeight = 0.0f;
	direction = 1;
	skillRange = FLYING_ATTACK_RANGE;
	skillCoolTime = FLYING_SKILL_COOLTIME;
	skillSpeed = FLYING_SKILL_SPEED;
	multiplyCon=con=0;

	bindingTime = FLYING_BINDING_TIME;
	bindingDamage = 0;
}
void FlyingZombie::walk(float dt)
{
	Zombie::walk(dt);
	flyHeight += 1;

	if(flyHeight > 50.0f)
	{
		flyHeight = 0.0f;
		direction *= -1;
	}
	zombieMoveFor(b2Vec2(0,(1.0f*direction)/PTM_RATIO));
}
bool FlyingZombie::checkSkill()
{
	float distance = (vTarget-getPosition()).Length()*PTM_RATIO;
	if(distance>skillRange)return false;
	
	CCPoint preVec;
	preVec.setPoint(vTarget.x*PTM_RATIO,vTarget.y*PTM_RATIO);
	CCPoint postVec = getPositionWithCocos();

	if(vTarget.x > getPosition().x)
	{
		preVec = getPositionWithCocos();
		postVec.setPoint(vTarget.x*PTM_RATIO,vTarget.y*PTM_RATIO);
	}
	skillCalculator(preVec,postVec);

	for(float x=preVec.x; x<=postVec.x; x++)
	{
	//	if(getYonLine(x)  < gm->getYGroundPos(x) )
		{
		//	return false;
		}
	}
	return true;
}

void FlyingZombie::skillCalculator(CCPoint a, CCPoint b)
{
	multiplyCon = (b.y-a.y)/(b.x-a.x);
	con = a.y - multiplyCon*a.x;
}

float FlyingZombie::getYonLine(float x)
{
	return multiplyCon*x+con;
}
void FlyingZombie::skillStart()
{
	skillStartTime = Timer::getCurrentTime();
	skillTarget = vTarget;
	SoundManager::getInstance()->playEffect("music/zombie/flying_zombie.ogg");
}
void FlyingZombie::skill()
{
	if(Timer::getCurrentTime()-skillStartTime <= 500 ) return; // 0.5ÃÊ°£ !¶ç¿ì°í ¸ØÃçÀÖ´Ù°¡ µ¹Áø
	float distance = (getPosition()-skillTarget).Length()*PTM_RATIO;
	skillBody->SetActive(true);
	if(distance <=10)
	{
		skillEnd();
		skillBody->SetActive(false);
	}
	zombieMoveTo(skillTarget,skillSpeed);
}

void FlyingZombie::cameraMoved(float dX)
{
	if(currentState == skilling)
	{
		skillTarget.x = skillTarget.x+dX;
	}
}

void FlyingZombie::skillBodyInit(CCAnimate* cam)
{
	isSkillLess = false; // ÇÊ¼ö

	skillAnim = cam;
	b2BodyDef bd;
	b2FixtureDef fd;
	b2PolygonShape box;
	bd.type = b2_dynamicBody;
	bd.gravityScale=0;
	fd.isSensor = true;
	fd.density = 1.0f;
	fd.filter.categoryBits = CATEGORY_ZOMBIE;
	fd.filter.maskBits = CATEGORY_PLAYER;
	fd.shape = &box;
	bd.active=false;

	bd.position = character->GetPosition();
	box.SetAsBox(characterS->getContentSize().width/2/PTM_RATIO,characterS->getContentSize().height/2/PTM_RATIO);

	skillBody = world->CreateBody(&bd);
	skillBody->CreateFixture(&fd);

	UserDataInBody* mData = new UserDataInBody();
	mData->mSprite = NULL;
	mData->mChr = this;
	mData->mType = TYPE_ZOMBIE_SKILL;
	skillBody->SetUserData(mData);
}

void FlyingZombie::attack(){}
bool FlyingZombie::checkAttack(){return false;}
