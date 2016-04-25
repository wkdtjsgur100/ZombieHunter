#include "RandManager.h"

RandManager::RandManager(unsigned int min,unsigned int max)
{
	setRange(min,max);
}
RandManager::RandManager()
{
	this->max = -1;
	this->min = -1;
}
void RandManager::_refreshFlag()
{
	flag = (( 1 << ( max + 1 ) ) - 1) ^ ( ( (1 << min)  - 1) );
	count = max-min+1;
}
void RandManager::setRange(unsigned int min,unsigned int max)
{
	this->min = min;
	this->max = max;
	_refreshFlag();
}
unsigned int RandManager::getCompleteRand()
{
	if( max == -1 && min == -1 ) 
	{
		CC_ASSERT("please init range..");
		return 0;
	}

	if(!flag) _refreshFlag();

	int random = rand() % count;
	unsigned int re = min;

	for( int i = min, cnt = -1; i < max + 1; i++ )
	{
		if( ( flag >> i ) & 1 )
			cnt++;

		if( cnt == random )
		{
			re = i;
			flag = flag ^ ( 1 << re );
			break;
		}
	}
	count--;

	return re;
}