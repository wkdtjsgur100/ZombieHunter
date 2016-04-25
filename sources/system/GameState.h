#ifndef __GameState_SCENE_H__
#define __GameState_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include <vector>
#include <list>

#include "GLES-Render.h"

#include "cocos-ext.h" //for http-post

#include "weapon/Weapon.h"
#include "Camera.h"

#include "MyContactListener.h"

#include "ui/Buttons.h"
#include "../CocosDenshion/include/SimpleAudioEngine.h"
#include "stage/Stage.h"
#include "ScoreManager.h"
#include "PauseState.h"
#include "ui/UserInterface.h"
#include "tutorial/TutorialManager.h"
#include "system/GameOverState.h"

USING_NS_CC;

#define TAG_FADE 51

using namespace cocos2d::extension; //for http-post;
using namespace CocosDenshion;

class GameState : public CCLayer
{
protected:
	enum{LEFT_BUTTON,RIGHT_BUTTON};

	CCSprite* pChracImg;
	Buttons* buttons[2];

	b2World* _world;
	
	CCTexture2D* texture;
	//WeaponManager* weaponMng;
	bool pressing;
	CCPoint touchPoint;
	MyContactListener *contactListener;
	StageManager* stageManager;
	Buttons* pauseButton;
	CCSprite* fade;
	CCParticleSystem* m_emitter;
	UserInterface* userInterface;
	TutorialManager* tutorialManager;

	bool isPressReload;
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static CCScene* scene();
    
    // a selector callback
	void onExit();	
	void onHttpRequestCompleted(CCNode* sender,void *data);
	void sendGetHttp(CCObject* sender);
	void sendPostHttp(CCObject* sender);
	void draw();
	void doSnow();
	void doRain();
	void update(float dt);
	void box2DTest(float dt);
	void tutorialUpdate(float dt);
    void menuCloseCallback(CCObject* pSender);
	void ccTouchesBegan(CCSet *pTouches,CCEvent *pEvent);
	void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesEnded(CCSet *pTouches,CCEvent *pEvent);
	void addNewSpriteInWorld(CCPoint location);
	void pauseGame(CCObject* pSender);
	void setInterfaceEnabled(bool enable);
	void pauseSchedulerAndActions();

	virtual void keyBackClicked();
    // implement the "static node()" method manually
    CREATE_FUNC(GameState);
};
#endif // __GameState_SCENE_H__
