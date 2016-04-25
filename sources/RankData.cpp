#include "RankData.h"
#include "system/UserInfo.h"
#include <tchar.h>

RankData * RankData::instance = NULL;

RankData::RankData()
{
	bLoadComplete = false;
	IsLoading = true;
}

void RankData::loadRank(std::string name,std::string uuid,std::string score)
{
	IsLoading = true;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	TCHAR strUTF8[1024];

	MultiByteToWideChar(CP_ACP,MB_COMPOSITE,name.c_str(),-1,strUTF8,1024);

	TCHAR sendJson[1024] = {0};

	_tcscpy(sendJson,L"http://deploydjango1.herokuapp.com/?");

	if(!name.empty())
	{
		_tcscat(sendJson,L"id=");

		_tcscat(sendJson,strUTF8);
	}
	//sendJson.append(strUTF8);
	_tcscat(sendJson,L"&uuid=");
	
	if(uuid.empty())
		uuid = "NOUUID";
	
	MultiByteToWideChar(CP_ACP,MB_COMPOSITE,uuid.c_str(),-1,strUTF8,1024);

	_tcscat(sendJson,strUTF8);

	if(!score.empty())
	{
		_tcscat(sendJson,L"&score=");

		MultiByteToWideChar(CP_ACP,MB_COMPOSITE,score.c_str(),-1,strUTF8,1024);

		_tcscat(sendJson,strUTF8);
	}
	//sendJson.append("&score=");

	char charBuffer[1024];

	//sendJson.append(score);
	WideCharToMultiByte(CP_UTF8, 0, sendJson, -1, charBuffer, 1024, NULL, NULL );
	CCHttpRequest* request = new CCHttpRequest();

	//std::string url = "http://deploydjango1.herokuapp.com/?";
	//url = charBuffer;

	request->setUrl(charBuffer);
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::string url = "http://deploydjango1.herokuapp.com/?";

	if(!name.empty())
	{
		url.append("id=");
		url.append(name.c_str());
	}
	//sendJson.append(strUTF8);
	url.append("&uuid=");
	
	if(uuid.empty())
		uuid = "NOUUID";
	
	url.append(uuid.c_str());

	if(!score.empty())
	{
		url.append("&score=");

		url.append(score.c_str());
	}
	//sendJson.append("&score=");

	CCHttpRequest* request = new CCHttpRequest();

	request->setUrl(url.c_str());
#endif
	request->setTag("send");

	request->setRequestType(CCHttpRequest::kHttpGet);
	std::vector<std::string> headers;
	
	headers.push_back("Content-Type: application/json; charset=utf-8");
	request->setHeaders(headers);

	CCHttpClient::getInstance()->send(request); //보내고 나서

	request->setResponseCallback(this,callfuncND_selector(RankData::onHttpRequestCompleted));//받습니다

	request->release();
}

void RankData::onHttpRequestCompleted(CCNode* sender,void *data)
{
	CCHttpResponse *response = (CCHttpResponse*)data;

	if(!response)
	{
		CCMessageBox("server error.","error");
		return;
	}

	if( NULL != strlen(response->getHttpRequest()->getTag()))
	{
		CCLog("%s complete.",response->getHttpRequest()->getTag());
	}
	int statusCode = response->getResponseCode();
	CCLog("response code : %d",statusCode);

	if(!response->isSucceed())
	{	
		CCMessageBox("server error","error");
		CCLog("response failed");
		CCLog("errorbuffer: %s",response->getErrorBuffer());
		return;
	}
	std::vector<char>* buffer = response->getResponseData();

	std::string str;

	for(unsigned int i=0;i<buffer->size();i++)
		str.push_back((*buffer)[i]);

	const char* json = str.c_str();

	//json Parsing..
	Document document; //json 처리용 객체.

	if(document.Parse<0>(json).HasParseError()) //parse Error가 뜨면
	{
		CCLog("Parse Error");
		return;
	}
	
	if(!document.IsObject())
	{
		CCLog("parse Error");
		return;
	}
	
	ranks.clear();

	bestRank = "UNRANK";

	if(document.HasMember("uuid"))
	{
		UserInfo::getInstance()->registerPreferenceUUID(document["uuid"].GetString());
	}
	if(document.HasMember("rank"))
	{
		this->bestRank = document["rank"].GetString();
	}
	if(document.HasMember("ranklist"))
	{
		for(rapidjson::SizeType i=0;i<document["ranklist"].Size();i++)
		{
			rapidjson::SizeType j=0;

			RankInfo ri;

			ri.name = document["ranklist"][i][j++].GetString();
			ri.score = document["ranklist"][i][j].GetString();

			ranks.push_back(ri);
		}
		
		document["ranklist"].Clear();
	}
	bLoadComplete = true;
	IsLoading = false;
}
RankData::~RankData()
{
}

RankData* RankData::getInstance()
{
	if(!instance)
		instance = new RankData();
	return instance;
}
RankInfo RankData::getRank(int idx)
{
	if(ranks.empty())
		CCLog("please load rank..");

	if(idx < ranks.size())
		return ranks.at(idx);
	else
		return RankInfo("","0");
}