/*
		2013.11.28 
		zombieAttack수정, generate 수정
		jumpZombie수정.
		walk 수정.
		update 매개변수 수정.
*/
#pragma  once

#include "Character.h"
#include "UserDataInBody.h"
#include "ZombieResourceManager.h"
#include "util/EffectManager.h"
#include "ScoreManager.h"
#include "system/SoundManager.h"

class Zombie : public Character
{
public:
	enum ztype_enum{normal = 10,
		fast = 11,
		thrower = 12,
		jump = 13,
		flying = 14,
		huge = 21,
	};
	enum zState_enum{walking = 8751,
		attacking = 8752, skilling = 8753,dying = 8754, 
		dead = 8755,falling = 8756,standing=8757};

	Zombie();
	~Zombie();
	virtual void release();

	Zombie(b2World* _world, direction_enum _direction,CCLayer* _layer,float32 sX,float32 sY);
	void directionChange(direction_enum zd);
	void setTypeStat(
		ztype_enum zt, 
		int _speed,
		int _hp, 
		int _score,
		int _damage,
		float _attackRange,
		long _attackSpeed,
		float _genEnerge
		); //좀비 타입별 수치적 특성을 정의
	//만약 attackRange가 0이면 밀리를 뜻함. bodyInit에서 박스크기/2만큼으로 설정해주자.

	//좀비 타입별 보디/ 목 / 머리위치를 받아서 상대좌표를 구하는 용도.,
	//디자인쪽에서 주는 좌표계는 왼쪽위가 0,0

	virtual void update(float dt,b2Vec2 pv);
	virtual void subUpdate(float dt,b2Vec2 pv);
	virtual void cameraMoved(float dX);
	virtual void zombieMoveFor(b2Vec2 vec);
	virtual void zombieMoveTo(b2Vec2 toVec,int _speed);
	void updateMoveDelta(b2Vec2 vMoveDelta);

	//pv(플레이어 위치)와 AttackRange가지고 checkAttack!
	virtual bool checkAttack();
	//각 좀비들의 고유한 스킬을 쓸 때인지 판별
	virtual bool checkSkill();
	// skill을 다시한 번 쓸수 있는지??
	bool checkSkillCoolTime(); 

	void changeState(zState_enum);//상태가 바뀔때 한 번만 불러짐.
	virtual void walk(float dt);
	void dyingAnimating();
	void die(bool isHeadAttacked,b2Vec2 force);
	void attackAnimating();
	virtual void attack();
	virtual void skillStart();
	virtual void skill();
	void skillEnd();
	void headAttacked(b2Vec2 force);
	bool isDeadOnDamage(int _damage);

	direction_enum zDirection;
	direction_enum baseDirection;
	zState_enum currentState;

	////////Stat

	ztype_enum typeZ;
	int hp,maxHp;
	int score;
	int damage;
	long attackSpeed;
	float attackRange; // 밀리는 자기박스크기/2, 레인지는 고유한 attackRange를 가짐
	b2Vec2 vTarget;		//목표지점.
	b2Vec2 shootTarget;
	b2Vec2 vMoveDelta; //움직이는 양.(매프레임)

	//	b2Body* attackBody;
	///////////body, 애니메이션
	void headBodyInit(CCSprite* _headS,float radius,CCPoint headPoint);
	//attackAnimation이 없는 경우
	void attackBodyInit(CCSize attackBoxSize,CCPoint attackPoint,bool isActive);

	void attackBodyInit(CCAnimate* _attackAnim,CCSize attackBoxSize,CCPoint attackPoint,int attackFrame);
	virtual void directionUpdate();
	
	bool isAttackLess;
	bool isHeadLess;
	bool isSkillLess;
	bool isInArchitecture;

	b2Body* head;
	CCSprite* headS;
	b2Body* attackBody;
	b2Body* skillBody;

	b2Vec2 headNBodyVec;
	b2Vec2 centerToHeadVec; // 몸의 중점과 머리의 중점과의 벡터
	b2Vec2 centerToAttackVec; // 몸의 중점과 AttackBody와의 벡터
	b2Vec2 attackBoxSize;

	CCSize zombieSize;


	//Sprite
	CCAnimate* attackAnim;
	CCAnimate* deadAnim;
	CCAnimate* skillAnim;
	CCAnimate* headShotAnim;

	unsigned long lastAttackTime;
	bool isAttackAnimating;
	bool isAlreadyAttack;
	float attackStartFrameRatio; // 충돌검사 시작할 프레임 / 전체 프레임 개수
	float attackEndFrameRatio;
	bool isSkillAnimating;
	bool isHeadAttacked;
	unsigned long lastSkillTime;
	unsigned long skillCoolTime;
	
	float genEnerge;

	CCActionManager* actionManager;
	//GroundManager* gm;
private:
};
 