#include "Weapon.h"
WeaponBuilder* WeaponBuilder::setZOrder(int zOrder)
{
	this->zOrder = zOrder;
	return this;
}
WeaponBuilder* WeaponBuilder::setRandomVariable(float randomVariable)
{
	this->randomVariable = randomVariable;
	return this;
}
WeaponBuilder* WeaponBuilder::setInterSection(float intersection)
{
	this->intersection = intersection;
	return this;
}
WeaponBuilder* WeaponBuilder::setBulletNum(int bulletNum)
{
	this->bulletNum = bulletNum;
	return this;
}
WeaponBuilder* WeaponBuilder::setFireTime(unsigned long fireTime)
{
	this->fireTime = fireTime;
	return this;
}
WeaponBuilder* WeaponBuilder::setSoundDelay(float soundDelay)
{
	this->soundDelay = soundDelay;
	return this;
}
WeaponBuilder* WeaponBuilder::setAnimationCount(int aniCount)
{
	this->aniCount = aniCount;
	return this;
}

WeaponBuilder* WeaponBuilder::setLeftHand(CCSprite* left,CCPoint weaponPoint,CCPoint shoulderPoint,float cHeight)
{
	leftHand = left;
	leftHand->retain();

	leftWPoint = weaponPoint;
	leftSPoint = shoulderPoint;

	leftWPoint.x = leftWPoint.x;
	leftWPoint.y = leftHand->getContentSize().height-leftWPoint.y;
	
	leftSPoint.x = leftSPoint.x;
	leftSPoint.y = cHeight-leftSPoint.y;

	return this;
}

WeaponBuilder* WeaponBuilder::setRightHand(CCSprite* right,CCPoint weaponPoint,CCPoint shoulderPoint,float cHeight)
{
	// left핸드가 기본(양손무기도 left로 처리함.) right는 한손무기에만.
	rightHand = right;
	rightHand->retain();

	rightWPoint = weaponPoint;
	rightSPoint = shoulderPoint;

	rightWPoint.x = rightWPoint.x;
	rightWPoint.y = rightHand->getContentSize().height-rightWPoint.y;

	rightSPoint.x = rightSPoint.x;
	rightSPoint.y = cHeight-rightSPoint.y;

	return this;
}
WeaponBuilder* WeaponBuilder::setHeadShotDamage(int headshot_damage)
{
	this->headshot_damage = headshot_damage;
	return this;
}
CCSprite* Weapon::getLeftHand()
{
	return leftHand;
}
CCSprite* Weapon::getRightHand()
{
	return rightHand;
}
void Weapon::fire(b2Vec2 vTarget)
{
	if(maxRemainBulletCnt <= 0 && currentRemainBulletCnt <= 0)
	{
		reloading = true;
		return;
	}
	if(!isFire)
	{
		return;
	}
	if(currentRemainBulletCnt <= 0)
	{
		reloading = true;
		return;
	}

	fireBullet(vTarget);

	//tv로 bulletSpeed만큼의 속도로 나감.
	isFire = false;

	currentRemainBulletCnt--;
}

NormalGun::NormalGun(b2World* world,CCLayer* target,WeaponBuilder* wb)
	:Weapon(world,target,wb)
{
}
void NormalGun::release()
{
	Weapon::release();
}
//virtual 
void NormalGun::fireBullet(b2Vec2 vTarget)
{
	bullets.push_back(new Bullet(world,vFirePos,vTarget,bulletSpeed,damage,headshot_damage,piercing,bulletTexture,aniCount));
	CCSprite* spr = ((UserDataInBody*)bullets.back()->body->GetUserData())->mSprite;
	spr->setVisible(true);
	targetLayer->addChild(spr,zOrder);
}
void Weapon::moveByCamera(float dX)
{
	list<AttackingObject*>::iterator it;
	for(it = bullets.begin();it != bullets.end();it++)
	{
		float bx = (*it)->body->GetPosition().x;
		float by = (*it)->body->GetPosition().y;

		(*it)->body->SetTransform(b2Vec2(bx+dX/PTM_RATIO,by),(*it)->body->GetAngle());
	}
}

ShotGun::ShotGun(b2World* world,CCLayer* target,WeaponBuilder* wb)
	:NormalGun(world,target,wb)
{
	bulletNum = wb->getBulletNum();
	randomVariable = wb->getRandomVariable();
}
void ShotGun::fireBullet(b2Vec2 vTarget)
{
	for(int i=0;i<bulletNum;i++)
	{
		float random = rand()%((int)(randomVariable*0.5*100));
		
		int isPlus = rand() % 2;
		
		if(isPlus == 0)
			isPlus = -1;
		else
			isPlus = 1;
		
		AttackingObject* bullet = new Bullet(world,vFirePos,vTarget,bulletSpeed,damage,headshot_damage,piercing,bulletTexture,isPlus*random/100.0f,aniCount);

		CCSprite* spr = ((UserDataInBody*)bullet->body->GetUserData())->mSprite;
		spr->setVisible(true);
		targetLayer->addChild(spr,zOrder);

		bullets.push_back(bullet);
	}	//tv로 bulletSpeed만큼의 속도로 나감.
}

FireGun::FireGun(b2World* world,CCLayer* target,WeaponBuilder* wb)
	:ShotGun(world,target,wb)
{

}
void FireGun::fireBullet(b2Vec2 vTarget)
{
	for(int i=0;i<bulletNum;i++)
	{
		float random = rand()%((int)(randomVariable*0.5*100));
		
		int isPlus = rand() % 2;
		
		if(isPlus == 0)
			isPlus = -1;
		else
			isPlus = 1;
		
		AttackingObject* bullet = new Fire(world,vFirePos,vTarget,bulletSpeed,damage,piercing,bulletTexture,isPlus*random/100.0f,aniCount);

		CCSprite* spr = ((UserDataInBody*)bullet->body->GetUserData())->mSprite;
		spr->setVisible(true);
		targetLayer->addChild(spr,zOrder);

		bullets.push_back(bullet);
	}	//tv로 bulletSpeed만큼의 속도로 나감.
}
ThrowingStone::ThrowingStone(b2World* world,CCLayer* target,WeaponBuilder* wb)
	:Weapon(world,target,wb)
{
	isFire= true;
	this->world = world;
	this->targetLayer = target;
	this->intersection = wb->getIntersection();
	this->fireTime = wb->getFireTime()/1000.0;
	this->lastFireTime = wb->getFireTime()/1000.0;



}
void ThrowingStone::release()
{
	Weapon::release();
}
void ThrowingStone::fireBullet(b2Vec2 vTarget)
{
	float32 radianAngleFromXAxis=3.14/3.0;

	b2Vec2 vUnitProjection;
	if( vFirePos.x -vTarget.x  > 0.00001) // from right to left
	{
		 vUnitProjection.Set(-1,0);	
		 vUnitProjection.x = -cos(-radianAngleFromXAxis);
		 vUnitProjection.y = -sin(-radianAngleFromXAxis);
	}
	else								//from left to right
	{
		vUnitProjection.Set(1,std::sqrt(3.0));	
		vUnitProjection.x = cos(radianAngleFromXAxis);
		vUnitProjection.y = sin(radianAngleFromXAxis);
	}
	


	float32 g = world->GetGravity().y;

	float32 speed=10;
	b2Vec2 vDisplacement = vTarget-vFirePos;
	float32 xDistance = abs(vDisplacement.x);

	//아래 공식은 평지에서 계산 방법

	speed= sqrt(xDistance*(-g) / sin(2*radianAngleFromXAxis) );

	//지형이 있을때 공식
	speed = sqrt(
					g*(vTarget.x-vFirePos.x)*(vTarget.x-vFirePos.x)
								/
					(2*cos(radianAngleFromXAxis)*cos(radianAngleFromXAxis)*(vTarget.y-vFirePos.y-tan(radianAngleFromXAxis)*abs(vFirePos.x-vTarget.x)))
				);


	bullets.push_back(new Stone(world,
		vFirePos,
		vFirePos+vUnitProjection,
		speed,
		damage,
		piercing,bulletTexture,aniCount));

	CCSprite* spr = ((UserDataInBody*)bullets.back()->body->GetUserData())->mSprite;
	spr->setVisible(true);
	targetLayer->addChild(spr,zOrder);
}
void Weapon::addDeadBullet(AttackingObject* attackingObject)
{
	deadBullets.push_back(attackingObject);
}
void Weapon::update(float dt)
{
	if(!isSoundOn())
		soundTimer -= dt;

	this->fireTime -= dt;
	
	if(fireTime <= 0)
	{
		isFire = true;
		fireTime = lastFireTime;
	}
	list<AttackingObject*>::iterator it;
	for(it=bullets.begin(); it!=bullets.end(); it++)
	{
		AttackingObject* bullet = (AttackingObject*)*it;
		b2Body* b = bullet->body;

		CCPoint bulletPos = ccp(b->GetPosition().x*PTM_RATIO,
			b->GetPosition().y*PTM_RATIO);

		float dist = bulletPos.getDistance(CCPoint(vFirePos.x*PTM_RATIO,vFirePos.y*PTM_RATIO));

		CCSprite* spr = ((UserDataInBody*)bullet->body->GetUserData())->mSprite;
		float bodyX = bullet->body->GetPosition().x;
		float bodyY = bullet->body->GetPosition().y;
		spr->setPosition(ccp(bodyX*PTM_RATIO,bodyY*PTM_RATIO));
		spr->setRotation(-1*CC_RADIANS_TO_DEGREES(bullet->body->GetAngle()));

		if(!bullet->isDead)
		{
			if(dist > intersection)
			{
				deadBullets.push_back(bullet);
				continue;
			}
		}
		if(b->GetPosition().x*PTM_RATIO > targetLayer->getContentSize().width
			|| b->GetPosition().x*PTM_RATIO<0
			|| b->GetPosition().y*PTM_RATIO<0  
			|| !b->IsAwake()
			|| bullets.size() > 100
			|| bullet->isDead
			|| bullet->piercing<=0
			){
				deadBullets.push_back(bullet);
		}
	}

	for(it=deadBullets.begin(); it!=deadBullets.end(); it++)
	{
		AttackingObject* bullet = (AttackingObject*)(*it);
		removeBullet(bullet);
	}
	deadBullets.clear();
}
void Weapon::removeBullet(AttackingObject* attackObject)
{
	//spr->stopAllActions();
	//attackObject->bulSpr->removeAllChildrenWithCleanup(true);

	bullets.remove(attackObject);
	attackObject->release();

	delete attackObject;
	attackObject = NULL;
}