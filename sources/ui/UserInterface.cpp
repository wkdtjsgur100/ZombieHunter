#include "UserInterface.h"

UserInterface::UserInterface(CCLayer* target)
{
	
	mitterBox = CCSprite::create("UI/mitter_box.png");

	mitterBox->setPosition(ccp(115,490));

	target->addChild(mitterBox,ZORDER_INTERFACE);
//
	gaugeBar = CCSprite::create("UI/gauge_bar.png");

	gaugeBar->setPosition(ccp(960*0.5,120));

	target->addChild(gaugeBar,ZORDER_INTERFACE);

	////

	hpGaugeCenter = CCSprite::create("UI/gauge_center.png");
	hpGaugeCenter->setPosition(ccp(478,120));

	target->addChild(hpGaugeCenter,ZORDER_INTERFACE);
//
	hpGaugeSide = CCSprite::create("UI/gauge_side.png");
	hpGaugeSide->setFlipX(true);
	hpGaugeSide->setPosition(ccp(606,120));

	target->addChild(hpGaugeSide,ZORDER_INTERFACE);
//
	bestScoreLabel = CCSprite::create("UI/bestscore.png");

	bestScoreLabel->setPosition(ccp(400,520));

	target->addChild(bestScoreLabel,ZORDER_INTERFACE);

	
	////
	scoreLabel = CCSprite::create("UI/score.png");

	scoreLabel->setPosition(ccp(390,480));

	target->addChild(scoreLabel,ZORDER_INTERFACE);
	
	////
	pointLabel = CCSprite::create("UI/point_icon.png");

	pointLabel->setPosition(ccp(200,470));

	target->addChild(pointLabel,ZORDER_INTERFACE);
	
	////
	characterLabel = CCSprite::create("UI/character_icon.png");

	characterLabel->setPosition(ccp(354,120));

	target->addChild(characterLabel,ZORDER_INTERFACE);
	//

	meterLabel = CCLabelTTF::create("m","fonts/rank_font.ttf",30.0);

	meterLabel->setHorizontalAlignment(kCCTextAlignmentRight);

	meterLabel->setPosition(ccp(196,506));

	target->addChild(meterLabel,ZORDER_INTERFACE);

	//
	score = CCLabelTTF::create("0","fonts/rank_font.ttf",30.0);

	score->setHorizontalAlignment(kCCTextAlignmentRight);

	score->setPosition(ccp(686,480));

	target->addChild(score,ZORDER_INTERFACE);

	//
	bestScore = CCLabelTTF::create("0","fonts/rank_font.ttf",30.0);

	bestScore->setHorizontalAlignment(kCCTextAlignmentRight);

	bestScore->setPosition(ccp(630,520));

	target->addChild(bestScore,ZORDER_INTERFACE);
	//
	movingDist = CCLabelTTF::create("0","fonts/rank_font.ttf",30.0);

	movingDist->setHorizontalAlignment(kCCTextAlignmentRight);

	movingDist->setPosition(ccp(166,510));

	target->addChild(movingDist,ZORDER_INTERFACE);

	//
	savingPoint = CCLabelTTF::create("0","fonts/rank_font.ttf",30.0);

	savingPoint->setHorizontalAlignment(kCCTextAlignmentRight);

	savingPoint->setPosition(ccp(172,470));

	target->addChild(savingPoint,ZORDER_INTERFACE);
//
	hpPercent = CCLabelTTF::create("100%","fonts/rank_font.ttf",30.0);

	hpPercent->setHorizontalAlignment(kCCTextAlignmentRight);

	hpPercent->setPosition(ccp(512,120));

	target->addChild(hpPercent,ZORDER_INTERFACE);

	hpRate = 1.0f;
}
void UserInterface::update(float dt)
{
	movingDist->setString(CCString::createWithFormat("%d",
		ScoreManager::sharedInstance()->moveDistance)->m_sString.c_str());

	savingPoint->setString(CCString::createWithFormat("%d",
		ScoreManager::sharedInstance()->point)->m_sString.c_str());

	score->setString(CCString::createWithFormat("%d",
		ScoreManager::sharedInstance()->point+ScoreManager::sharedInstance()->moveDistance)->m_sString.c_str());
	
	bestScore->setString(UserInfo::getInstance()->getBestScore().c_str());

	hpGaugeCenter->setScaleX(hpRate);
	hpGaugeCenter->setPositionX(357.0f+243*hpRate*0.5f);

	hpGaugeSide->setPositionX(357.0f+243*hpRate+6);

	hpPercent->setString(CCString::createWithFormat("%d%%",
		(int)(hpRate*100))->m_sString.c_str());

}