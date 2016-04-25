#include "Architecture.h"

bool Architecture::isIn(float x){
	if(getPosition().x - getWidth()*0.5 <= x &&
		getPosition().x + getWidth()*0.5 >= x)
		return true;
	return false;
}
float Architecture::getFloorHeight(int floorIndex)
{
	if(floorsHeight.empty())
	{
		CC_ASSERT("층이 없는 Architect입니다");
		return -1;
	}
	if(floorIndex >= 0 )
		return floorsHeight.at(floorIndex);
	else
	{
		CC_ASSERT("floorNum은 0보다 같거나 커야함");
		return -1.0f;
	}
}
void Architecture::addFloorHeight(float floorHeight)
{
	floorsHeight.push_back(floorHeight);
}
void Architecture::applyForce(b2Vec2 force,b2Vec2 point)
{	
	point.x *= PTM_RATIO;
	point.y *= PTM_RATIO;
	for(int i=0;i<components.size();i++)
	{
		components[i]->getBody()->ApplyForce(force,point);
	}
}
CCPoint Architecture::getPosition() { return position; }
int Architecture::getWidth(){ return width; }
int Architecture::getHeight(){ return height; }

float Architecture::getHP() { return hp; }
float Architecture::getFullHP() { return fullHp; }

void Architecture::setActiveArch(bool active)
{
	for(int i=0;i<components.size();i++)
		components[i]->getBody()->SetActive(active);
}
void Architecture::generateZombies()
{
	for(int i=0;i<floorsHeight.size();i++)
	{
		int genRandNum = 0;

		if( genZombieNumber.mMax != 0 )
			genRandNum = rand() % (genZombieNumber.mMax - genZombieNumber.mMin + 1) + genZombieNumber.mMin;

		for(int j=0;j<genRandNum;j++)
		{
			int genRandType = 0;

			if(genZombieType.mMax != 0 )
				genRandType = rand() % (genZombieType.mMax - genZombieType.mMin + 1) + genZombieType.mMin;

			int genMin = getPosition().x - getWidth() * 0.5;
			int genMax = getPosition().x + getWidth() * 0.5;

			int sX = rand() % ( genMax - genMin + 1 ) + genMin;

			Zombie* z;

			switch(genRandType)
			{
			case 0: z = new ThrowerZombie(world,Zombie::left,target,sX,floorsHeight[i]+BASE_GROUND_HEIGHT); break;
			case 1: z = new FlyingZombie(world,Zombie::left,target,sX,floorsHeight[i]+BASE_GROUND_HEIGHT); break;
			default : return;
			}
			z->isInArchitecture = true;
			zombieManager->generate(z);
		}
	}
}
void Architecture::attack(float damage)
{
	if(damage < 0)
	{
		CCLog("Damage < 0 .. error");
		return;
	}
	hp -= damage;

	if( isCollapsed ) return;
	if(hp <= 0) //hp가 0이하면
	{
		SoundManager::getInstance()->playEffect("music/architect/crash.ogg");

		for(int i=0;i<components.size();i++)
		{
			b2Filter filter;
			filter.categoryBits = CATEGORY_ARCHITECT;
			filter.maskBits = CATEGORY_GROUND | CATEGORY_ARCHITECT | CATEGORY_FALLING_ZOMBIE;
			CCSprite* spr = ((UserDataInBody*)components[i]->getBody()->GetUserData())->mSprite;
			spr->setZOrder(ZORDER_GROUND+1);
			//spr->setZOrder(components[i]->getZOrder()+6);
			components[i]->getBody()->GetFixtureList()->SetFilterData(filter);
			components[i]->getBody()->SetType(b2_dynamicBody);
		}//무너진다.
		isCollapsed = true;
	}
}

//  원 모양의 바디인 Architecture를 구성할 component를 추가한다.

void Architecture::addCircleComponent(CCTexture2D* texture,float x,float y,int zOrder,int bodyWidthDelta,int bodyHeightDelta)
{
	addComponent(texture,x,y,CIRCLE,zOrder,bodyWidthDelta,bodyHeightDelta);
}

//  박스 모양의 바디인 Architecture를 구성할 component를 추가한다.

void Architecture::addBoxComponent(CCTexture2D* texture,float x,float y,int zOrder,int bodyWidthDelta,int bodyHeightDelta)
{
	addComponent(texture,x,y,BOX,zOrder,bodyWidthDelta,bodyHeightDelta);
}
void Architecture::addPolygonComponent(CCTexture2D* texture,float x,float y,int zOrder,int bodyWidthDelta,int bodyHeightDelta)
{
	addComponent(texture,x,y,POLYGON,zOrder,bodyWidthDelta,bodyHeightDelta);
}

void Architecture::update(b2Vec2 vTarget)
{
	list<Zombie*>::iterator it;

	for(it = zombieManager->zombies.begin(); it != zombieManager->zombies.end();it++)
	{
		Zombie* z = *it;

		int mark = 1;

		if(z->zDirection == Zombie::left)
			mark = -1;

		if(z->getPositionWithCocos().x < getPosition().x - getWidth()*0.5 - 0.001)
		{
			z->directionChange(Zombie::right);
			mark = 1;
		}
		else if(z->getPositionWithCocos().x > getPosition().x + getWidth()*0.5 + 0.001)
		{
			z->directionChange(Zombie::left);
			mark = -1;
		}
		b2Vec2 moveVec = b2Vec2(z->speed*mark/PTM_RATIO,0);

		z->updateMoveDelta(moveVec);
	}
	
	zombieManager->update(Timer::getDt(),vTarget);

	for(int i=0;i<components.size();i++)
	{
		if(NULL != ((UserDataInBody*)components[i]->getBody()->GetUserData())->mSprite)
		{
			CCSprite* spr = ((UserDataInBody*)components[i]->getBody()->GetUserData())->mSprite;

			float x = components[i]->getBody()->GetPosition().x;
			float y = components[i]->getBody()->GetPosition().y;

			spr->setPosition(ccp(x*PTM_RATIO,y*PTM_RATIO));
			spr->setRotation(-1*CC_RADIANS_TO_DEGREES(components[i]->getBody()->GetAngle()));
		}
	}
}
//delta만큼 해당 Architexture를 움직인다.

void Architecture::move(float deltaX,float deltaY)
{
	position.x += deltaX;

	for(int i=0;i<components.size();i++)
	{
		if(NULL != ((UserDataInBody*)components[i]->getBody()->GetUserData())->mSprite)
		{
			CCSprite* spr = ((UserDataInBody*)components[i]->getBody()->GetUserData())->mSprite;
			float x = spr->getPositionX();
			float y = spr->getPositionY();

			components[i]->getBody()->SetTransform(b2Vec2((x+deltaX)/PTM_RATIO,(y+deltaY)/PTM_RATIO),components[i]->getBody()->GetAngle());
		}
	}
}
void Architecture::killZombieOnArchitecture()
{
	zombieManager->allKillZombies();
}
Architecture::Architecture(CCLayer* target,b2World* world,int fullhp,int width,int height)
{
	this->zombieManager = new ZombieManager(world,target);

	this->fullHp = fullhp;
	this->hp = fullhp;
	this->target = target;
	this->width = width;
	this->height = height;
	this->world = world;
	this->position = ccp(target->getContentSize().width+width*0.5,163-25+height*0.5);
	this->isCollapsed = false;
}

void Architecture::deepCopy(Architecture* other)
{
	position = other->getPosition();

	for(int i=0;i<other->getFloorSize();i++)
		this->addFloorHeight(other->getFloorHeight(i));

	for(int i=0;i<other->components.size();i++) //component 복사
	{
		ArchComponent* com = other->components[i]; 

		CCSprite *img = ((UserDataInBody*)com->getBody()->GetUserData())->mSprite;

		this->addComponent(img->getTexture(),img->getPositionX()-position.x+width*0.5,
			img->getPositionY()-position.y+height*0.5,com->getType(),img->getZOrder(),com->getBodyWidthDelta(),com->getBodyHeightDelta());
	}

	this->genZombieNumber = other->genZombieNumber;
	this->genZombieType = other->genZombieType;

	this->generateZombies();
}
void Architecture::addComponent(CCTexture2D* texture,float x,float y,int type,int zOrder,int bodyWidthDelta,int bodyHeightDelta) 
{
	CCSprite* spr = CCSprite::createWithTexture(texture);

	spr->setPosition(ccp(position.x-width*0.5+x,position.y-height*0.5+y));		
	
	target->addChild(spr,zOrder);
	ArchComponent* component = new ArchComponent();
	component->setBodyWidthDelta(bodyWidthDelta);
	component->setBodyHeightDelta(bodyHeightDelta);
	component->setType(type);

	b2BodyDef bodyDef;

	float sprH = spr->getContentSize().height;
	float sprW = spr->getContentSize().width;

	bodyDef.type = b2_kinematicBody;	
	bodyDef.position.Set( (position.x - width * 0.5 + x)/PTM_RATIO,
		(position.y - height * 0.5 + y)/PTM_RATIO );		//body Position Set.

	UserDataInBody* data = new UserDataInBody();
	data->mChr = this;
	data->mSprite = spr;
	data->mType = TYPE_ARCHITECT;
	bodyDef.userData = data;															//body에 텍스쳐 붙임.	
	/*
	for(int i=0;i<components.size();i++)
	{
	if(y + sprH*0.5 > this->height)
	{
	height = sprH;
	break;
	}
	if(x + sprW*0.5 > this->width)
	{
	width = sprW;
	break;
	}
	}
	*/
	b2Body* body = world->CreateBody(&bodyDef);
	//	body->SetAwake(false);
	//	body->SetSleepingAllowed(false);
	b2FixtureDef fixtureDef;

	if(type == CIRCLE)
	{
		b2CircleShape circle;
		circle.m_radius = sprH*0.5/PTM_RATIO;

		fixtureDef.shape = &circle;
		fixtureDef.density = 1.0f;						
		fixtureDef.friction= 0.2f;
		fixtureDef.restitution = 1.0f;

		fixtureDef.filter.categoryBits = CATEGORY_ARCHITECT;
		fixtureDef.filter.maskBits = CATEGORY_BULLET | CATEGORY_PLAYER; 

		fixtureDef.density = 1.0f;

		body->CreateFixture(&fixtureDef);
	}
	else if(type == BOX)
	{
		b2PolygonShape box;
		box.SetAsBox((sprW-bodyWidthDelta)/PTM_RATIO*0.5,(sprH-bodyHeightDelta)/PTM_RATIO*0.5);

		fixtureDef.shape = &box;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		fixtureDef.restitution = 0.0f;

		fixtureDef.filter.categoryBits = CATEGORY_ARCHITECT;
		fixtureDef.filter.maskBits = CATEGORY_BULLET | CATEGORY_PLAYER; 

		fixtureDef.density = 1.0f;

		body->CreateFixture(&fixtureDef);
	}
	else if(type == POLYGON)
	{
		b2Vec2 vs2[4];

		vs2[0].Set(-width*0.5/PTM_RATIO,-height*0.5/PTM_RATIO);
		vs2[1].Set(width*0.5/PTM_RATIO,-height*0.5/PTM_RATIO);
		vs2[2].Set(0/PTM_RATIO,(height*0.5)/PTM_RATIO);
		vs2[3].Set(-width*0.5/PTM_RATIO,-height*0.5/PTM_RATIO);
		b2ChainShape baseChain;
		baseChain.CreateChain(vs2,4);
		b2FixtureDef fixtureDef;

		fixtureDef.shape = &baseChain;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 1.0f;

		fixtureDef.filter.categoryBits = CATEGORY_ARCHITECT;
		fixtureDef.filter.maskBits = CATEGORY_BULLET | CATEGORY_PLAYER; 

		body->CreateFixture(&fixtureDef);
	}
	else
	{
		CCLog("error Shape Type");
		return;
	}

	component->setBody(body);
	component->setZOrder(zOrder);
	components.push_back(component);
}
void Architecture::release(void)
{
	//CCObject::release();
	//delete zombieManager;

	if(components.empty())
		return;

	for(int i=0;i<components.size();i++)
	{
		UserDataInBody* ud = ((UserDataInBody*)components[i]->getBody()->GetUserData());

		CCSprite* spr = ud->mSprite;
		//		spr->removeAllChildrenWithCleanup(true);
		target->removeChild(spr);
		spr = NULL;
		delete ud;
		delete components[i];
		components[i] = NULL;
	}
	components.clear();
}
ArchitectManager::ArchitectManager(b2World* world,CCLayer* target)
{
	this->world = world;
	this->target = target;
	randTimer.setGenTimeRand(5.0f,20.0f);
	zOrder = 0;
	genTypeMax = 0;
	genTypeMin = 0;
}
void ArchitectManager::setGenZombieNumber(int max,int min)
{
	for(int i = 0;i != archs.size();i++)
	{
		archs[i]->genZombieNumber.mMax = max;
		archs[i]->genZombieNumber.mMin = min;
	}
}
void ArchitectManager::setGenZombieType(int max,int min)
{
	for(int i = 0;i != archs.size();i++)
	{
		archs[i]->genZombieType.mMax = max;
		archs[i]->genZombieType.mMin = min;
	}
}
int ArchitectManager::getBackArchitectHP()
{
	if(showArchs.empty())
		return -1;

	return showArchs.back()->getHP();
}
float ArchitectManager::getBackArchitectFloorHeight(int floorIndex)
{
	if(showArchs.empty())
		return -1.0f;

	return showArchs.back()->getFloorHeight(floorIndex);
}
int ArchitectManager::getBackArchitectFloorSize()
{
	if(showArchs.empty())
		return -1;

	return showArchs.back()->getFloorSize();
}
bool ArchitectManager::getBackIsIn(float x)
{
	if(showArchs.empty())
		return false;

	return showArchs.back()->isIn(x);
}
int ArchitectManager::whereIsIt(float x) 
{
	list<Architecture*>::iterator it;
	int i=0;
	for(it = showArchs.begin();it!=showArchs.end();it++,i++)
	{
		if((*it)->isIn(x))
			return i;
	}
	return -1;
}
void ArchitectManager::release()
{
	if(showArchs.empty() && archs.empty())
		return;

	list<Architecture*>::iterator it;

	for(int i=0;i<archs.size();i++)
	{
		archs.at(i)->release();
		delete archs.at(i);
	}
	archs.clear();

	for(it = showArchs.begin();it!=showArchs.end();it++)
	{
		(*it)->release();
		delete (*it);
	}

	showArchs.clear();
}
void ArchitectManager::setGenTime(float max,float min)
{
	randTimer.setGenTimeRand(max,min);
}
void ArchitectManager::setZOrderOnCreate(int zOrder)
{
	this->zOrder = zOrder;
}
void ArchitectManager::createRandomArchitect()
{
	if(archs.empty())
		return;

	int random = randForCreate.getCompleteRand();

	Architecture *arch = new Architecture(target,world,archs.at(random)->getFullHP(),
		archs.at(random)->getWidth(),archs.at(random)->getHeight());

	arch->deepCopy(archs.at(random));
	//arch->toDynamic();
	arch->setActiveArch(true);

	showArchs.push_back(arch);
}
void ArchitectManager::destoryFrontArchitect()
{
	if(!showArchs.empty())
	{
		for(int i=0;i<showArchs.front()->components.size();i++)
		{
			target->removeChild(((UserDataInBody*)showArchs.front()->components[i]->getBody()->GetUserData())->mSprite);
		}
		showArchs.front()->release();

		showArchs.pop_front();
	}
}
void ArchitectManager::addArchitectPattern(Architecture* arch)
{
	arch->setActiveArch(false);
	arch->setVisible(false);
	archs.push_back(arch);
}
void ArchitectManager::updateAll(b2Vec2 vTarget)
{
	list<Architecture*>::iterator it;

	for(it=showArchs.begin();it != showArchs.end();it++)
		(*it)->update(vTarget);
}
void ArchitectManager::moveByCamera(float dX)
{
	list<Architecture*>::iterator it;

	for(it=showArchs.begin();it != showArchs.end();it++)
		(*it)->move(dX,0);

	if(showArchs.empty())
	{
		if(randTimer.isTimeOver())
		{
			createRandomArchitect();
			randTimer.updateRandClock();
			return;
		}
		randTimer.clocking();
		return;
	}

	float width = showArchs.back()->getWidth();
	float height = showArchs.back()->getHeight();

	float backPosX = showArchs.back()->getPosition().x;
	float w = CCDirector::sharedDirector()->getWinSize().width;

	float frontPosX = showArchs.front()->getPosition().x;
	float frontWidth = showArchs.front()->getWidth();

	if(frontPosX <= -frontWidth*2)
		destoryFrontArchitect();

	if(backPosX + width*0.5 < CCDirector::sharedDirector()->getWinSize().width)
	{
		if(randTimer.isTimeOver())
		{
			createRandomArchitect();
			randTimer.updateRandClock();
		}
		else
			randTimer.clocking();
	}
}