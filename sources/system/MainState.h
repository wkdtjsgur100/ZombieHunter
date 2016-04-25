#pragma once
#include "cocos2d.h"
#include "TutorialChoiceState.h"

#include "RankData.h"

#include "OptionState.h"
#include "UserInfo.h"
#include "ui/Buttons.h"

USING_NS_CC;

#define ZORDER_MAIN 300

class RankCell : public CCTableViewCell
{
public:
	RankCell()
	{
	}
};

class RankTable : public CCTableViewDataSource, public CCTableViewDelegate
{
	CCTableView* tableView;
public:
	RankTable();

	void createTable();
	CCTableView* getTableView();
	
	virtual void tableCellHighlight(CCTableView* table, CCTableViewCell* cell){}
	virtual void tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell){}
	virtual void scrollViewDidScroll(CCScrollView* view){
	}
    virtual void scrollViewDidZoom(CCScrollView* view) {}
    
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
};

class MainState : public CCLayer,public CCEditBoxDelegate
{
	Buttons* menu;
	CCEditBox* inputBox;
	std::string userID;
	RankTable* table;
	CCMenuItemImage* sfxToggle;
	CCMenuItemImage* bgmToggle;
	CCLabelTTF *name_label;
	Background* background;
	CCSprite* loading_icon;
public:

	virtual bool init();
	static CCScene* scene();

	void gameStartCallback(CCObject* pSender);
	void sfxCallBack(CCObject* pSender);
	void bgmCallBack(CCObject* pSender);
	void inviteCallBack(CCObject* pSender);

	void onExit();
	void inputID();
	void addUserTitle();

	void update(float dt);

	virtual void keyBackClicked();

	virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text) {};
	virtual void editBoxEditingDidEnd(CCEditBox* editBox);
	virtual void editBoxReturn(CCEditBox* editBox){};

	CREATE_FUNC(MainState);

};