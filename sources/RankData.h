#pragma once

#include <vector>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace rapidjson;

class RankInfo
{
public:
	std::string name;
	std::string score;

	RankInfo(std::string name,std::string score)
	{
		this->name = name;
		this->score = score;
	}
	RankInfo()
	{
	}
};
class RankData : public CCObject
{
	static RankData* instance;
	
	vector<RankInfo> ranks;
	RankData();
	~RankData();

	void onHttpRequestCompleted(CCNode* sender,void *data);
	void onSendCompleted(CCNode* sender,void *data);

	std::string bestRank;
public:

	bool bLoadComplete;
	bool IsLoading;

	static RankData* getInstance();
	void loadRank(std::string name,std::string uuid,std::string score);
	
	RankInfo getRank(int idx);
	std::string getBestRank() { return bestRank; }

	bool isLoadComplete() { return bLoadComplete; } 
};