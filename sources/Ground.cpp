#include "Ground.h"
#include "UserDataInBody.h"

Ground::~Ground()
{
	if(groundBody != NULL)
	{
		UserDataInBody* ud = (UserDataInBody*)groundBody->GetUserData();
		target->removeChild(ud->mSprite);
		delete ud;
		world->DestroyBody(groundBody);
		groundBody = NULL;
		ud = NULL;
	}
}
Ground::Ground(b2World* world,CCLayer* target,GroundPatternInfo* patternInfo,int baseHeight,int zOrder)
{
	this->zOrder = zOrder;
	this->world = world;
	this->target = target;

	CCSprite* spr = CCSprite::createWithTexture(patternInfo->texture);
	position.x = target->getContentSize().width+spr->getContentSize().width*0.5;
	position.y = (baseHeight-BASEGROUND_DELTA)+spr->getContentSize().height*0.5;
	spr->setPosition(position);

	if(patternInfo->points.empty())
	{
		CC_ASSERT("points Empty.. error");
		return;
	}
	if(patternInfo->texture == NULL)
	{
		CC_ASSERT("texture Empty.. error");
		return;
	}

	this->points = patternInfo->points;

	b2BodyDef groundDef;
	groundDef.type = b2_kinematicBody;
	UserDataInBody* ud = new UserDataInBody();
	ud->mChr = this;
	ud->mSprite = spr;
	ud->mType = TYPE_GROUND;

	groundDef.userData = ud;

	target->addChild(spr,zOrder);

	b2Vec2* vs2 = new b2Vec2[patternInfo->points.size()];

	groundBody = world->CreateBody(&groundDef);

	list<CCPoint>::iterator it;
	int i=0;

	for(it = patternInfo->points.begin();it != patternInfo->points.end();it++,i++)
	{
		float trans_x = (*it).x+(getPosition().x-spr->getContentSize().width*0.5);
		float trans_y = (*it).y+(getPosition().y-spr->getContentSize().height*0.5);
		vs2[i].Set((trans_x)/PTM_RATIO,(trans_y)/PTM_RATIO);
	}

	b2ChainShape baseChain;
	baseChain.CreateChain(vs2,patternInfo->points.size());

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &baseChain;

	fixtureDef.density = 1.0f;
	fixtureDef.filter.categoryBits = CATEGORY_GROUND;
	fixtureDef.filter.maskBits = CATEGORY_BULLET | CATEGORY_ARCHITECT |
		CATEGORY_ZOMBIE | CATEGORY_FALLING_ZOMBIE;
	fixtureDef.friction = 1.0f;

	groundBody->CreateFixture(&fixtureDef);
}
CCPoint Ground::getPosition()
{
	return position;
}
float Ground::getWidth()
{
	UserDataInBody* ud = (UserDataInBody*)groundBody->GetUserData();
	return ud->mSprite->getContentSize().width;
}
float Ground::getHeight()
{
	UserDataInBody* ud = (UserDataInBody*)groundBody->GetUserData();
	return ud->mSprite->getContentSize().height;
}

//true �����ϸ� �����ؾߵǰ�, �ƴϸ� false. overridding �Ѱ� �ƴ�!
bool Ground::moveByCamera(float dX)
{
	position.x += dX;

	b2Vec2 pos = groundBody->GetPosition();
	groundBody->SetTransform(b2Vec2(pos.x+dX/PTM_RATIO,pos.y),groundBody->GetAngle());

	UserDataInBody* ud = (UserDataInBody*)groundBody->GetUserData();
	ud->mSprite->setPosition(ccp(ud->mSprite->getPositionX()+dX,ud->mSprite->getPositionY()));

	if(position.x <= -1*getWidth()*0.5)
	{
		delete this;
		return true;
	}
	return false;
}
BaseGroundManager::~BaseGroundManager()
{
	if(baseGroundBody != NULL)
	{
		UserDataInBody* ud = (UserDataInBody*) baseGroundBody->GetUserData();
		delete ud;
		baseGroundBody->GetWorld()->DestroyBody(baseGroundBody);
	}
}
BaseGroundManager::BaseGroundManager(b2World* world,float baseHeight)
{
	this->baseHeight = baseHeight;
	b2BodyDef baseDef;
	baseDef.type = b2_kinematicBody;

	UserDataInBody *ud = new UserDataInBody();

	ud->mChr=this;
	ud->mSprite =NULL;
	ud->mType = TYPE_GROUND;
	baseDef.userData = ud;

	b2Vec2 vs2[2];
	baseGroundBody = world->CreateBody(&baseDef);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	vs2[0].Set(-winSize.width/PTM_RATIO,(baseHeight-BASEGROUND_DELTA)/PTM_RATIO);
	vs2[1].Set(winSize.width*2/PTM_RATIO,(baseHeight-BASEGROUND_DELTA)/PTM_RATIO);

	b2ChainShape baseChain;
	baseChain.CreateChain(vs2,2);
	b2FixtureDef fixtureDef;

	fixtureDef.shape = &baseChain;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;

	fixtureDef.filter.categoryBits = CATEGORY_GROUND;
	fixtureDef.filter.maskBits = CATEGORY_BULLET | CATEGORY_ARCHITECT | CATEGORY_ZOMBIE |
		CATEGORY_FALLING_ZOMBIE;
	baseGroundBody->CreateFixture(&fixtureDef);
}
void GroundManager::addRandomObjectToGrounds() 
{
	if(groundPatternInfos.empty())
		return;

	int random = randForCreate.getCompleteRand();

	Ground *g = new Ground(world,target,groundPatternInfos.at(random),
		baseGroundManager->getBaseHeight(),zOrder);

	showingGrounds.push_back(g);
}
void GroundManager::setGenTime(float max,float min)
{
	randTimer.setGenTimeRand(max,min);
}
//������ ������ �÷��� �� ���� �Ǵ��Ѵ�.
bool GroundManager::createCheck() 
{
	if(showingGrounds.empty())
	{
		if(randTimer.isTimeOver())
		{
			randTimer.updateRandClock();
			return true;
		}
		
		randTimer.clocking();

		return false;
	}
	float backWidth = showingGrounds.back()->getWidth();

	if(showingGrounds.back()->getPosition().x + 
		backWidth*0.5 
		< target->getContentSize().width)
	{
		if(randTimer.isTimeOver())
		{
			randTimer.updateRandClock();
			return true;
		}
		randTimer.clocking();
	}
	return false;
}
GroundManager::~GroundManager()
{
	delete baseGroundManager;
	vector<GroundPatternInfo*>::iterator it;
	for(it = groundPatternInfos.begin();it!=groundPatternInfos.end();it++)
		delete (*it);

	groundPatternInfos.clear();

	list<Ground*>::iterator it2;

	for(it2 = showingGrounds.begin();it2 != showingGrounds.end();it2++)
		delete (*it2);

	showingGrounds.clear();
	patternPoints.clear();
}
GroundManager::GroundManager(CCLayer *target,BaseGroundManager* baseGroundManager,b2World *_world) 
{
	this->target = target;
	this->world = _world;

	this->baseGroundManager = baseGroundManager;

	//lastGround = NULL;
	randTimer.setGenTimeRand(11,10);

	zOrder = 0;
	genTypeMax = 0;
	genTypeMin = 0;
}
//������ �����Ǵ� ������ �����Ѵ�.

//���������� �ϳ� �߰��Ѵ�.ù��° ���ڴ� �����̹����� src,�ι������ڴ� ���� ������ src�̴�.
void GroundManager::addInGroundPatterns(const char* ImgSrc,const char* txtSrc) 
{
	GroundPatternInfo* gpi = new GroundPatternInfo();
	gpi->loadPoints(txtSrc);
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(ImgSrc);
	gpi->texture = texture;
	groundPatternInfos.push_back(gpi);
}


//xPos�� �϶� ������ y��ǥ�� ��ȯ�Ѵ�.
float GroundManager::getYGroundPos(float xPos)
{
	//CCPoint direction = getDirectionInGround(xPos);
	float yPos = baseGroundManager->getBaseHeight()-BASEGROUND_DELTA;
	if(!itIsInPattern(xPos)) return yPos;

	CCPoint v;
	list<Ground*>::iterator it;
	CCPoint direction;

	for(it = showingGrounds.begin();it != showingGrounds.end();it++)
	{
		if((*it)->getPosition().x-(*it)->getWidth()*0.5 <= xPos &&
			(*it)->getPosition().x+(*it)->getWidth()*0.5 >= xPos) //��� ���Ͽ� ���� �ִ��� �˻�.
		{
			CCPoint patternPos = (*it)->getPosition();
			float trans_x = xPos-(patternPos.x-(*it)->getWidth()*0.5);//��ȯ �� x��ǥ

			list<CCPoint>::iterator point_it;
			CCPoint before_point = (*it)->points.front();

			for(point_it = (*it)->points.begin(); point_it != (*it)->points.end(); point_it++)
			{
				if((*point_it).x > trans_x)
				{
					float x2 = (*point_it).x-before_point.x;
					float x1 = trans_x-(int)before_point.x;
					float y2 = (*point_it).y-before_point.y;

					float y1;
					if(x1 != 0) y1 = x1*y2/x2;
					else y1 = 0; 

					yPos += y1+before_point.y;
					break;
				}
				before_point = (*point_it);
			}
			break;
		}
	}

	return yPos;
}
//���ϴ� ��ü�� x��ǥ�� �ѱ�� �� ��ü�� ������ ���� delta��ŭ �̵��Ѵٰ� �������� ���� ���� ��ȯ�Ѵ�.
//delta���� -�� �������� �̵��Ѵٰ� �������� ���Ͱ��� ��ȯ�Ѵ�.
CCPoint GroundManager::getMoveVecFromGround(float objX,int delta)
{
	if(delta == 0) return ccp(0,0);
	
	if(showingGrounds.empty()) return ccp(delta,0);

	if(!itIsInPattern(objX) && !itIsInPattern(objX+delta)) 
		return ccp(delta,0);

	CCPoint objPos = ccp(objX,getYGroundPos(objX));
	CCPoint v;

	list<Ground*>::iterator it;
	CCPoint direction;
	float sumDelta = (float)abs(delta);

	for(it = showingGrounds.begin();it != showingGrounds.end();it++)
	{
		list<CCPoint>::iterator point_it;

		for(point_it = (*it)->points.begin(); point_it != (*it)->points.end(); point_it++)
		{
			patternPoints.push_back(ccp((*point_it).x+((*it)->getPosition().x-(*it)->getWidth()*0.5),
				(*point_it).y+((*it)->getPosition().y-(*it)->getHeight()*0.5)));
		}
	}

	patternPoints.push_front(ccp(patternPoints.front().x-abs(delta)-0.0001,patternPoints.front().y));
	patternPoints.push_back(ccp(patternPoints.back().x+abs(delta)+0.0001,patternPoints.back().y));

	list<CCPoint>::iterator point_it;

	CCPoint before_point = objPos;

	if(delta > 0)
	{
		for(point_it = patternPoints.begin(); point_it != patternPoints.end(); point_it++)
		{
			if((*point_it).x >= objPos.x) //��ȯ�� x��ǥ�� ���� ��ǥ���� �۾����� ������ sum�� �����Ѵ�.
			{
				CCPoint d = (*point_it) - before_point;

				if(sumDelta-d.getLength() <= 0)
				{
					v = v + d.normalize()*sumDelta;
					break;
				}
				else
				{
					v = v + d;
				}
				sumDelta -= d.getLength();

				before_point = (*point_it);
			}
		}
	}
	else
	{
		list<CCPoint>::reverse_iterator point_it;

		for(point_it = patternPoints.rbegin();point_it!=patternPoints.rend();point_it++)
		{
			if((*point_it).x <= objPos.x)
			{
				CCPoint d = before_point - (*point_it);

				if(sumDelta-d.getLength() <= 0)
				{
					v = v + -d.normalize()*sumDelta;
					break;
				}
				else
					v = v-d;
				sumDelta -= d.getLength();

				before_point = (*point_it);
			}
		}
	}
	patternPoints.clear();

	float check_y = getYGroundPos(objPos.x+v.x);

	if(check_y != objPos.y+v.y)
		v.y = check_y - objPos.y;

	return v;
}
bool GroundManager::itIsInPattern(float x)
{
	list<Ground*>::iterator it;
	bool isInPattern = false;
	for(it = showingGrounds.begin();it != showingGrounds.end();it++)
	{
		float PosX = (*it)->getPosition().x; //for debug
		if((*it)->getPosition().x-(*it)->getWidth()*0.5 <= x &&
			(*it)->getPosition().x+(*it)->getWidth()*0.5 >= x) //��� ���Ͽ� ���� �ִ��� �˻�.
		{
			isInPattern = true;
		}
	}
	return isInPattern;
}

void GroundManager::moveByCamera(float dX)
{
	list<Ground*>::iterator it;
	Ground* deadGround = NULL;

	if(createCheck())
		addRandomObjectToGrounds();

	for(it = showingGrounds.begin();it != showingGrounds.end();it++)
	{
		if((*it)->moveByCamera(dX))
			deadGround = (*it);
	}
	if(deadGround != NULL)
		showingGrounds.remove(deadGround);
}