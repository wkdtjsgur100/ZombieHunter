#pragma once
#include "Character.h"
#include "UserDataInBody.h"
#include "weapon/WeaponManager.h"
#include "weapon/Weapon.h"

#define TAG_LEFT_HAND 123
#define TAG_RIGHT_HAND 456

class Player : public Character, CCObject
{
	enum{NORMAL_GUN,SHOTGUN,DDABAL_GUN};
public:
	enum pState_enum{walking = 1351,hitted = 1352, binded = 1353, dying = 1355, dead = 1356, stopping = 1357};
	Player(void);
	Player(b2World* _world,CCLayer* _layer,float32 sX,float32 sY);
	~Player();
	void update(float dt,GroundManager* gm);
	void walk(float dt,GroundManager* gm);
	void damaged(int dam);
	void weaponInit();
	void fire(b2Vec2 tv);
	void moveForPlayer(b2Vec2 tv);
	void changeHands();
	void changeState(pState_enum pState);
	void changeDirection(direction_enum pd);
	void bind(long time, int damage);
	void binding();
	bool isDead();

	WeaponManager* weaponManager;
	pState_enum currentState;
private:
	float nonDieTime;
	float moveDistance;
	b2Vec2 weaponVec;
	CCPoint shoulderVec;
	bool isShootRight;
	float shootAngle;
	long lastFireTime;
	bool isLastShooting;

	direction_enum currentDirection;
	

	CCAnimate* hittedAnim;
	CCAnimate* deadAnim;
	CCAnimate* stopAnim;

	CCActionManager* actionManager;

	long bindStartTime;
	long bindTotalTime;

};

