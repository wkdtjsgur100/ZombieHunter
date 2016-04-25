#pragma once

#include "cocos2d.h"
#include "UserInfo.h"

USING_NS_CC;

class OptionState : public CCLayer,public CCEditBoxDelegate
{
	CCMenu* menu;
	CCEditBox* inputBox;
public:
	virtual bool init();
	static CCScene* scene();

	void popScene(CCObject* pSender);
	void modifyName(CCObject* pSender);
	
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text) {};
	virtual void editBoxEditingDidEnd(CCEditBox* editBox);
	virtual void editBoxReturn(CCEditBox* editBox){};

	CREATE_FUNC(OptionState);
};