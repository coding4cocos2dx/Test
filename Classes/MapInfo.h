#ifndef __DRAGONEGGTRIPLE_MAPINFO_H__
#define __DRAGONEGGTRIPLE_MAPINFO_H__

#include "cocos2d.h"
#include "DataTools.h"
USING_NS_CC ;

class MapInfo :public Ref
{
public:
	MapInfo ();
	~MapInfo();

	int** getGemMap();

private:

};

#endif  //__DRAGONEGGTRIPLE_MAPINFO_H__

