#pragma  once
#include "Timer.h"
#include "Box2D/Box2D.h"
#include "MoveObjectByCamera.h"
#include "RandManager.h"

#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "UserDataInBody.h"

using namespace std;
using namespace rapidjson; // for json

#define PTM_RATIO 32
#define BASEGROUND_DELTA 35

struct GroundPatternInfo
{
	CCTexture2D* texture;
	list<CCPoint> points;
	
	GroundPatternInfo()
	{
		texture = NULL;
	}
	~GroundPatternInfo()
	{
		points.clear();
	}
	void loadPoints(const char* src)
	{
		std::string fileName = CCFileUtils::sharedFileUtils()->fullPathForFilename(src);
		unsigned long bufferSize = 0;
		unsigned char* json = CCFileUtils::sharedFileUtils()->getFileData(fileName.c_str(), "r", &bufferSize); //json �����͸� �޾ƿ´�.

		//������ garbage data�� ��������.
		std::string clearData((const char*)json);
		size_t pos = clearData.rfind("]"); 
		clearData = clearData.substr(0,pos+1);

		//json Parsing..
		Document document; //json ó���� ��ü.

		if(document.Parse<0>(clearData.c_str()).HasParseError()) //parse Error�� �߸�
		{
			CCLog("Parse Error");
			return;
		}
		int inX,inY;
		if(!document.IsArray())
		{
			CCLog("parse Error");
			return;
		}
		for(rapidjson::SizeType i=0;i<document.Size();i++)
		{
			assert(document.IsArray());

			inX = document[i]["x"].GetInt();
			inY = document[i]["y"].GetInt();

			CCPoint p = ccp(inX,inY);
			points.push_back(p);
		}

		if(json != NULL)
		{
			document.Clear();
			delete[] json;
			json = NULL;
		}
	}
};

class Ground
{
	b2Body* groundBody;
	int zOrder;
	b2World* world;
	CCLayer* target;
	CCPoint position;
public:
	list<CCPoint> points;

	~Ground();
	Ground(b2World* world,CCLayer* target,GroundPatternInfo* patternInfo,int baseHeight,int zOrder = 0);
	CCPoint getPosition();
	float getWidth();
	float getHeight();

	//true �����ϸ� �����ؾߵǰ�, �ƴϸ� false. overridding �Ѱ� �ƴ�!
	bool moveByCamera(float dX);
};

class BaseGroundManager
{
	b2Body* baseGroundBody;
	float baseHeight;
public:
	float getBaseHeight(){return baseHeight;}
	~BaseGroundManager();
	BaseGroundManager(b2World* world,float baseHeight);
};
class GroundManager : public MoveObjectByCamera
{
private:
	BaseGroundManager* baseGroundManager;
	vector<GroundPatternInfo*> groundPatternInfos;
	list<Ground*> showingGrounds;
	//Ground* lastGround;
	list<CCPoint> patternPoints;
	b2World* world;
	CCLayer* target;

	Timer randTimer;
	int zOrder;
	int genTypeMax,genTypeMin;

	RandManager randForCreate;
private:	
	///ground�� random�� ������ ������ �ø���.
	void addRandomObjectToGrounds();
	//������ ������ �÷��� �� ���� �Ǵ��Ѵ�.
	bool createCheck();
	//���� ���̰� �ִ� ������ �������� �� ������ �� ������ �����Ѵ�.
	//void destoryGround();
	//x��ǥ�� ���Ͼȿ� �ִ��� �Ǵ��Ѵ�.
	bool itIsInPattern(float x);
public:
	~GroundManager();
	GroundManager(CCLayer *target,BaseGroundManager* baseGroundManager,b2World* _world) ;
	
	float getBaseGroundHeight() { return baseGroundManager->getBaseHeight(); }
	
	void setGenTime(float max,float min);
	void setGenType(int max,int min){
		if(max < min)
		{
			genTypeMax = min;
			genTypeMin = max;
		}
		else
		{
			genTypeMax = max;
			genTypeMin = min;
		}
		if(genTypeMax >= groundPatternInfos.size())
		{
			genTypeMax = groundPatternInfos.size()-1;
		}

		randForCreate.setRange(genTypeMin,genTypeMax);
	}

	void setZOrderOnCreate(int zOrder) { this->zOrder = zOrder; }
	//���������� �ϳ� �߰��Ѵ�.ù��° ���ڴ� �����̹����� src,�ι������ڴ� ���� ������ src�̴�.
	void addInGroundPatterns(const char* ImgSrc,const char* txtSrc);

	//xPos�� �϶� ������ y��ǥ�� ��ȯ�Ѵ�.
	float getYGroundPos(float xPos);

	//���ϴ� ��ü�� x��ǥ�� �ѱ�� �� ��ü�� ������ ���� delta��ŭ �̵��Ѵٰ� �������� ���� ���� ��ȯ�Ѵ�.
	//delta���� -�� �������� �̵��Ѵٰ� �������� ���Ͱ��� ��ȯ�Ѵ�.
	CCPoint getMoveVecFromGround(float objX,int delta);

	void moveByCamera(float dX);
};
