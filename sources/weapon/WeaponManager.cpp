#include "WeaponManager.h"

WeaponManager::WeaponManager(CCLayer* target,CCPoint initPos,int zOrder)
{
	this->target = target;
	weaponButtons = Buttons::create();

	weaponButtons->setPosition(initPos);
	weaponButtons->setMusicSrc("music/ui/ui_weapon_change.ogg");

	currentWeapon = NULL;
	target->addChild(weaponButtons,zOrder);
	/*
	CCMenuItemImage* reloadMenuItem = CCMenuItemImage::create("UI/reload_call.png","UI/reload_call.png",
		this,menu_selector(WeaponManager::reloading));
	
	reloadButtons = Buttons::create();

	reloadButtons->addChild(reloadMenuItem);

	reloadButtons->setPosition(1720*0.5,100*0.5);
	
	reloadButtons->setMusicSrc("music/ui/ui_reload.ogg");
	*/
	reloadCallTex = CCTextureCache::sharedTextureCache()->addImage("UI/reload_call.png");

	float sprWidth = reloadCallTex->getContentSize().width/2;
	float sprHeight = reloadCallTex->getContentSize().height;

	reloadCallSpr = CCSprite::createWithTexture(reloadCallTex,CCRectMake(0,0,sprWidth,sprHeight));

	CCAnimation *animation = CCAnimation::create();

	animation->setDelayPerUnit(0.125f);

	for(int i = 0;i<2;i++)
		animation->addSpriteFrameWithTexture(reloadCallTex, CCRectMake(i*sprWidth,0, sprWidth, sprHeight));

	CCAnimate *animate =  CCAnimate::create(animation);
	CCAction* rep = CCRepeatForever::create(animate);

	reloadCallSpr->runAction(rep);
	reloadCallSpr->retain();

	reloadCallSpr->setPosition(ccp(0,100));
	reloadCallSpr->setVisible(false);

	target->addChild(reloadCallSpr,ZORDER_INTERFACE);
	//target->addChild(reloadButtons,ZORDER_INTERFACE);

	currentWeaponTag = 0;
}
void WeaponManager::release()
{
	target->removeChild(weaponButtons);
	//target->removeChild(reloadButtons);
	target->removeChild(reloadCallSpr);

	reloadCallSpr->release();

	for(int wIt = 0;wIt < weapons.size();wIt++)
	{
		weapons[wIt]->release();
		delete weapons[wIt];
		weapons[wIt] = NULL;
	}
	weapons.clear();
}
void WeaponManager::fire(b2Vec2 vTarget)
{
	if(currentWeapon != NULL)
	{
		if(!currentWeapon->isReloading() && 
			currentWeapon->getIsFire() &&
			currentWeapon->getMaxRemainBulletCnt() > 0)
		{
			if(currentWeapon->isSoundOn())
			{
				switch(currentWeaponTag)
				{
				case 0: currentWeapon->soundId = SoundManager::getInstance()->playEffect("music/weapon/wpn_handgun.ogg"); break;
				case 1: currentWeapon->soundId = SoundManager::getInstance()->playEffect("music/weapon/wpn_rifle.ogg");break;
				case 2: currentWeapon->soundId = SoundManager::getInstance()->playEffect("music/weapon/wpn_machinegun.ogg");break;
				case 3: currentWeapon->soundId = SoundManager::getInstance()->playEffect("music/weapon/wpn_firegun_loop.ogg"); break;
				case 4: currentWeapon->soundId = SoundManager::getInstance()->playEffect("music/weapon/wpn_rifle.ogg"); break;
				}
				currentWeapon->updateSoundTimer();
			}
		}
		currentWeapon->fire(vTarget);
	}
}
void WeaponManager::update(float dt)
{
	if(currentWeapon != NULL)
	{
		currentWeapon->update(dt);
		if(currentWeapon->getCurrentRemainBulletCnt() <= 0)
		{
			reloadCallSpr->setPosition(ccp(currentWeapon->getFirePos().x*PTM_RATIO,
				currentWeapon->getFirePos().y*PTM_RATIO+80));
			reloadCallSpr->setVisible(true);
		}
		else
			reloadCallSpr->setVisible(false);
	}
}
void WeaponManager::updateFirePos(b2Vec2 vFirePos)
{
	if(currentWeapon != NULL)
		currentWeapon->updateFirePos(vFirePos);
}

CCSprite* WeaponManager::getLeftHand()
{
	if(currentWeapon==NULL)return NULL;
	return currentWeapon->getLeftHand();
}

CCSprite* WeaponManager::getRightHand()
{
	if(currentWeapon==NULL)return NULL;
	return currentWeapon->getRightHand();
}

bool WeaponManager::changeWeapon(int index)
{
	if(index >= weapons.size() || weapons.empty())
		return false;

	if(currentWeapon!=weapons.at(index))
	{
		currentWeaponTag = index;
		currentWeapon = weapons.at(index);
		return true;
	}
	return false;
}
void WeaponManager::addWeapon(Weapon* weapon,CCMenuItemImage* item)
{
	if(currentWeapon == NULL)
		currentWeapon = weapon;
	
	CCLabelTTF* pCurrentRemainLabel = CCLabelTTF::create(
		CCString::createWithFormat("%d",weapon->getCurrentRemainBulletCnt())
		->m_sString.c_str(),
		"fonts/rank_font.ttf",20.0);
	
	pCurrentRemainLabel->setPosition(ccp(40,30));
	pCurrentRemainLabel->setTag(0);

	item->addChild(pCurrentRemainLabel);

	CCLabelTTF* pMaxRemainLabel = CCLabelTTF::create(
		CCString::createWithFormat("/%d",weapon->getMaxRemainBulletCnt())->m_sString.c_str(),
		"fonts/rank_font.ttf",20.0);
	
	pMaxRemainLabel->setPosition(ccp(40,10));
	pMaxRemainLabel->setTag(1);
	
	item->addChild(pMaxRemainLabel);

	int child_num = weaponButtons->getChildrenCount();

	item->setAnchorPoint(ccp(0,0));
	item->setPosition(ccp(100 * child_num,20));
	item->setTag(child_num);

	weaponButtons->addChild(item);

	weapons.push_back(weapon);
}
void WeaponManager::clearHandsRotate()
{
	getLeftHand()->setRotation(0);
	if(currentWeapon->isDoubleHand)return;
	getRightHand()->setRotation(0);
	
}