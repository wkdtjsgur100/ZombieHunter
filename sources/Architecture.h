#pragma once

#include<cocos2d.h>
#include<list>
#include<Box2D/Box2D.h>

#include "UserDataInBody.h"
#include "MoveObjectByCamera.h"
#include "Timer.h"
#include "RandManager.h"
#include "zombie/ZombieManager.h"

using namespace std;

USING_NS_CC;

#define PTM_RATIO 32.0f

struct IntegerRange
{
	int mMin;
	int mMax;

	IntegerRange()
		:mMin(0),mMax(0)
	{
	}
	IntegerRange(int min,int max)
	{
		mMin = min;
		mMax = max;
	}
};
class ArchComponent
{
	b2Body* body;
	int zOrder;
	int bodyWidthDelta,bodyHeightDelta;
	int type;
public:	
	void setBodyWidthDelta(int bodyWidthDelta)
	{
		this->bodyWidthDelta = bodyWidthDelta;
	}
	void setBodyHeightDelta(int bodyHeightDelta)
	{
		this->bodyHeightDelta = bodyHeightDelta;
	}
	int getBodyWidthDelta()
	{
		return bodyWidthDelta;
	}
	int getBodyHeightDelta()
	{
		return bodyHeightDelta;
	}

	ArchComponent()
	{
		body = NULL;
		zOrder = 0;
	}
	~ArchComponent()
	{
		if(body != NULL)
		{
			body->GetWorld()->DestroyBody(body);
			body = NULL;
		}
	}
	b2Body* getBody()
	{
		if(body != NULL)
			return body;
		else
		{
			CCLog("body is null");
			return NULL;
		}
	}
	int getZOrder() { return zOrder; }
	void setZOrder(int zOrder) { this->zOrder = zOrder; }

	void setBody(b2Body* body)
	{
		this->body = body;
	}
	void setType(int type){ this->type = type; }
	int getType() { return type; }
};

class Architecture : public CCObject
{
	enum{CIRCLE,BOX,POLYGON};
	CCPoint position;
	int width,height;
	CCLayer* target;
	b2World* world;
	float hp;
	float fullHp;
	vector<float> floorsHeight;
	ZombieManager* zombieManager;
	bool isCollapsed;
public:
	vector<ArchComponent*> components;
	IntegerRange genZombieType;
	IntegerRange genZombieNumber;
public:

	void killZombieOnArchitecture();
	void generateZombies();
	int getFloorSize() { return floorsHeight.size(); }
	bool isIn(float x);
	float getFloorHeight(int floorIndex);
	void addFloorHeight(float floorHeight);
	void applyForce(b2Vec2 force,b2Vec2 point);
	CCPoint getPosition();
	int getWidth();
	int getHeight();

	float getHP();
	float getFullHP();

	void setActiveArch(bool active);
	void attack(float damage);
	void setVisible(bool visible)
	{
		for(int i=0;i<components.size();i++)
		{
			CCSprite *spr = ((UserDataInBody*)components[i]->getBody()->GetUserData())->mSprite;
			spr->setVisible(visible);
		}
	}

	//  원 모양의 바디인 Architecture를 구성할 component를 추가한다.
	
	void addCircleComponent(CCTexture2D* texture,float x,float y,int zOrder=0,int bodyWidthDelta=0,int bodyHeightDelta=0);

	void addBoxComponent(CCTexture2D* texture,float x,float y,int zOrder=0,int bodyWidthDelta=0,int bodyHeightDelta=0);
	
	void addPolygonComponent(CCTexture2D* texture,float x,float y,int zOrder=0,int bodyWidthDelta=0,int bodyHeightDelta=0);

	void update(b2Vec2 vTarget);
	//delta만큼 해당 Architexture를 움직인다.

	void move(float deltaX,float deltaY);

	Architecture(CCLayer* target,b2World* world,int fullhp,int width,int height);
	void release(void);

	void deepCopy(Architecture* other);
private:

	void addComponent(CCTexture2D* texture,float x,float y,int type,int zOrder,int bodyWidthDelta=0,int bodyHeightDelta=0);
};

class ArchitectManager : public MoveObjectByCamera
{
	vector<Architecture*> archs;
	list<Architecture*> showArchs; //보여지는 구조물들.
	Timer randTimer;
	CCLayer* target;
	int zOrder;
	int genTypeMax,genTypeMin;
	RandManager randForCreate;
	b2World* world;

public:
	ArchitectManager(b2World* world,CCLayer* target);
	int getBackArchitectHP();
	float getBackArchitectFloorHeight(int floorIndex);
	int getBackArchitectFloorSize();
	
	bool getBackIsIn(float x);
	
	int whereIsIt(float x);  //x가 속한 arch의 index를 반환 아무데도 없으면 -1.

	void release();
	void setGenTime(float max,float min);
	void setZOrderOnCreate(int zOrder);
	void createRandomArchitect();
	void destoryFrontArchitect();
	void addArchitectPattern(Architecture* arch);
	void updateAll(b2Vec2 vTarget);
	void moveByCamera(float dX);

	void setGenZombieType(int max,int min);
	void setGenZombieNumber(int max,int min);

	void setGenType(int max,int min){
		if(max < min)
		{
			genTypeMax = min;
			genTypeMin = max;
		}
		else
		{
			genTypeMax = max;
			genTypeMin = min;
		}
		if(genTypeMax >= archs.size())
		{
			genTypeMax = archs.size()-1;
		}
		randForCreate.setRange(genTypeMin,genTypeMax);
	}
};

