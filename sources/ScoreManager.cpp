#include "ScoreManager.h"

ScoreManager* ScoreManager::mInstance = NULL;

ScoreManager::ScoreManager()
{
	moveDistance = 0;
	point = 0;
}

ScoreManager* ScoreManager::sharedInstance()
{
	if(mInstance==NULL)mInstance = new ScoreManager();
	return mInstance;
}
void ScoreManager::release()
{
	if(mInstance!=NULL)
		delete mInstance;
}