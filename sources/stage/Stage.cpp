#include "Stage.h"


StageInfo::StageInfo(void)
{
}


StageInfo::~StageInfo(void)
{
}

StageManager::StageManager(b2World* world,CCLayer* targetLayer)
{
	this->world = world;
	this->targetLayer = targetLayer;

	currentStage = 0;

	groundInit();
	archInit();
	zombieInit();
	shopInit();
	backgroundInit();
	playerInit();

	shop->setWeaponManagerTarget(player->weaponManager);

	loadStage();
	setStage(1);
}
StageManager::~StageManager()
{
	if(!isInit())
		return;

	delete groundMng;

	archMng->release();
	delete archMng;
	//WeaponManager* weaponMng;
	delete player;
#ifndef BODY_DEBUG
	delete background;
#endif
	delete zombieManager;
	delete shop;

	this->stages.clear();
}
void StageManager::playerInit()
{
	player = new Player(world,targetLayer,
		300,  
		groundMng->getYGroundPos(300));
}
void StageManager::backgroundInit()
{
#ifndef BODY_DEBUG
	background = new Background(targetLayer);
#endif
}
void StageManager::shopInit()
{
	shop = new ShopManager(targetLayer);
	shop->addTexturePattern(CCTextureCache::sharedTextureCache()->addImage("ground/shop.png"));
	shop->setInitHeight(200);
	shop->setZOrderOnCreate(ZORDER_HUGEZOMBIE-1);
	shop->setGenTime(0,0);
}
void StageManager::groundInit()
{
	BaseGroundManager* baseGround = new BaseGroundManager(world,162);

	groundMng = new GroundManager(targetLayer,baseGround,world);

	groundMng->addInGroundPatterns("ground/ground_1.png","data/ground_1.json");
	groundMng->addInGroundPatterns("ground/ground_2.png","data/ground_2.json");
	groundMng->addInGroundPatterns("ground/ground_3.png","data/ground_3.json");
	groundMng->addInGroundPatterns("ground/ground_4.png","data/ground_4.json");
	groundMng->addInGroundPatterns("ground/ground_5.png","data/ground_5.json");
	groundMng->addInGroundPatterns("ground/ground_6.png","data/ground_6.json");
	groundMng->addInGroundPatterns("ground/ground_7.png","data/ground_7.json");

	groundMng->setZOrderOnCreate(ZORDER_GROUND);
}
void StageManager::zombieInit()
{
	////////////////               Zombie                     ////////////////////
	zombieManager = new ZombieManager(world,targetLayer);
	zombieManager->generateHugeZ(ccp(-490,groundMng->getYGroundPos(0)-20));
}
void StageManager::archInit()
{
	CCTexture2D* drum01Img = CCTextureCache::sharedTextureCache()->addImage("architecture/drum01.png");
	CCTexture2D* drum02Img = CCTextureCache::sharedTextureCache()->addImage("architecture/drum02.png");

	CCTexture2D* pan01Img = CCTextureCache::sharedTextureCache()->addImage("architecture/pan01.png");
	CCTexture2D* pan02Img = CCTextureCache::sharedTextureCache()->addImage("architecture/pan02.png");

	CCTexture2D* poly01Img = CCTextureCache::sharedTextureCache()->addImage("architecture/poly01.png");
	CCTexture2D* poly02Img = CCTextureCache::sharedTextureCache()->addImage("architecture/poly02.png");

	CCTexture2D* carImg[4];

	carImg[0] = CCTextureCache::sharedTextureCache()->addImage("architecture/car1.png");
	carImg[1] = CCTextureCache::sharedTextureCache()->addImage("architecture/car2.png");
	carImg[2] = CCTextureCache::sharedTextureCache()->addImage("architecture/car3.png");
	carImg[3] = CCTextureCache::sharedTextureCache()->addImage("architecture/car4.png");

	CCTexture2D* thin_pan = CCTextureCache::sharedTextureCache()->addImage("architecture/thin_pan.png");
	CCTexture2D* thin_leg = CCTextureCache::sharedTextureCache()->addImage("architecture/thin_leg.png");

	////////////Architect init//////////////
	
	archMng = new ArchitectManager(world,targetLayer);
	
	Architecture *drum01 = new Architecture(targetLayer,world,100,74,108);

	drum01->addBoxComponent(drum01Img,74/2,108/2,ZORDER_GROUND+1);

	archMng->addArchitectPattern(drum01);

	///////////////////////////////////////////////////////////////////////////
	Architecture *arch1 = new Architecture(targetLayer,world,100,257,142);

	arch1->addFloorHeight(107-34);

	arch1->addBoxComponent(drum01Img,37,54,7);
	arch1->addBoxComponent(drum01Img,257-37,54,ZORDER_GROUND+1,0,20);
	arch1->addBoxComponent(pan01Img,257*0.5,108,ZORDER_GROUND+1,0,20);

	archMng->addArchitectPattern(arch1);
	
	////////////////////////////////////////////////////////////
	
	Architecture *arch2 = new Architecture(targetLayer,world,100,74,92);

	arch2->addBoxComponent(poly01Img,36,46,ZORDER_GROUND+1,40,40);

	archMng->addArchitectPattern(arch2);

	////////////////////////////////////////////////////////////////

	Architecture *arch3 = new Architecture(targetLayer,world,100,74,92);

	arch3->addBoxComponent(poly02Img,36,46,ZORDER_GROUND+1,40,40);

	archMng->addArchitectPattern(arch3);

	////////////////////////////////////////////////////////////////
	float h = 540.0f;

	Architecture *arch4 = new Architecture(targetLayer,world,100,203,362);

	arch4->addBoxComponent(thin_leg,58,40);
	arch4->addBoxComponent(thin_leg,203-58,40);
	arch4->addBoxComponent(thin_leg,58,81+16+40);
	arch4->addBoxComponent(thin_leg,203-58,81+16+40);
	arch4->addBoxComponent(thin_leg,58,81+16+40+40+16+40);
	arch4->addBoxComponent(thin_leg,203-58,81+16+40+40+16+40);

	arch4->addBoxComponent(thin_pan,101,89);
	arch4->addBoxComponent(thin_pan,101,89+97);
	arch4->addBoxComponent(thin_pan,101,89+97*2);

	arch4->addBoxComponent(carImg[0],18+92,35);
	arch4->addBoxComponent(carImg[1],90,81+16+29);
	arch4->addBoxComponent(carImg[2],48,(81+16)*2+41);
	arch4->addBoxComponent(carImg[3],18+92,(81+16)*3+35);

	arch4->addFloorHeight(81+16);
	arch4->addFloorHeight((81+16)*2);
	arch4->addFloorHeight((81+16)*3);

	archMng->addArchitectPattern(arch4);

	////////////////////////////////////

}
bool StageManager::isInit()
{
	if(groundMng && archMng && zombieManager && background && shop && player)
		return true;
	return false;
}
void StageManager::loadStage()
{
	if(!isInit())
		return;

	for(int i=0; ;i++)
	{
		char src[256];

		sprintf(src,"data/stage/stage_%d.json",i+1);

		std::string fileName = CCFileUtils::sharedFileUtils()->fullPathForFilename(src);
		unsigned long bufferSize = 0;
		unsigned char* json = CCFileUtils::sharedFileUtils()->getFileData(fileName.c_str(), "r", &bufferSize); //json 데이터를 받아온다.

		if(!json)
			return;
		//양쪽의 garbage data를 지워주자.
		std::string clearData((const char*)json);
		size_t pos = clearData.rfind("}"); 
		clearData = clearData.substr(0,pos+1);

		//json Parsing..
		Document document; //json 처리용 객체.

		if(document.Parse<0>(clearData.c_str()).HasParseError()) //parse Error가 뜨면
		{
			CCLog("Parse Error");
			return;
		}
		if(!document.IsObject())
		{
			CCLog("parse Error");
			return;
		}
		StageInfo stageInfo;

		stageInfo = currentStageInfo;

		if(document.HasMember("stageTime"))
		{
			stageInfo.stageTime = (float)document["stageTime"].GetDouble();
		}
		if(document.HasMember("groundGenTime"))
		{
			stageInfo.groundGenTime.rangeMax = (float)document["groundGenTime"]["max"].GetDouble();
			stageInfo.groundGenTime.rangeMin = (float)document["groundGenTime"]["min"].GetDouble();
		}
		if(document.HasMember("groundGenType"))
		{
			stageInfo.groundGenType.rangeMax = (float)document["groundGenType"]["max"].GetInt();
			stageInfo.groundGenType.rangeMin = (float)document["groundGenType"]["min"].GetInt();
		}
		if(document.HasMember("zombieGenTime"))
		{
			stageInfo.zombieGenTime.rangeMax = (float)document["zombieGenTime"]["max"].GetDouble();
			stageInfo.zombieGenTime.rangeMin = (float)document["zombieGenTime"]["min"].GetDouble();
		}
		if(document.HasMember("zombieGenType"))
		{
			stageInfo.zombieGenType.rangeMax = (float)document["zombieGenType"]["max"].GetInt();
			stageInfo.zombieGenType.rangeMin = (float)document["zombieGenType"]["min"].GetInt();
		}
		if(document.HasMember("zombieEnerge"))
		{
			stageInfo.chargeEnerge = (float)document["zombieEnerge"]["chargeEnerge"].GetInt();
			stageInfo.maxEnerge = (float)document["zombieEnerge"]["maxEnerge"].GetInt();
		}
		if(document.HasMember("zombieCoolTime"))
		{
			stageInfo.fastZCool = (long)document["zombieCoolTime"]["fastZCool"].GetInt();
			stageInfo.flyingZCool = (long)document["zombieCoolTime"]["flyingZCool"].GetInt();
			stageInfo.normalZCool = (long)document["zombieCoolTime"]["normalZCool"].GetInt();
			stageInfo.throwerZCool = (long)document["zombieCoolTime"]["throwerZCool"].GetInt();
		}
		if(document.HasMember("architectGenTime"))
		{
			stageInfo.architectGenTime.rangeMax = (float)document["architectGenTime"]["max"].GetDouble();
			stageInfo.architectGenTime.rangeMin = (float)document["architectGenTime"]["min"].GetDouble();
		}
		if(document.HasMember("architectGenType"))
		{
			stageInfo.architectGenType.rangeMax = (float)document["architectGenType"]["max"].GetInt();
			stageInfo.architectGenType.rangeMin = (float)document["architectGenType"]["min"].GetInt();
		}
		if(document.HasMember("zombieOnArchitectNumber"))
		{
			stageInfo.zombieOnArchitectNumber.rangeMax = (float)document["zombieOnArchitectNumber"]["max"].GetInt();
			stageInfo.zombieOnArchitectNumber.rangeMin = (float)document["zombieOnArchitectNumber"]["min"].GetInt();
		}
		if(document.HasMember("zombieOnArchitectType"))
		{
			stageInfo.zombieOnArchitectType.rangeMax = (float)document["zombieOnArchitectType"]["max"].GetInt();
			stageInfo.zombieOnArchitectType.rangeMin = (float)document["zombieOnArchitectType"]["min"].GetInt();
		}
		if(document.HasMember("shopGenTime"))
		{
			stageInfo.shopGenTime.rangeMax = (float)document["shopGenTime"]["max"].GetDouble();
			stageInfo.shopGenTime.rangeMin = (float)document["shopGenTime"]["min"].GetDouble();
		}
		currentStageInfo = stageInfo;
		stages.push_back(stageInfo);


		if(json)
		{
			delete[] json;
			json = NULL;
		}
	}
}
void StageManager::setStage(int stage)
{
	if(	stage > stages.size() || 
		!isInit())
		return;

	StageInfo info = stages.at(stage-1);

	groundMng->setGenTime(info.groundGenTime.rangeMax,info.groundGenTime.rangeMin);
	groundMng->setGenType(info.groundGenType.rangeMax,info.groundGenType.rangeMin);

	archMng->setGenTime(info.architectGenTime.rangeMax,info.architectGenTime.rangeMin);
	archMng->setGenType(info.architectGenType.rangeMax,info.architectGenType.rangeMin);

	zombieManager->setGenTime(info.zombieGenTime.rangeMax,info.zombieGenTime.rangeMin);
	zombieManager->setGenType(info.zombieGenType.rangeMax,info.zombieGenType.rangeMin);
	zombieManager->setEnerges(info.chargeEnerge,info.maxEnerge);
	
	zombieManager->setCoolTimes(info.normalZCool,info.fastZCool,info.throwerZCool,info.flyingZCool);
	

	archMng->setGenZombieNumber(info.zombieOnArchitectNumber.rangeMax,info.zombieOnArchitectNumber.rangeMin);
	archMng->setGenZombieType(info.zombieOnArchitectType.rangeMax,info.zombieOnArchitectType.rangeMin);

	shop->setGenTime(info.shopGenTime.rangeMax,info.shopGenTime.rangeMin);

	currentStage = stage;
	currentStageInfo = info;
}
void StageManager::stageUp()
{
	if(!isInit())
	{
		CCLog("please Init");
		return;
	}
	if(stages.empty())
	{
		CCLog("Stage Empty");
		return;
	}

	setStage(++currentStage);
}
void StageManager::stageDown()
{
	if(!isInit())
	{
		CCLog("please Init");
		return;
	}
	if(stages.empty())
	{
		CCLog("Stage Empty");
		return;
	}

	setStage(--currentStage);
}
void StageManager::moveByCamera(float dX)
{
	
	if(currentStageInfo.stageTime <= 0)
	{
		if(stages.size() >= currentStage)
			stageUp();
	}
	else
		currentStageInfo.stageTime -= Timer::getDt();
}
void StageManager::update(float dt)
{
#ifndef BODY_DEBUG
	background->update();
#endif
	shop->update(player->getPositionWithCocos());
}