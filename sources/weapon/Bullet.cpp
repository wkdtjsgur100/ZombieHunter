#include "Bullet.h"

Bullet::Bullet(b2World* world,b2Vec2 vFirePos,b2Vec2 vTargetPos,
		int bulletSpeed,int dam,int headshot_damage,int _piercing,CCTexture2D* bulletTexture,float variable,int aniNum)
		: AttackingObject(world,vFirePos,vTargetPos,bulletSpeed,dam,headshot_damage,_piercing,bulletTexture,aniNum)
{
	b2Vec2 bv = b2Vec2(vCurrentDir.x,vCurrentDir.y);

	b2Vec2 newVec;
	newVec.x = bv.x;
	newVec.y = bv.y;

	float r = CC_DEGREES_TO_RADIANS(variable);

	float cs = cos(r);
	float sn = sin(r);

	newVec.x = cs*bv.x - sn*bv.y;
	newVec.y = sn*bv.x + cs*bv.y;

	newVec.x*=bulletSpeed; // ÅºÈ¯ ½ºÇÇµå
	newVec.y*=bulletSpeed;

	body->SetLinearVelocity(newVec);
	body->SetGravityScale(0);
}
void Bullet::release()
{
	AttackingObject::release();
}
Fire::Fire(b2World* world,b2Vec2 vFirePos,b2Vec2 vTargetPos,int bulletSpeed,int dam, int _piercing,
		CCTexture2D* bulletTexture,float variable,int aniNum)
		:Bullet(world,vFirePos,vTargetPos,bulletSpeed,dam,dam,_piercing,bulletTexture,variable,aniNum)
{
	float sprWidth = bulletTexture->getContentSize().width/aniNum;
	float sprHeight = bulletTexture->getContentSize().height;

	CCAnimation *animation = CCAnimation::create();
	
	float theta = body->GetAngle();
	if(theta != 0)
	{
		if(vCurrentDir.y/tanf(theta) < 0)
			body->SetTransform(body->GetPosition(),body->GetAngle() + 3.141592);
	}
	animation->setDelayPerUnit(0.125f);

	for(int i = 0;i<aniNum;i++)
		animation->addSpriteFrameWithTexture(bulletTexture, CCRectMake(i*sprWidth,0, sprWidth, sprHeight));

	CCAnimate *animate =  CCAnimate::create(animation);	
	CCAction * scale = CCScaleTo::create(0.3f,3.0f);

	CCActionInterval * totalAction = CCSpawn::create(animate,scale,NULL);

	CCRepeatForever* rep = CCRepeatForever::create(totalAction);

	UserDataInBody* ud = (UserDataInBody*)body->GetUserData();
	
	(ud->mSprite)->runAction(rep);
}
void Fire::release()
{
	UserDataInBody* ud = (UserDataInBody*)body->GetUserData();
	//(ud->mSprite)->stopAllActions();
	Bullet::release();

}
