#include "MapInfo.h"
#include <time.h>

MapInfo::MapInfo(void)
{
}


MapInfo::~MapInfo(void)
{
}

int** MapInfo::getGemMap()//生成元素地图的矩阵
{
	int ** gemMap = (int **)malloc(sizeof(int*)*kMatrixWidth);
	for(int i = 0;i<kMatrixWidth;i++)
	{
		gemMap[i] = (int*)malloc(sizeof(int)*kMatrixWidth);
	}
	srand ((unsigned)time(nullptr));
	for(int i = 0;i<kMatrixWidth;i++)
	{
		for(int j = 0;j<kMatrixWidth;j++)
		{
				if (MAP_TEST[i][j] == -1)
				{
					gemMap[i][j] = rand()%3 + 1;
				}
				else if (MAP_TEST[i][j] > 0)
				{
					gemMap[i][j] = MAP_TEST[i][j];
				}
		}
	}

	return gemMap;
}