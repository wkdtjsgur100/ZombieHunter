#pragma once

#include "../CocosDenshion/include/SimpleAudioEngine.h"
#include "UserInfo.h"

using namespace CocosDenshion;

class SoundManager
{
	static SoundManager* instance;

	SoundManager();
	~SoundManager();
public:
	static SoundManager* getInstance();

	void playBackgroundMusic(const char* pszFilePath,bool bLoop=false);
	void pauseBackgroundMusic();
	void resumeBackgroundMusic();

	int playEffect(const char* pszFilePath,bool bLoop=false);
	
	void pauseEffect(unsigned int nSoundId);
	void resumeEffect(unsigned int nSoundId);
	void stopEffect(unsigned int nSoundId);

	void setBackgroundMusicVolume(float volume);
};