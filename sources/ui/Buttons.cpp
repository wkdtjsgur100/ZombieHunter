#include "Buttons.h"
#include "../CocosDenshion/include/SimpleAudioEngine.h"

using namespace CocosDenshion;

bool Buttons::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint location = touch->getLocation();
	bool re = CCMenu::ccTouchBegan(touch,event);

	if(musicSrc.empty())
		return re;

	CCArray* childArray = this->getChildren();
	
	CCObject* it = NULL;
	CCARRAY_FOREACH(childArray,it)
	{
		CCMenuItemImage *item = static_cast<CCMenuItemImage*>(it);
		if(item != NULL)
		{
			if(item->boundingBox().containsPoint(location-getPosition()))
				SoundManager::getInstance()->playEffect(musicSrc.c_str());
		}
	}
	return re;
}

bool Buttons::isSelected(int tag)
{
	CCMenuItemImage* item = static_cast<CCMenuItemImage*>(getChildByTag(tag));

	if(item != NULL)
		return item->isSelected();
	else
	{
		CCLog("해당태그가 없음");
		return false;
	}
}
void Buttons::unSelected()
{
	if (m_pSelectedItem)
    {
        m_pSelectedItem->unselected();
        m_pSelectedItem->activate();
    }
}
void Buttons::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCMenu::ccTouchEnded(touch,event);
}
//  virtual void ccTouchCancelled(CCTouch *touch, CCEvent* event);
void Buttons::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	CCMenu::ccTouchMoved(touch,event);
}