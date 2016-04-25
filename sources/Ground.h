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
		unsigned char* json = CCFileUtils::sharedFileUtils()->getFileData(fileName.c_str(), "r", &bufferSize); //json 데이터를 받아온다.

		//양쪽의 garbage data를 지워주자.
		std::string clearData((const char*)json);
		size_t pos = clearData.rfind("]"); 
		clearData = clearData.substr(0,pos+1);

		//json Parsing..
		Document document; //json 처리용 객체.

		if(document.Parse<0>(clearData.c_str()).HasParseError()) //parse Error가 뜨면
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

	//true 리턴하면 삭제해야되고, 아니면 false. overridding 한것 아님!
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
	///ground에 random한 지형을 생성해 올린다.
	void addRandomObjectToGrounds();
	//지금이 지형을 올려야 될 땐지 판단한다.
	bool createCheck();
	//현재 보이고 있는 지형이 뒤쪽으로 다 빠지면 그 지형을 삭제한다.
	//void destoryGround();
	//x좌표가 패턴안에 있는지 판단한다.
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
	//지형패턴을 하나 추가한다.첫번째 인자는 지형이미지의 src,두번쨰인자는 지형 정보의 src이다.
	void addInGroundPatterns(const char* ImgSrc,const char* txtSrc);

	//xPos값 일때 지형의 y좌표를 반환한다.
	float getYGroundPos(float xPos);

	//원하는 객체의 x좌표를 넘기면 그 객체가 지형을 따라 delta만큼 이동한다고 했을때의 벡터 값을 반환한다.
	//delta값이 -면 뒤쪽으로 이동한다고 했을때의 벡터값을 반환한다.
	CCPoint getMoveVecFromGround(float objX,int delta);

	void moveByCamera(float dX);
};
