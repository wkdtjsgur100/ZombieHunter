#include "SoundManager.h"

SoundManager* SoundManager::instance = NULL;

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
}

SoundManager* SoundManager::getInstance()
{
	if(!instance)
		instance = new SoundManager();
	return instance;
}

void SoundManager::playBackgroundMusic(const char* pszFilePath,bool bLoop)
{
	if(UserInfo::getInstance()->getIsBackgroundSoundOn())
	{
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(pszFilePath,bLoop);
	}
}

void SoundManager::pauseBackgroundMusic()
{
	if(SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void SoundManager::resumeBackgroundMusic()
{
	if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

int SoundManager::playEffect(const char* pszFilePath,bool bLoop)
{
	if(UserInfo::getInstance()->getIsEffectSoundOn())
	{
		return SimpleAudioEngine::sharedEngine()->playEffect(pszFilePath,bLoop);
	}
	return -1;
}
void SoundManager::setBackgroundMusicVolume(float volume)
{
	if(SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(volume);
}

void SoundManager::stopEffect(unsigned int nSoundId)
{
	if( nSoundId != -1 &&
		UserInfo::getInstance()->getIsEffectSoundOn() )
	{
		SimpleAudioEngine::sharedEngine()->stopEffect(nSoundId);
	}
}
void SoundManager::pauseEffect(unsigned int nSoundId)
{
	if( nSoundId != -1 &&
		UserInfo::getInstance()->getIsEffectSoundOn() )
	{
		SimpleAudioEngine::sharedEngine()->pauseEffect(nSoundId);
	}
}

void SoundManager::resumeEffect(unsigned int nSoundId)
{
	if( nSoundId != -1 &&
		UserInfo::getInstance()->getIsEffectSoundOn() )
	{
		SimpleAudioEngine::sharedEngine()->resumeEffect(nSoundId);
	}
}