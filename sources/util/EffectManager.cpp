#include "EffectManager.h"

EffectManager* EffectManager::instance = 0;

EffectManager* EffectManager::getInstance()
{
	if(instance == 0)
		instance = new EffectManager();
	return instance;
}

void EffectManager::moveByCamera(float dX)
{
	std::list<CCLabelTTF*>::iterator it;

	for(it = moveLabels.begin(); it != moveLabels.end();it++)
	{
		(*it)->setPositionX((*it)->getPositionX()+dX);
	}
}
void EffectManager::setTarget(CCLayer* target)
{
	this->target = target;
}
void EffectManager::update()
{
	if(!moveLabels.empty())
	{
		if(moveLabels.back()->getOpacity() == 0)
		{
			target->removeChild(moveLabels.back());
			moveLabels.pop_back();
		}
	}
	if(!interfaceLabels.empty())
	{
		if(interfaceLabels.back()->getOpacity() == 0)
		{	
			target->removeChild(interfaceLabels.back());
			interfaceLabels.pop_back();
		}
	}
	if(!sprs.empty())
	{
		if(sprs.back()->getOpacity() == 0)
		{
			target->removeChild(sprs.back());
			sprs.pop_back();
		}
	}
}
CCLabelTTF* EffectManager::labelEffect(std::string str,CCPoint startPos,LabelEffectInfo ei)
{
	CCLabelTTF* newLabel = CCLabelTTF::create(str.c_str(),"fonts/rank_font.ttf",ei.fontSize);

	CCMoveBy* moveAction = CCMoveBy::create(ei.duration,ei.deltaPosition);
	CCFadeOut* fadeAction = CCFadeOut::create(ei.duration);

	CCSpawn* totalAction = CCSpawn::create(moveAction,fadeAction,NULL);

	newLabel->setPosition(startPos);
	newLabel->setColor(ei.color);

	newLabel->runAction(totalAction);

	if( target != NULL) target->addChild(newLabel,ZORDER_INTERFACE);

	return newLabel;
}
void EffectManager::InterfaceLabelEffect(std::string str,CCPoint startPos,LabelEffectInfo ei)
{
	interfaceLabels.push_back(labelEffect(str,startPos,ei));
}
void EffectManager::moveLabelEffect(std::string str,CCPoint startPos,LabelEffectInfo ei)
{
	moveLabels.push_back(labelEffect(str,startPos,ei));
}
void EffectManager::release()
{
	std::list<CCLabelTTF*>::iterator it;

	for(it = moveLabels.begin(); it != moveLabels.end();it++)
		target->removeChild((*it));

	for(it = interfaceLabels.begin();it != interfaceLabels.end();it++)
		target->removeChild((*it));

	interfaceLabels.clear();
	moveLabels.clear();
}
