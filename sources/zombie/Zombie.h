/*
		2013.11.28 
		zombieAttack����, generate ����
		jumpZombie����.
		walk ����.
		update �Ű����� ����.
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
		); //���� Ÿ�Ժ� ��ġ�� Ư���� ����
	//���� attackRange�� 0�̸� �и��� ����. bodyInit���� �ڽ�ũ��/2��ŭ���� ����������.

	//���� Ÿ�Ժ� ����/ �� / �Ӹ���ġ�� �޾Ƽ� �����ǥ�� ���ϴ� �뵵.,
	//�������ʿ��� �ִ� ��ǥ��� �������� 0,0

	virtual void update(float dt,b2Vec2 pv);
	virtual void subUpdate(float dt,b2Vec2 pv);
	virtual void cameraMoved(float dX);
	virtual void zombieMoveFor(b2Vec2 vec);
	virtual void zombieMoveTo(b2Vec2 toVec,int _speed);
	void updateMoveDelta(b2Vec2 vMoveDelta);

	//pv(�÷��̾� ��ġ)�� AttackRange������ checkAttack!
	virtual bool checkAttack();
	//�� ������� ������ ��ų�� �� ������ �Ǻ�
	virtual bool checkSkill();
	// skill�� �ٽ��� �� ���� �ִ���??
	bool checkSkillCoolTime(); 

	void changeState(zState_enum);//���°� �ٲ� �� ���� �ҷ���.
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
	float attackRange; // �и��� �ڱ�ڽ�ũ��/2, �������� ������ attackRange�� ����
	b2Vec2 vTarget;		//��ǥ����.
	b2Vec2 shootTarget;
	b2Vec2 vMoveDelta; //�����̴� ��.(��������)

	//	b2Body* attackBody;
	///////////body, �ִϸ��̼�
	void headBodyInit(CCSprite* _headS,float radius,CCPoint headPoint);
	//attackAnimation�� ���� ���
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
	b2Vec2 centerToHeadVec; // ���� ������ �Ӹ��� �������� ����
	b2Vec2 centerToAttackVec; // ���� ������ AttackBody���� ����
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
	float attackStartFrameRatio; // �浹�˻� ������ ������ / ��ü ������ ����
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
 