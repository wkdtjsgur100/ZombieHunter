#include "Character.h"
Character::Character()
{


}
Character::Character(b2World* _world,CCLayer* _layer,float32 sX,float32 sY)
{
	world = _world;
	mLayer = _layer;
	hp = 1;
	startX = sX;
	startY = sY;
	speed = normalSpeed = 1;

	walkAnim = NULL;

}
Character::~Character()
{
}
void Character::setPosition(b2Vec2 vec)
{
	character->SetTransform(vec,character->GetAngle());
}
void Character::setPositionWithCocos(CCPoint vec)
{
	b2Vec2 mv(vec.x/PTM_RATIO,vec.y/PTM_RATIO);
	setPosition(mv);
}
b2Vec2 Character::getPosition()
{
	return character->GetPosition();
}
CCPoint Character::getPositionWithCocos()
{
	return ccp(character->GetPosition().x*PTM_RATIO,character->GetPosition().y*PTM_RATIO);
}
void Character::moveFor(b2Vec2 forVec)
{
	setPosition(b2Vec2(getPosition().x+forVec.x ,getPosition().y+forVec.y));
	body->SetTransform(body->GetPosition()+forVec,body->GetAngle());
}

void Character::moveTo(b2Vec2 toVec,int _speed)
{
	b2Vec2 normalVec = ( toVec-getPosition());
	normalVec.Normalize();
	normalVec.x*=((float)_speed/PTM_RATIO);
	normalVec.y*=((float)_speed/PTM_RATIO);
	moveFor(normalVec);
}

void Character::characterBodyInit(CCSize contentSize,CCSize bodySize,CCPoint bodyPoint,void* _this, int _zOrder,  CCAnimate* _walkAnim, int _type)
{
	zOrder = _zOrder;
	walkAnim = _walkAnim;

	b2PolygonShape box;

	characterS = CCSprite::create();
	characterS->setContentSize(contentSize);

	characterS->runAction(walkAnim);
	//characterS->retain();
	if(_type == TYPE_HUGE_ZOMBIE)
	{
		CCSprite* zombieS = CCSprite::create("zombie_huge/bulldozer_zombie.png");
		zombieS->setContentSize(getOneTextureSize(zombieS->getTexture(),CCSizeMake(6,2)));
		zombieS->runAction(createAnim(zombieS->getTexture(),CCSizeMake(6,2),0.1f,-1));
		zombieS->setPosition(ccp(50,163));
		zombieS->setAnchorPoint(CCPointZero);

		characterS->addChild(zombieS);
	}
	mLayer->addChild(characterS,zOrder);

	float characterWidth = (float)(characterS->getContentSize().width);
	float characterHeight = (float)(characterS->getContentSize().height);


	/////////////base
	b2BodyDef bd;
	b2FixtureDef fd;
	fd.isSensor = true;
	bd.type = b2_dynamicBody;
	//	fd.filter.groupIndex = -2;
	if(_type == TYPE_PLAYER)
	{
		fd.filter.categoryBits = CATEGORY_PLAYER;
		fd.filter.maskBits = CATEGORY_ZOMBIE|CATEGORY_ZOMBIEBULLET;
		hpBar = CCSprite::create("UI/hp_bar.png",CCRectMake(0,0,CCDirector::sharedDirector()->getWinSize().width,12));
		hpBar->setAnchorPoint(ccp(0.5f,0));
		hpBar->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2,0));
		mLayer->addChild(hpBar);
	}
	else if(_type == TYPE_ZOMBIE )
	{
		fd.filter.categoryBits = CATEGORY_ZOMBIE;
		fd.filter.maskBits = CATEGORY_BULLET;

		hpBar = CCSprite::create("UI/hp_bar.png",CCRectMake(0,0,characterWidth/2,12));
		hpBar->setAnchorPoint(ccp(0.5f,0));
		hpBar->setPosition(ccp(characterWidth/2,characterHeight));
		characterS->addChild(hpBar);
	}

	////////////character
	b2Vec2 characterVec;
	characterVec.Set((startX)/PTM_RATIO , (startY + characterHeight/2)/PTM_RATIO);
	bd.type = b2_kinematicBody;
	bd.gravityScale=0;
	bd.position.Set(characterVec.x,characterVec.y); // 이건 머리몸통 둘다 더한 값의위치
	fd.shape = &box;
	character = world->CreateBody(&bd);
	character->CreateFixture(&fd);


	UserDataInBody* mData = new UserDataInBody();
	mData->mSprite = characterS;
	mData->mChr = _this;
	mData->mType = _type;
	character->SetUserData(mData);
	character->SetActive(false);
	mData = NULL;

	////body
	bd.type = b2_dynamicBody;
	bodyPoint.x = bodyPoint.x/PTM_RATIO;
	bodyPoint.y = (characterHeight-bodyPoint.y)/PTM_RATIO;
	bodySize.width = bodySize.width/PTM_RATIO;
	bodySize.height = bodySize.height/PTM_RATIO;
	bodyVec.Set(bodyPoint.x,bodyPoint.y);
	 
	originX = (startX-characterWidth/2)/PTM_RATIO;
	originY = startY/PTM_RATIO;

	centerToBodyVec.Set(originX+bodyPoint.x-startX/PTM_RATIO , startY/PTM_RATIO-bodyPoint.y+originY);
	
	bd.position.Set(originX+bodyPoint.x,originY+bodyPoint.y);
	box.SetAsBox(bodySize.width/2,bodySize.height/2);
	fd.shape = &box;
	body = world->CreateBody(&bd);
	body->CreateFixture(&fd);
	UserDataInBody* bData = new UserDataInBody();
	bData->mSprite = NULL;
	bData->mChr = _this;
	bData->mType = _type;
	body->SetUserData(bData);
}

CCSize Character::getOneTextureSize(CCTexture2D* tec, CCSize size)
{
	return CCSize(tec->getContentSize().width/size.width,
		tec->getContentSize().height/size.height);
}
CCAnimate* Character::createAnim(CCTexture2D* tec, CCSize size, float speed,int loops)
{
	float wSize = size.width;
	float hSize = size.height;
	float characterWidth = (tec->getContentSize().width)/wSize;
	float characterHeight = (tec->getContentSize().height)/hSize;

	CCSprite* sprite = CCSprite::createWithTexture(tec,CCRectMake(0,0,characterWidth,characterHeight));

	CCAnimation *animation = CCAnimation::create();
	animation->setDelayPerUnit(speed); //애니메이션 interval time 설정.
	for(int i=0; i<hSize; i++)
	{
		for(int j = 0;j<wSize;j++)
			animation->addSpriteFrameWithTexture(tec, CCRectMake(j*characterWidth,characterHeight*i, characterWidth, characterHeight));
	}
	if(loops==-1)loops = 0xFFFFFF;
	animation->setLoops(loops);
	CCAnimate *animate = CCAnimate::create(animation);
	animate->setTag(TAG_CURRENT_ANIM);
	animate->retain();

	return animate;

}
void Character::update(float dt){}