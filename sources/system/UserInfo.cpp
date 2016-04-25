#include "UserInfo.h" 
#include "SoundManager.h"

UserInfo* UserInfo::instance = NULL; 
 
UserInfo::UserInfo() 
{ 
	backgroundSoundOn = CCUserDefault::sharedUserDefault()->getBoolForKey("UserSoundBackground",true);
	isBackgroundSoundFirst = !backgroundSoundOn;
	effectSoundOn = CCUserDefault::sharedUserDefault()->getBoolForKey("UserSoundEffect",true);
	bTutorial = CCUserDefault::sharedUserDefault()->getBoolForKey("UserTutorial",true);
} 
UserInfo::~UserInfo() 
{ 
} 
UserInfo* UserInfo::getInstance() 
{ 
    if(!instance) 
        instance = new UserInfo(); 
    return instance; 
} 

std::string UserInfo::getBestScore()
{
	if(bestScore.empty())
	{
        std::string ret = CCUserDefault::sharedUserDefault()->getStringForKey("BestScore");
		if(!ret.compare(""))
			ret = "0";

        bestScore = ret; 
	}
	return bestScore;
}
std::string UserInfo::getName() 
{ 
    if(name.empty()) 
    { 
        #if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
/* 
        JniMethodInfo methodInfo; 
        if(!JniHelper::getStaticMethodInfo(methodInfo,"good.games.mygame.GoodGame", 
            "getPreferenceName","()Ljava/lang/String;")) 
        { 
            CCLog("%s %d error to get methodInfo",__FILE__,__LINE__); 
            return name; 
        } 
        else 
        { 
            jstring str = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID); 
            const char* msg = methodInfo.env->GetStringUTFChars(str,0); 
            //CCLog("%s",msg); 
            methodInfo.env->ReleaseStringUTFChars(str,msg); 
            methodInfo.env->DeleteLocalRef(methodInfo.classID); 
        }
*/
		#endif 
        std::string ret = CCUserDefault::sharedUserDefault()->getStringForKey("UserName");
        name = ret; 
    } 
    return name; 
}
void UserInfo::registerPreferenceUUID(std::string uuid)
{
	CCUserDefault::sharedUserDefault()->setStringForKey("UUID",uuid.c_str()); 
}
std::string UserInfo::getUUID()
{
	if(uuid.empty())
	{
		uuid = CCUserDefault::sharedUserDefault()->getStringForKey("UUID");
	}
	return uuid;
}
void UserInfo::setTutorialCheck(bool bTutorial)
{
	this->bTutorial = bTutorial;
	CCUserDefault::sharedUserDefault()->setBoolForKey("UserTutorial",bTutorial);
}

void UserInfo::setBackgroundSound(bool isOn)
{
	backgroundSoundOn = isOn; 
	CCUserDefault::sharedUserDefault()->setBoolForKey("UserSoundBackground",isOn);

	if(!isOn)
		SoundManager::getInstance()->pauseBackgroundMusic();
	else
	{
		if(isBackgroundSoundFirst)
			SoundManager::getInstance()->playBackgroundMusic("music/background/main_bgm_loop.ogg",true);
		else
			SoundManager::getInstance()->resumeBackgroundMusic();
	}
}

void UserInfo::setEffectSound(bool isOn) 
{ 
	effectSoundOn = isOn; 
	CCUserDefault::sharedUserDefault()->setBoolForKey("UserSoundEffect",isOn);
}
	 
void UserInfo::end() 
{ 
    if(instance) 
        delete instance; 
} 
void UserInfo::registerPreferenceName(std::string name) 
{ 
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
/* 
    JniMethodInfo methodInfo; 
    if(!JniHelper::getStaticMethodInfo(methodInfo,"good.games.mygame.GoodGame", 
        "registerName","(Ljava/lang/String;)V")) 
    { 
        CCLog("%s %d error to get methodInfo",__FILE__,__LINE__); 
        return; 
    } 
    else 
    { 
        jstring stringArg1 = methodInfo.env->NewStringUTF(name.c_str()); 
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID,methodInfo.methodID,stringArg1); 
 
        methodInfo.env->DeleteLocalRef(stringArg1); 
         
        methodInfo.env->DeleteLocalRef(methodInfo.classID); 
    }
*/
#endif
    CCUserDefault::sharedUserDefault()->setStringForKey("UserName",name.c_str()); 
} 

void UserInfo::registerPreferenceBestScore(std::string bestScore)
{
	this->bestScore = bestScore;
	CCUserDefault::sharedUserDefault()->setStringForKey("BestScore",bestScore.c_str());
}
 
CCEditBox* UserInfo::createEditBox() 
{ 
    CCSize size; 
    CCScale9Sprite* boxSpr = CCScale9Sprite::create("main_screen/id_input/input_spr.png"); 
    size.setSize(boxSpr->getContentSize().width+5,boxSpr->getContentSize().height+5); 
    CCEditBox* inputBox = CCEditBox::create(size,boxSpr); 
 
    CCSize winSize = CCDirector::sharedDirector()->getWinSize(); 
 
    inputBox->setPosition(ccp(winSize.width*0.5,winSize.height*0.5)); 
    inputBox->setMaxLength(5); 
	inputBox->setReturnType(kKeyboardReturnTypeDone);
	
	inputBox->attachWithIME();

    return inputBox; 
}