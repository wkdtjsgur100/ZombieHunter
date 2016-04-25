#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"

#endif

class UserInfo
{
	bool isBackgroundSoundFirst;
	std::string name;
	std::string bestScore;
	std::string uuid;

	static UserInfo* instance;
	bool backgroundSoundOn;
	bool effectSoundOn;

	bool bTutorial; ///튜토리얼 안내창을 띄울지 말지 결정하는 변수

	UserInfo();
	~UserInfo();
public:
	CCEditBox* createEditBox();
	void setTutorialCheck(bool bTutorial);
	bool getTutorialCheck() { return bTutorial; }
	void registerPreferenceUUID(std::string uuid);
	void registerPreferenceName(std::string name);
	void registerPreferenceBestScore(std::string bestScore);

	static UserInfo* getInstance();

	bool getIsBackgroundSoundOn() { return backgroundSoundOn; } 
	bool getIsEffectSoundOn() { return effectSoundOn; }

	void setBackgroundSound(bool isOn);
	void setEffectSound(bool isOn);

	void end();
	std::string getUUID();
	std::string getName();
	std::string getBestScore();
};