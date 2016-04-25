#include "TutorialState.h"

bool TutorialState::init()
{
	if(!GameState::init())
		return false;

	this->setInterfaceEnabled(false);

	this->schedule(schedule_selector(TutorialState::tutorialUpdate));

	this->stageManager->stageDown();

	tutorialManager->setCurrentPage(1);

	move = CCSprite::create("tutorial/move.png");

		CCPoint playerPos = stageManager->getPlayer()->getPositionWithCocos();
	
	move->setPosition(ccp(playerPos.x,playerPos.y + 70));
	
	move->setVisible(false);

	this->addChild(move,ZORDER_TUTORIAL_BACK);

	return true;
}
void TutorialState::tutorialUpdate(float dt)
{
	static CCSprite* fail;
	static Zombie* temp_z = NULL;
	static CCSprite* remove_icon;
	static CCSprite* touchIcon;

	if(tutorialManager->getCurrentPage() == 1)
	{
		if(stageManager->getZombieManager()->hugeZombie->getPositionWithCocos().x >= -150)
		{
			for(int i=0;i<2;i++)
			{
				buttons[i]->setTouchEnabled(true);
				buttons[i]->setZOrder(ZORDER_TUTORIAL_BACK+1);
			}
			tutorialManager->explainOne();
			tutorialManager->increasePage();
		}
	}
	else if(tutorialManager->getCurrentPage() == 2)
	{
		for(int i=0;i<2;i++)
		{
			if(buttons[i]->isSelected(RIGHT_BUTTON) || buttons[i]->isSelected(LEFT_BUTTON))
				tutorialManager->increasePage();
		}
	}
	else if(tutorialManager->getCurrentPage() == 3)
	{
		tutorialManager->removeExplain();

		move->setVisible(true);

		for(int i=0;i<2;i++)
			buttons[i]->setZOrder(ZORDER_INTERFACE);

		CCPoint playerPos = stageManager->getPlayer()->getPositionWithCocos();

		tutorialManager->increasePage();
	}
	else if(tutorialManager->getCurrentPage() == 4)
	{
		CCPoint playerPos = stageManager->getPlayer()->getPositionWithCocos();

		move->setPosition(ccp(playerPos.x,playerPos.y + 70));
		
		for(int i=0;i<2;i++)
		{
			if(buttons[i]->isSelected(RIGHT_BUTTON) || buttons[i]->isSelected(LEFT_BUTTON))
				tutorialManager->moveCheck += stageManager->getPlayer()->speed;
		}
		if(tutorialManager->moveCheck >= 200.0f)
		{
			this->removeChild(move);
			tutorialManager->increasePage();
		}
	}
	else if(tutorialManager->getCurrentPage() == 5)
	{
		CCPoint genPos;
		genPos.x = 930.0f;
		genPos.y = stageManager->getGroundMng()->getYGroundPos(genPos.x);

		temp_z = new NormalZombie(_world,Zombie::left,this,genPos.x,genPos.y);
		stageManager->getZombieManager()->generate(temp_z);

		remove_icon = CCSprite::create("tutorial/remove_icon.png");

		remove_icon->setPosition(ccp(temp_z->getPositionWithCocos().x+40,temp_z->getPositionWithCocos().y+82));

		this->addChild(remove_icon,ZORDER_INTERFACE);

		tutorialManager->explainTwo();
		this->pauseSchedulerAndActions();
	}
	else if(tutorialManager->getCurrentPage() == 6)
	{
		if(temp_z->currentState != Zombie::dying &&
			temp_z->currentState != Zombie::dead)
			remove_icon->setPosition(ccp(temp_z->getPositionWithCocos().x+40,temp_z->getPositionWithCocos().y+82));

		if(stageManager->getPlayer()->weaponManager->weapons.at(0)->getCurrentRemainBulletCnt() <= 0)
		{
			for(int i=0;i<2;i++)
				buttons[i]->setTouchEnabled(false);

			stageManager->getPlayer()->weaponManager->weaponButtons->setZOrder(ZORDER_TUTORIAL_BACK+1);
			stageManager->getPlayer()->weaponManager->reloadCallSpr->setZOrder(ZORDER_TUTORIAL_BACK+1);

			CCPoint playerPos = stageManager->getPlayer()->getPositionWithCocos();

			tutorialManager->removeExplain();
			tutorialManager->reloadExplain();

			fail = CCSprite::create("tutorial/fail.png");

			fail->setPosition(ccp(375,40));

			this->addChild(fail,ZORDER_TUTORIAL_BACK+2);
			
			touchIcon = CCSprite::create("tutorial/touch_icon.png");

			touchIcon->setPosition(ccp(playerPos.x + 100,playerPos.y + 140));

			this->addChild(touchIcon,ZORDER_TUTORIAL_BACK+1);

			tutorialManager->increasePage();
		}
	}
	else if(tutorialManager->getCurrentPage() == 8)
	{	
		tutorialManager->removeExplain();
		this->removeChild(fail);
		this->removeChild(touchIcon);

		for(int i=0;i<2;i++)
			buttons[i]->setTouchEnabled(true);
		
		tutorialManager->explainTwo();

		tutorialManager->increasePage();
	}//9인경우에 좀비가 죽으면 페이지 증가..
	else if(tutorialManager->getCurrentPage() == 9)
	{
		if(temp_z != NULL)
		{
			remove_icon->setPosition(ccp(temp_z->getPositionWithCocos().x+40,temp_z->getPositionWithCocos().y+82));

			if(temp_z->currentState == Zombie::dying)	
			{
				this->removeChild(remove_icon);
				tutorialManager->increasePage();
			}
			if(temp_z->getPositionWithCocos().x <= 10)
				temp_z->setPositionWithCocos(ccp(800,temp_z->getPositionWithCocos().y));
		}
	}
	else if(tutorialManager->getCurrentPage() == 10)
	{
		tutorialManager->removeExplain();

		for(int i=0;i<2;i++)
			buttons[i]->setTouchEnabled(false);

		this->stageManager->getPlayer()->weaponManager->weaponButtons->setTouchEnabled(true);
		this->stageManager->getPlayer()->weaponManager->weaponButtons->setZOrder(ZORDER_TUTORIAL_BACK+1);

		tutorialManager->explainThree();
		
		tutorialManager->increasePage();
		
	}
	else if(tutorialManager->getCurrentPage() == 11)
	{
		for(int i=0;i<5;i++)
		{
			if(stageManager->getPlayer()->weaponManager->weaponButtons->isSelected(i))
				tutorialManager->increasePage();	
		}
	}
	else if(tutorialManager->getCurrentPage() == 12)
	{
		tutorialManager->removeExplain();

		CCPoint genPos;
		genPos.x = 700.0f;
		genPos.y = stageManager->getGroundMng()->getYGroundPos(genPos.x);

		temp_z = new NormalZombie(_world,Zombie::left,this,genPos.x,genPos.y);
		stageManager->getZombieManager()->generate(temp_z);
		
		remove_icon = CCSprite::create("tutorial/remove_icon.png");

		remove_icon->setPosition(ccp(temp_z->getPositionWithCocos().x+40,temp_z->getPositionWithCocos().y+82));

		this->addChild(remove_icon,ZORDER_INTERFACE);

		tutorialManager->explainAgainRemoveZombie();

		for(int i=0;i<2;i++)
			buttons[i]->setTouchEnabled(true);

		this->stageManager->getPlayer()->weaponManager->weaponButtons->setZOrder(ZORDER_INTERFACE);
		
		tutorialManager->increasePage();
	}
	else if(tutorialManager->getCurrentPage() == 13)
	{
		if(temp_z != NULL)
		{
			remove_icon->setPosition(ccp(temp_z->getPositionWithCocos().x+40,temp_z->getPositionWithCocos().y+82));

			if(temp_z->currentState == Zombie::dying)		
				tutorialManager->increasePage();
			if(temp_z->getPositionWithCocos().x <= 10)
				temp_z->setPositionWithCocos(ccp(800,temp_z->getPositionWithCocos().y));
		}
	}
	else if(tutorialManager->getCurrentPage() == 14)
	{
		tutorialManager->removeExplain();

		this->removeChild(remove_icon);

		tutorialManager->explainFour();
		
		CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addImage("ground/shop.png");
		CCPoint genPos;
		genPos.x = 930.0f;
		genPos.y = stageManager->getGroundMng()->getYGroundPos(genPos.x) + tex->getContentSize().height*0.5;

		stageManager->getShop()->createBackGroundObject(genPos,tex);
		stageManager->getShop()->setObjectZOrder(ZORDER_TUTORIAL_BACK+1);
		stageManager->getShop()->setShopZOrder(ZORDER_TUTORIAL_BACK+1);

		for(int i=0;i<2;i++)
			buttons[i]->setZOrder(ZORDER_TUTORIAL_BACK+1);

		stageManager->getPlayer()->characterS->setZOrder(ZORDER_TUTORIAL_BACK+1);

		tutorialManager->increasePage();
	}
	else if(tutorialManager->getCurrentPage() == 15)
	{
		for(int i=0;i<2;i++)
		{
			if(buttons[i]->isSelected(RIGHT_BUTTON) || buttons[i]->isSelected(LEFT_BUTTON))
				tutorialManager->increasePage();
		}
	}
	else if(tutorialManager->getCurrentPage() == 16)
	{
		if(stageManager->getShop()->isPlayerInShop())
		{
			tutorialManager->removeExplain();

			ScoreManager::sharedInstance()->point = 2000;

			tutorialManager->explainFive();

			stageManager->getShop()->setObjectZOrder(ZORDER_INTERFACE);

			for(int i=0;i<2;i++)
			{
				buttons[i]->setEnabled(false);
				buttons[i]->setZOrder(ZORDER_INTERFACE);
			}

			stageManager->getPlayer()->weaponManager->weaponButtons->setTouchEnabled(false);

			stageManager->getPlayer()->characterS->setZOrder(ZORDER_PLAYER);

			tutorialManager->increasePage();
		}
	}
	else if(tutorialManager->getCurrentPage() == 18)
	{
		CCDirector::sharedDirector()->replaceScene(GameState::scene());
	}

	for(int i=0;i<contactListener->contacts.size();i++)
	{
		MyContact contact = contactListener->contacts[i];

		b2Body* bodyA = contact.fixtureA->GetBody();
		b2Body* bodyB = contact.fixtureB->GetBody();

		UserDataInBody* usrDataA = (UserDataInBody*)bodyA->GetUserData();
		UserDataInBody* usrDataB = (UserDataInBody*)bodyB->GetUserData();

		if((usrDataB->mType == TYPE_ZOMBIE_ATTACK && usrDataA->mType==TYPE_PLAYER) ||
			(usrDataB->mType == TYPE_PLAYER && usrDataA->mType==TYPE_ZOMBIE_ATTACK))
		{
			Zombie* z;
			if(usrDataB->mType == TYPE_ZOMBIE_ATTACK && usrDataA->mType==TYPE_PLAYER)
				z = (Zombie*)(usrDataB->mChr);
			if(usrDataA->mType == TYPE_ZOMBIE_ATTACK && usrDataB->mType==TYPE_PLAYER)
				z = (Zombie*)(usrDataA->mChr);

			if(z->damage >= 900)
			{
				if(!tutorialManager->isExplainDead)
				{
					tutorialManager->removeExplain(); 
					this->pauseSchedulerAndActions();
					tutorialManager->explainDead(userInterface);
				}
				stageManager->getZombieManager()->hugeZombie->zombieMoveFor(b2Vec2(-350/PTM_RATIO,0));
			}
			else
			{
				if(!tutorialManager->isExplainDead)
				{
					tutorialManager->removeExplain(); 
					this->pauseSchedulerAndActions();
					tutorialManager->explainDead(userInterface);
				
					if(temp_z != NULL && temp_z->currentState != Zombie::dead && temp_z->currentState != Zombie::dying)
						temp_z->zombieMoveFor(b2Vec2(400/PTM_RATIO,0));
				}
			}
			if(tutorialManager->isExplainDead)
				continue;

			userInterface->gaugeBar->setZOrder(ZORDER_TUTORIAL_BACK+2);
			userInterface->hpGaugeCenter->setZOrder(ZORDER_TUTORIAL_BACK+2);
			userInterface->hpGaugeSide->setZOrder(ZORDER_TUTORIAL_BACK+2);
			userInterface->hpPercent->setZOrder(ZORDER_TUTORIAL_BACK+2);
			userInterface->characterLabel->setZOrder(ZORDER_TUTORIAL_BACK+2);
		}
	}
}

CCScene* TutorialState::scene()
{
	CCScene *scene = CCScene::create();
	TutorialState *layer = TutorialState::create();
	
	scene->addChild(layer);

	return scene;
}