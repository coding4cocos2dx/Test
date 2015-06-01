#ifndef __DRAGONEGGTRIPLE_GEMACTION_H__
#define __DRAGONEGGTRIPLE_GEMACTION_H__

#include <iostream>
#include "cocos2d.h"
#include "Gem.h"
#include "DataTools.h"

USING_NS_CC;

class GemAction  :public Ref
{
public:
	

	static GemAction &getInstance()
	{
		static GemAction instance;
		return instance;
	}

	ActionInterval *getGemMoveAction(Direction direction);//�����ƶ�

	FiniteTimeAction *getGemRenewAction(MyPoint distination);//��Ǳ�ڸ�������

	FiniteTimeAction *getGemMoveToAction(MyPoint distination);//����ʱ�ƶ�

	ActionInterval *getGemChangeAction(Direction direction);//�����ƶ�

	ActionInterval *getCollectGemAction(const Point& startPoint,const Point& distPoint,float time);

	ActionInterval *getConvergeMoveAction(Point point);

	Point           getControlPoint(Point point1,Point point2,int distance);
	Point           getControlPointSingle(Point point1,Point point2,int distance);

	static void        setBright(Node* sender);
	static void        removeBright(Node* sender);
	//��Ч
// 	void playBackgroudMusic(CCNode* sender,void* data);
// 
// 	void stopBackgroudMusic();
// 
// 	void playEffectMusic(CCNode* sender,void* data);
// 
// 	void switchBackgroudMusic();


private:

	GemAction ();
	~GemAction ();
	static CCGLProgram* _glProgramBright;
	static CCGLProgram* _glProgramNormal;

	char* _musicPath;
	DISALLOW_COPY_AND_ASSIGN(GemAction);
};

#endif  //__DRAGONEGGTRIPLE_GEMACTION_H__

