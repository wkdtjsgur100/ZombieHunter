#include "Zombie.h"

#define PTM_RATIO 32
Zombie::Zombie()
{

}
Zombie::~Zombie()
{
}
void Zombie::release()
{
	if(walkAnim!=NULL)
	{
		walkAnim->stop();
		walkAnim->release();
	}
	if(attackAnim!=NULL && !isAttackLess)
	{
		attackAnim->stop();
		attackAnim->release();
	}
	if(skillAnim!=NULL)
	{
		skillAnim->stop();
		skillAnim->release();
	}
	if(deadAnim!=NULL)
	{
		deadAnim->stop();
		deadAnim->release();
	}
	
	if(characterS!=NULL)
	{
		hpBar = NULL;
		mLayer->removeChild(characterS);
		
	}
}

Zombie::Zombie(b2World* _world, direction_enum _direction,CCLayer* _layer,float32 sX,float32 sY)
	:Character(_world,_layer,sX,sY)
{
	zDirection = baseDirection =  _direction;

	currentState = walking;
	lastAttackTime = 0;
	lastSkillTime = 0;

	isAttackAnimating = false;
	isAlreadyAttack = false;
	isSkillAnimating = false;
	actionManager = CCDirector::sharedDirector()->getActionManager();

	shootTarget.SetZero();
	isAttackLess = true;
	isHeadLess = true;
	isSkillLess = true;
	isInArchitecture = false;
	isHeadAttacked = false;

	vMoveDelta = b2Vec2(0,0);


	attackAnim = NULL;
	skillAnim = NULL;
	headShotAnim = NULL;
}

void Zombie::
	setTypeStat(
	ztype_enum zt, 
	int _speed,
	int _hp, 
	int _score,
	int _damage,
	float _attackRange,
	long _attackSpeed,
	float _genEnerge)
	{
		typeZ = zt;
		speed = _speed;
		hp = maxHp = _hp;
		score = _score;
		damage = _damage;
		attackRange = _attackRange;
		attackSpeed = _attackSpeed;
		genEnerge = _genEnerge;
	}
void Zombie::update(float dt,b2Vec2 pv)
{//기본적인 좀비의 패턴.
	this->vTarget = pv;

	if(currentState==falling)	//떨어지고 있으면 걍 떨어지기만 해 딴짓하지말고
		return;
	
	if(currentState==dying || currentState == dead) //죽었으면 처리해줌
	{
		if(characterS->getActionByTag(TAG_CURRENT_ANIM)==NULL ) //지정한 액션(애니메이션)의 loop가 끝나면 release해서 NULL로 만드는듯.. 딥빡! 일단 임시방편.
		{
			if(currentState==dead) return;

			dyingAnimating();
		}
		return; 
	}
	

	if(currentState==skilling)
	{
		skill();
		return;
	}
	if(!isAttackAnimating)	directionUpdate();
	if(currentState==walking) walk(dt);
	if(!checkSkillCoolTime() && checkSkill() && !isAttackAnimating	) // 스킬쓸수있냐해!
	{
		if(currentState!=skilling)
			changeState(skilling);
	}
	else
	{
		if(!checkAttack() && !isAttackAnimating ) 		
		{
			if(currentState!=walking ) //이미 워킹중이면 또 할필요 없다해 
				changeState(walking); //아니면 걸어다니셈
		}
		else
		{
			attackAnimating();
		}
	}
	// 상황 판단의 근거 -> 플레이어와 자신.

	subUpdate(dt,pv); // 각 좀비별 업데이트
}
void Zombie::directionUpdate()
{
	if(!isInArchitecture) //구조물 안에 있으면 타겟을 향해 걷지 않음.
	{
		if(vTarget.x <= getPosition().x) directionChange(Zombie::left);
		else directionChange(Zombie::right);
	}
}
void Zombie::changeState(zState_enum _state)
{
	CCAnimate* cam = NULL;
	int zOrder = ZORDER_ZOMBIE;

	if(_state ==walking)
	{
		cam = walkAnim;
		if(cam==NULL)return;
	}
	else if(_state == attacking)
	{
		if(Timer::getCurrentTime() - lastAttackTime <  attackSpeed )
		{
			changeState(standing);
			return;
		}
		cam = attackAnim;
		if(cam==NULL)return;
		isAttackAnimating = true;
		shootTarget = vTarget;
	}
	else if(_state == skilling)
	{
		cam = skillAnim;
		if(cam==NULL)return;
		isSkillAnimating = true;
		skillStart();
	}
	else if(_state ==dying)
	{
		if(isHeadAttacked)
		{
			cam = headShotAnim;
			if(headShotAnim == NULL)
				cam = deadAnim;
		}
		else
		{
			cam = deadAnim;
			if(deadAnim == NULL)
				cam = headShotAnim;
		}
		if(cam==NULL)return;
		characterS->setZOrder(ZORDER_DEADZOMBIE);
		hpBar->setVisible(false);
		if(!isAttackLess)attackBody->SetActive(false);
		if(!isSkillLess)skillBody->SetActive(false);
	}

	else if(_state == dead)
	{
		currentState = _state;
		return;
	}
	else if(_state == falling)
	{
		currentState = _state;
		return;
	}
	else if(_state == standing)
	{
		currentState = _state;
		//characterS->stopAllActions();
		return;
	}

	actionManager->pauseTarget(characterS->getActionByTag(TAG_CURRENT_ANIM));
	actionManager->resumeTarget(cam);

	currentState = _state;
	characterS->stopAllActions();
	characterS->runAction(cam);
}
void Zombie::updateMoveDelta(b2Vec2 vMoveDelta)
{
	this->vMoveDelta =vMoveDelta;
}
void Zombie::walk(float dt)
{
	if( currentState!=skilling && !isInArchitecture) //구조물 안에 있으면 타겟을 향해 걷지 않음.
	{
		float afterX = getPosition().x + vMoveDelta.x; // 왔다리 갔다리 방지 
		if((afterX<vTarget.x&&zDirection == left)||
			(afterX>vTarget.x&&zDirection == right)) return ;
	}
	zombieMoveFor(vMoveDelta);
}

void Zombie::zombieMoveFor(b2Vec2 vec)
{
	moveFor(vec);
	if(!isAttackLess) attackBody->SetTransform(attackBody->GetPosition()+vec,attackBody->GetAngle());
	if(!isHeadLess) head->SetTransform(head->GetPosition()+vec,head->GetAngle());
	if(!isSkillLess)skillBody->SetTransform(skillBody->GetPosition()+vec,skillBody->GetAngle());
}
void Zombie::zombieMoveTo(b2Vec2 toVec,int _speed)
{
	b2Vec2 normalVec = ( toVec-getPosition());
	normalVec.Normalize();
	normalVec.x*=((float)_speed/PTM_RATIO);
	normalVec.y*=((float)_speed/PTM_RATIO);
	zombieMoveFor(normalVec);
}

void Zombie::dyingAnimating()
{
	if(characterS->getOpacity()<=0)changeState(dead);

	if(characterS->getOpacity()-5 <0 )
		characterS->setOpacity(0);
	else
		characterS->setOpacity(characterS->getOpacity()-5);

	setPositionWithCocos(getPositionWithCocos() + ccp(0,-2));
}

void Zombie::die(bool isHeadAttacked,b2Vec2 force)
{
	if(currentState!=dying && currentState!=dead)
	{
		this->isHeadAttacked = isHeadAttacked;
	
		ScoreManager::sharedInstance()->point += this->score;

		LabelEffectInfo ei;
	
		EffectManager::getInstance()->moveLabelEffect(CCString::createWithFormat("+%d",this->score)->m_sString,
		this->getPositionWithCocos(),ei);

		SoundManager::getInstance()->playEffect("music/zombie/mon_zombi_death.ogg");
		SoundManager::getInstance()->playEffect("music/zombie/mon_zombi_death_voice.ogg");

		changeState(dying);
	
		//deadAnim = 걍죽음;
		if(isHeadAttacked)
		{
			headAttacked(force);
			if(headShotAnim !=NULL)	
				deadAnim = headShotAnim;
		}

	}
}

bool Zombie::checkAttack()
{
	b2Vec2 v = b2Vec2(getPosition().x*PTM_RATIO , getPosition().y*PTM_RATIO) -b2Vec2(vTarget.x*PTM_RATIO,vTarget.y*PTM_RATIO);
	if(v.Length() < attackRange )
	{
		if(currentState==attacking)
		{
			//이미 어택하고있음 상태변경 할필요 없지
		}
		else
		{
			changeState(attacking);
		}
		return true;
	}
	else
		return false;


	return false;
}

void Zombie::attackAnimating()
{
	//공격!
	if(attackAnim==NULL)return;

	CCAnimation* cam;
	cam = attackAnim->getAnimation();
	
	if(isAttackAnimating)
	{
		if(attackAnim->getElapsed() >= cam->getDuration())
		{//Animation이끝날 시간이 됬다.
			characterS->stopAllActions();
			isAttackAnimating = false;
			isAlreadyAttack = false;
			lastAttackTime = Timer::getCurrentTime();
		}
		else //Animation 진행중
		{
			if(attackAnim->getElapsed() >= cam->getDuration()* attackStartFrameRatio ) // 진행중의 어느 시점, 공격해야 할 때가 올걸.
			{
				if(attackAnim->getElapsed() >= cam->getDuration()*attackEndFrameRatio) // 
					isAlreadyAttack = true; // attackEnd시에는 '이미 어택완료'체크를 해주자.

				attack(); //공격함. 오버라이드해서 다양한 경우에 쓰셈.
			}
		}	
	}
	else // Animation이 움직이고 있지 않을때. attackSpeed 후에 움직인다.
	{
		unsigned long currentTime = Timer::getCurrentTime();
		if(currentTime - lastAttackTime <  attackSpeed )return;

		isAttackAnimating = true;
		characterS->runAction(attackAnim);
		shootTarget = vTarget;
	}
} 

void Zombie::attack()
{
	//isAlreadyAttack이 false로되있는동안은 계속 충돌검사.
	if(isAlreadyAttack)
	{
		attackBody->SetActive(false);
	}
	else
	{
		attackBody->SetActive(true);	
	}
}
bool Zombie::checkSkill()
{
	return false;
}

bool Zombie::checkSkillCoolTime()
{
	if(skillAnim==NULL)return true;

	if(Timer::getCurrentTime() - lastSkillTime >= skillCoolTime)
		return false;
	return true;

}
void Zombie::skillStart(){}
void Zombie::skill(){}
void Zombie::skillEnd()
{
	isSkillAnimating = false;
	changeState(walking);
	lastSkillTime = Timer::getCurrentTime();
}
bool Zombie::isDeadOnDamage(int _damage)
{
	if(currentState==dead || currentState==dying || hp<=0)return true;

	hp = hp-_damage;
	if(hp<=0)
	{
		return true;
	}
	hpBar->setScaleX(( float)hp/(float)maxHp );
	return false;
}



void Zombie::directionChange(direction_enum zd)
{
	if(zDirection!=zd)
	{
		int mark = 1;
		zDirection = zd;

		if(zd!=baseDirection)
		{
			characterS->setFlipX(true);
			if(!isHeadLess)headS->setFlipX(true);
			mark = -1;
		}
		else
		{
			characterS->setFlipX(false);
			if(!isHeadLess)headS->setFlipX(false);
			mark = 1;
		}
		b2Vec2 plusVec;
		if(!isAttackLess)
		{
			plusVec.Set( mark * centerToAttackVec.x*2 ,0 );
			attackBody->SetTransform(attackBody->GetPosition() + plusVec , 
				attackBody->GetAngle());
		}
		if(!isHeadLess)
		{
			plusVec.Set(mark* centerToHeadVec.x*2,0);
			head->SetTransform(head->GetPosition() + plusVec , head->GetAngle());
		}
		
	}
}
void Zombie::headBodyInit(CCSprite* _headS,float radius,CCPoint headPoint)
{
	isHeadLess = false;
	headS = _headS;

	headPoint.x = headPoint.x/PTM_RATIO;
	headPoint.y = (characterS->getContentSize().height-headPoint.y)/PTM_RATIO;
	radius=radius/PTM_RATIO;

	centerToHeadVec.Set(originX + headPoint.x - startX/PTM_RATIO , startY/PTM_RATIO- headPoint.y+originY);

	b2BodyDef bd;
	b2FixtureDef fd;
	b2CircleShape circle;
	bd.type = b2_dynamicBody;
	bd.gravityScale = 0;
	fd.filter.categoryBits = CATEGORY_ZOMBIE;
	fd.filter.maskBits = CATEGORY_BULLET;
	fd.density=1.0f;
	fd.restitution=0.5f;
	fd.friction = 1.0f;
	fd.isSensor = true;
	
	////////////////////////head
	float hWidth = headS->getContentSize().width;
	float hHight = headS->getContentSize().height;
	mLayer->addChild(headS,zOrder);

	bd.type = b2_dynamicBody;
	bd.position.Set(originX+headPoint.x,originY+headPoint.y);
	head = world->CreateBody(&bd);
	circle.m_radius = radius;
	fd.shape = &circle;
	head->CreateFixture(&fd);
	head->SetBullet(true);
	
	UserDataInBody* udh = new UserDataInBody();
	udh->mSprite = headS;
	udh->mChr = this;
	udh->mType = TYPE_ZOMBIE;

	head->SetUserData(udh);
	headS->setVisible(false); // 개별 머리는 떨어져나갈때만 보임
}

void Zombie::attackBodyInit(CCSize attackBoxSize,CCPoint attackPoint,bool isActive)
{
	isAttackLess = false;
	if(attackRange==0)//밀리는 자기박스크기/2만큼 attackRange를 가지게하자
	{
		float characterWidth = characterS->getContentSize().width;
		attackRange = (float)characterWidth/4;

	}

	b2BodyDef bd;
	b2FixtureDef fd;
	b2PolygonShape box;
	bd.type = b2_dynamicBody;
	fd.isSensor = true;
	fd.density = 1.0f;
	bd.gravityScale = 0;
	///attack
	attackBoxSize.width = attackBoxSize.width/PTM_RATIO;
	attackBoxSize.height = attackBoxSize.height/PTM_RATIO;

	attackPoint.x = attackPoint.x/PTM_RATIO;
	attackPoint.y = (characterS->getContentSize().height-attackPoint.y)/PTM_RATIO;
	centerToAttackVec.Set(originX+attackPoint.x - startX/PTM_RATIO, startY/PTM_RATIO - attackPoint.y+originY);

	box.SetAsBox(attackBoxSize.width,attackBoxSize.height);
	bd.active = isActive;
	bd.type = b2_dynamicBody;
	bd.position.Set(originX+attackPoint.x , originY+attackPoint.y);
	fd.filter.categoryBits = CATEGORY_ZOMBIE;
	fd.filter.maskBits = CATEGORY_PLAYER;
	fd.shape = &box;
	attackBody = world->CreateBody(&bd);
	attackBody->CreateFixture(&fd);

	UserDataInBody* uda = new UserDataInBody();
	uda->mSprite = NULL;
	uda->mChr = this;
	uda->mType = TYPE_ZOMBIE_ATTACK;

	attackBody->SetUserData(uda);

}

void Zombie::attackBodyInit(CCAnimate* _attackAnim,CCSize attackBoxSize,CCPoint attackPoint,int attackFrame)
{
	isAttackLess = false;
	attackAnim  = _attackAnim;

	if(attackRange==0)//밀리는 자기박스크기/2만큼 attackRange를 가지게하자
	{
		float characterWidth = characterS->getContentSize().width;
		attackRange = (float)characterWidth/4;

	}
	/////////////base
	b2BodyDef bd;
	b2FixtureDef fd;
	b2PolygonShape box;
	bd.type = b2_dynamicBody;
	fd.isSensor = true;
	fd.density = 1.0f;
	bd.gravityScale = 0;
	///attack
	attackBoxSize.width = attackBoxSize.width/PTM_RATIO;
	attackBoxSize.height = attackBoxSize.height/PTM_RATIO;

	attackPoint.x = attackPoint.x/PTM_RATIO;
	attackPoint.y = (characterS->getContentSize().height-attackPoint.y)/PTM_RATIO;
	centerToAttackVec.Set(originX+attackPoint.x - startX/PTM_RATIO, startY/PTM_RATIO - attackPoint.y+originY);

	box.SetAsBox(attackBoxSize.width,attackBoxSize.height);
	bd.active = false;
	bd.type = b2_dynamicBody;
	bd.position.Set(originX+attackPoint.x , originY+attackPoint.y);
	fd.filter.categoryBits = CATEGORY_ZOMBIE;
	fd.filter.maskBits = CATEGORY_PLAYER;
	fd.shape = &box;
	attackBody = world->CreateBody(&bd);
	attackBody->CreateFixture(&fd);

	UserDataInBody* uda = new UserDataInBody();
	uda->mSprite = NULL;
	uda->mChr = this;
	uda->mType = TYPE_ZOMBIE_ATTACK;

	attackBody->SetUserData(uda);

	CCAnimation* ca = attackAnim->getAnimation();
	attackStartFrameRatio =  ( (float)attackFrame ) / ca->getTotalDelayUnits();
	attackEndFrameRatio = ((float)(attackFrame+1))/ca->getTotalDelayUnits();

}
void Zombie::headAttacked(b2Vec2 force)
{
	headS->setVisible(true);
	head->SetGravityScale(1.0f);

	headS->setZOrder(ZORDER_DEADZOMBIE+1);

	b2Filter f;
	f.categoryBits = CATEGORY_ZOMBIE;
	f.maskBits = CATEGORY_GROUND;

	character->GetFixtureList()->SetFilterData(f);
	head->GetFixtureList()->SetFilterData(f);//충돌 x
	head->GetFixtureList()->SetSensor(false);
	head->ApplyForce(force,head->GetPosition());
}
void Zombie::cameraMoved(float dX){}
void Zombie::subUpdate(float dt,b2Vec2 pv){}
