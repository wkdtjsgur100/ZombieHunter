#include "AttackingObject.h"

AttackingObject::AttackingObject(b2World* world,b2Vec2 vFirePos,b2Vec2 vTargetPos,
	int bulletSpeed,int dam,int headshot_damage,int _piercing,CCTexture2D* bulletTexture,int aniNum, bool doesBelongToPlayer)
{
	float sprWidth = bulletTexture->getContentSize().width/aniNum;
	float sprHeight = bulletTexture->getContentSize().height;

	bulSpr = CCSprite::createWithTexture(bulletTexture,CCRectMake(0,0,sprWidth,sprHeight));

	b2PolygonShape bulletBox;
	bulletBox.SetAsBox(bulSpr->getContentSize().width*0.5/PTM_RATIO,
		bulSpr->getContentSize().height*0.5/PTM_RATIO);

	b2FixtureDef bulletF;

	bulletF.shape = &bulletBox;
	bulletF.density = 1.0f;
	bulletF.restitution = 0.2f;
	bulletF.friction = 1.0f;
	bulletF.isSensor = true;
	

	UserDataInBody *ud = new UserDataInBody();
	if(doesBelongToPlayer)
	{
		bulletF.filter.categoryBits = CATEGORY_BULLET;
		bulletF.filter.maskBits = CATEGORY_ARCHITECT | CATEGORY_GROUND | CATEGORY_ZOMBIE | CATEGORY_ZOMBIEBULLET;

		ud->mType = TYPE_BULLET; //type = ÃÑ¾Ë
	}
	else
	{
		bulletF.filter.categoryBits = CATEGORY_ZOMBIEBULLET;
		bulletF.filter.maskBits = CATEGORY_ARCHITECT | CATEGORY_GROUND | CATEGORY_PLAYER | CATEGORY_BULLET;
	
		ud->mType = TYPE_ZOMBIEBULLET; //type = ÃÑ¾Ë
	}
	b2BodyDef b2d;
	b2d.type = b2_dynamicBody;
	b2d.bullet = true;
	b2d.position.Set(vFirePos.x,vFirePos.y);

	body = world->CreateBody(&b2d);
	body->CreateFixture(&bulletF);

	b2Vec2 bv(vTargetPos.x -vFirePos.x , vTargetPos.y - vFirePos.y);
	bv.Normalize();
	this->vCurrentDir = bv;

	float angle = atan2f(bv.y, bv.x);

	body->SetTransform(body->GetPosition(),angle);

	bulSpr->setPosition(ccp(vFirePos.x*PTM_RATIO,vFirePos.y*PTM_RATIO));
	bulSpr->setVisible(false);

	ud->mChr = this;
	ud->mSprite = bulSpr; //mWeapon->bulletSpeed

	body->SetUserData(ud);

	damage = dam;
	this->headshot_damage = headshot_damage;
	piercing = _piercing;

	isDead = false;
}
void AttackingObject::release()
{
	UserDataInBody* ud = (UserDataInBody*)body->GetUserData();
	CCSprite* spr = ud->mSprite;
	
	//spr->stopAllActions();
	(spr->getParent())->removeChild(spr);
	spr = NULL;

	(body->GetWorld())->DestroyBody(body); 
	
	list<void*>::iterator it;
	for(it = pierceList.begin();it!= pierceList.end();it++)
	{
		(*it) = NULL;
	}
	pierceList.clear();
	delete ud;
	ud = NULL;
}
void setMaskBits(uint16 maskBits)
{

}

bool AttackingObject::find(void* f)
{
	list<void*>::iterator it;
	for(it=pierceList.begin(); it!=pierceList.end(); it++)
	{
		if(*it==f)
			return true;
	}
	return false;
}

void AttackingObject::minusPiercing()
{
	piercing--;
}
/*
void setMaskBits(uint16 maskBits)
{

}*/