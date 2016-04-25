#pragma  once
#include"Box2D/Box2D.h"
#include <vector>
#include "cocos2d.h"
#include "Ground.h"
#include "Timer.h"

#define ANGLE_RATIO 57.324f
#define PTM_RATIO 32
#define TAG_CURRENT_ANIM 1234
using std::vector;
USING_NS_CC;
class Character 
{
public:
	enum direction_enum{left=7011 ,stop = 7012 ,right = 7013 };


	Character();
	~Character();
	Character(b2World* _world,CCLayer* _layer,float32 sX,float32 sY);
	void update(float dt);
	void setPosition(b2Vec2 vec);
	void setPositionWithCocos(CCPoint vec);
	CCPoint getPositionWithCocos();
	b2Vec2 getPosition();
	CCSize getOneTextureSize(CCTexture2D* tec, CCSize size);

	void moveFor(b2Vec2 forVec);
	void moveTo(b2Vec2 toVec,int _speed);
	void characterBodyInit(CCSize contentSize,CCSize bodySize,CCPoint bodyPoint,void* _this, int _zOrder,  CCAnimate* _walkAnim, int _type);
	
	CCAnimate* createAnim(CCTexture2D* tec, CCSize size, float speed,int loops);

	b2World *world;
	b2Body* character; 
	b2Body* body;
	CCSprite* characterS;
	CCAnimate* walkAnim;
	
	b2Vec2 bodyVec; // 몸의 중점.(대가리+몸통 있는건 걍 character고, 이건 몸통만의 중점위치)
	float32 startX;
	float32 startY;
	CCLayer* mLayer;
	CCSprite* hpBar;
	int speed;
	int normalSpeed;
	int hp,maxHp;

	int zOrder;
	//좀비기준 0이 어디인가
	float originX,originY; 
	b2Vec2 centerToBodyVec;


};
