#pragma once

#include "cocos2d.h"
#include "system/SoundManager.h"

USING_NS_CC;

class Buttons : public CCMenu
{
	std::string musicSrc;
public:
	bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	void ccTouchEnded(CCTouch* touch, CCEvent* event);
  //  virtual void ccTouchCancelled(CCTouch *touch, CCEvent* event);
	 void ccTouchMoved(CCTouch* touch, CCEvent* event);

	bool isSelected(int tag);

	void unSelected();

	void setMusicSrc(const char* musicSrc)
	{
		this->musicSrc.append(musicSrc);
	}
	void onEnter()
	{
		CCMenu::onEnter();
		CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
	}
	void onExit()
	{
		CCMenu::onExit();
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	}
	CREATE_FUNC(Buttons);
};