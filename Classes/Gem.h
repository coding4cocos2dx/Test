#ifndef __DRAGONEGGTRIPLE_GEM_H__
#define __DRAGONEGGTRIPLE_GEM_H__

#include <iostream>
#include "cocos2d.h"
/*#include "cocos-ext.h"*/
#include <vector>
#include "DataTools.h"
#include "GemAction.h"
#include "mapInfo.h"
#include <stdio.h>
#include <stdlib.h>


USING_NS_CC ;
/*USING_NS_CC_EXT;*/
using namespace std;


class Gem: public Node
{
protected:
	Label*                 _countLabel;

	Sprite*                      _labelBackGround;

	Array*                    _actionArray;

	Vector<FiniteTimeAction*> _vecActions;

	static Gem                ***_gemStoneMatrix;

	static MapInfo              *_mapInfo;

	static   Node*             _particleNode;//Ч���ڵ�

	static   Node*             _mapNode;//��ͼ�ڵ�

	static int                  **_mapMatrix;

	static vector<AnimationWraper>* _animationWraperVector;

	static vector<MyPoint>*         _emptyPointVector;

	static vector<ConnectionArea>* _connectionAreaVector;

	static int                  _grassguyCount;

	GemType                   _type;

	int                         _state;  //��������Ԫ�أ�stateֵ����Ԫ�ز�ͬ�в�ͬ���壬����Դ�,��������Ԫ�أ� -1ʱΪ������

	int                         _frozen;  //���� 0�ޱ�����-1����

	int                         _restrain;//���� 0��������-1����

	ChangeState                 _change;//0���������ޱ仯 1,������������2�������ֱ�Ӱ�죬

	int                         _score;//�ռ�Ԫ�ؼӱ���,Ĭ��Ϊ1

	bool                        _sick;

	int                         _flag;

	int                         _scoreSum;//�÷���ʾ

	GemKind                   _GemKind;

	Sprite*                     _spr;// bi�ж�ȡ��Ԫ��

	Node*                     _frozenNode;//�����ڵ�

	Node*                     _restrainNode;//�����ڵ�

	Color3B                   _particleColor1;

	void                        createGemSpriteByName( Node *layer, Point point,bool visible,const char* name);

public:
	Gem();

	~Gem();

	GemType                   getGemType();//ȡˮ�����ͷ���

	bool                        getSick(){return _sick;}

	virtual void                setSick(bool s);

	void                        setChange(ChangeState cs){_change = cs;}

	void                        addChange(int count=1);

	ChangeState                 getChange(){return _change;}

	int                         getState(){return _state;}

	void                        setFlag(int f){_flag = f;}

	int                         getFlag(){return _flag;}

	void                        setState(int s){_state=s;}

	virtual void                addScore(int count,bool isAffect);//���countΪ0,�����score,�����ۼ�

	int                         getScore(){return _score;}

	void                        addScore(int s){_score+=s;}

	void                        setScoreSum(int s){_scoreSum = s;}

	int                         getScoreSum(){return _scoreSum;}

	void                        showScore();

	void                        hideScore();

	void                        addCollectGemScore();

	void                        addCollectGemScoreUpdate();
	
	virtual bool                equal(Gem* gem);

	static Gem*               createRandomGem(Node *layer,Point point,bool visible=true);

	static Gem*               createFixGem(GemType type,Node *layer,Point point,bool visible=true);//�����̶�ˮ��������type���� i,j��ˮ��֪���Լ�����

	static Node*              createNodeFromFile(Node *layer,Point point,const char* name);

	virtual bool                createGemSprite(Node *layer,Point point,bool visible)=0;

	static  void                setMapInfo(Node* particleNode,Node* mapNode,Gem ***matrix,MapInfo* mapInfo,vector<AnimationWraper>* aw,vector<MyPoint>* pVector,vector<ConnectionArea>* connectVector,int** mapMatrix);

	static void                 frozenerAndRestrainerPerform();

	void                        addFrozen();

	void                        addRestrain();

	GemKind                   getGemKind(){return _GemKind;}

	int                         getFrozen(){return _frozen;}
	int                         getRestrain(){return _restrain;}

	virtual bool                canMove(); //Ԫ�� �ܷ��ƶ�

	virtual void                removeCollect(const Point& dist,float time,bool playMusic);

	virtual void                removeNoCollect(bool playMusic);

	void                        removeGem();

	static int                  getUniqueNumber(int max);

	MyPoint                     getCurrentIndex(const Point& point);

	int                         numbersOfSprRunningAction();

	Color3B                   getColor3B(){return _particleColor1;}
	//===================================================================
	virtual void                highLight();//��ʾʱ����

	virtual void                stopHighLight();//ֹͣ��ʾ

	virtual void                idle();//����ʱ����

	virtual void                selected();//ѡ��ʱ����

	virtual void                fallDownStart(); //���俪ʼ

	virtual void                fallDownEnd(); //�������

	void                        sendFallDownEndMSG();

	virtual void                zhayan();

	virtual void                becomeSick();//����

	virtual void                becomeWell();//����

	void                        bright();//����ʱ������ڷ���������ʱ��ִ�еı�Ӱ��ʱ����

	void                        brightCallback();

	void                        frozenOff();

	void                        frozenOffCallback();

	void                        restrainOff();

	void                        restrainOffCallback();
	//===================================================================
	virtual void                stopSpritAction(){this->stopAllActions();}//ֹͣ���ж���

	void                        swap(Direction direction,bool reverse,bool isStart, CallFunc* callbackAction);

	void                        swapCallback();

	void                        addAction( FiniteTimeAction* action);

	void                        fallDownToEnd();

	void                        clearAllAction();

	void                        runAnimition( Node* sender,void* animitionName);

	void                        showSprite(){this->setVisible(true);}

	void                        hideSprite(){this->setVisible(false);}

	int                         getActionVectorSize(){return _actionArray->count();}

	//====================================================================
	//����Ч��
	void                        explosionParticle();

	void                        fiveMatchEffect( Node* sender,void* data);

	void                        fourMatchEffect( Node* sender,void* data);

	void                        winnerModeStart();

	void                        winnerMode();

	//=====================================================================

	virtual void                explode(MyPoint &myPoint,MyPoint &distPoint,int count,int index);//Ԫ������

	virtual void                affected(int count);//��Ӱ��

	virtual void                beforeMatch(int i,int j);//����֮ǰԤ����

	virtual void                afterMatch(int i,int j);//����֮����β

	virtual void                growOnFertile(int i,int j);

	//����
	void                        propSingle();

	void                        playExplodeRandomMusic( Node* sender,void* data);
};

class RedGem : public Gem
{
public:
	RedGem();
	bool                        createGemSprite(Node *layer, Point point,bool visible);
};

class BlueGem : public Gem
{
public:
	BlueGem();
	bool                        createGemSprite( Node *layer,  Point point,bool visible);
};

class GreenGem : public Gem
{
public:
	GreenGem();
	bool                        createGemSprite( Node *layer,  Point point,bool visible);
};

class WhiteGem : public Gem
{
public:
	WhiteGem();
	bool                        createGemSprite( Node *layer,  Point point,bool visible);
};

class YellowGem : public Gem
{
public:
	YellowGem();
	bool                        createGemSprite( Node *layer,  Point point,bool visible);
};

class PurpleGem : public Gem
{
public:
	PurpleGem();
	bool                        createGemSprite( Node *layer,  Point point,bool visible);
};

class OrangeGem : public Gem
{
public:
	OrangeGem();
	bool                        createGemSprite( Node *layer,  Point point,bool visible);
};

// class Snake : public Gem
// {
// public:
// 	Snake();
// 	bool                        createGemSprite( Node *layer,  Point point,bool visible);
// };
// 
// class Wolf : public Gem
// {
// public:
// 	Wolf();
// 	bool                        createGemSprite( Node *layer,  Point point,bool visible);
// };
// 
// 
// //============================================================================
// //����Ԫ��
// 
// class Tiger : public Gem
// {
// public:
// 	Tiger();
// 	bool                        createGemSprite( Node *layer,  Point point,bool visible);
// 	bool                        canMove(){return false;}
// 	bool                        equal(Gem* Gem){return false;}
// 	void                        setSick(bool s){}
// 	void                        highLight(){};
// 	void                        addScore(int count,bool isAffect){}
// 	void                        idle(){}
// 	//void                        bright(){}
// 	void                        affected(int count);
// 	void                        eatAnimal(MyPoint& point,MyPoint& _selectPoint,MyPoint& _nextPoint);
// 	void                        changeState(int state);
// 	void                        doEatAimation(Gem* Gem,MyPoint& distPoint, CallFuncN* callback);
// 	void                        eatAnimalCallback( Node* sender,Gem* animal);
// 	void                        refreshState();
// 	void                        selected(){}
// 	void                        jumpAction();
// 	void                        tigerJumpAnimation();
// 	void                        tigerEatAnimation();
// };
// 
// class FishInBubble : public Gem
// {
// public:
// 	FishInBubble();
// 	bool                        createGemSprite( Node *layer,  Point point,bool visible);
// 	bool                        canMove();
// 	bool                        equal(Gem* Gem);
// 	void                        setSick(bool s){}
// 	void                        highLight(){};
// 	void                        addScore(int count,bool isAffect){}
// 	void                        idle();
// 	void                        beforeMatch(int i,int j);
// 	void                        affected(int count);
// 	void                        changeState(int state);
// 	void                        selected(){};
// 
// 	//    void                        explode(MyPoint myPoint,int count, Point distPoint);
// };
// 
// class ColorfullEgg : public Gem
// {
// public:
// 	ColorfullEgg();
// 	bool                        createGemSprite( Node *layer,  Point point,bool visible);
// 	bool                        canMove();
// 	bool                        equal(Gem* Gem);
// 	void                        beforeMatch(int i,int j);
// 	void                        setSick(bool s){}
// 	void                        addScore(int count,bool isAffect){}
// 	void                        idle(){}
// 	//void                        bright(){}
// };
// 
// class Henhouse : public Gem
// {
// public:
// 	Henhouse();
// 	bool                        createGemSprite( Node *layer,  Point point,bool visible);
// 
// 	bool                        canMove();
// 
// 	bool                        equal(Gem* Gem);
// 
// 	void                        addScore(int count,bool isAffect){}
// 
// 	void                        setSick(bool s){}
// 
// 	void                        idle(){};//����
// 
// 	void                        fallDownStart(){} //���俪ʼ
// 
// 	void                        fallDownEnd(){} //�������
// 
// 	void                        becomeSick(){}//����
// 
// 	void                        becomeWell(){}//����
// 
// 	void                        beforeMatch(int i,int j);
// 
// 	void                        throwParrot(AnimationWraper& aw, CallFuncN* callback);
// 
// 	void                        throwParrotCallback( Node* sender,void* data);
// 
// 	void                        selected(){};
// 
// };
// 
// class Butterfly : public Gem
// {
// public:
// 	Butterfly();
// 	bool                        createGemSprite( Node *layer,  Point point,bool visible);
// 
// 	bool                        equal(Gem* Gem);
// 
// 	void                        setSick(bool s){}
// 
// 	void                        addScore(int count,bool isAffect){}
// 
// 	void                        idle(){}
// 
// 	void                        fallDownStart(){}; //���俪ʼ
// 
// 	void                        becomeSick(){};//����
// 
// 	void                        becomeWell(){};//����
// 
// 	void                        explode(MyPoint& myPoint,MyPoint& distPoint,int count,int index);
// 
// 	void                        affected(int count){}
// 
// 	void                        convergeMove( Point& point,MyPoint& myPoint, CallFuncN* callback);
// 
// 	void                        moveCallback( CallFuncN* callback);
// 
// 	void                        changeState(int state);
// 
// 	void                        changeStateCallback();
// };
// 
// class Chicken : public Gem
// {
// public:
// 	Chicken();
// 	bool                        createGemSprite( Node *layer,  Point point,bool visible);
// 
// 	bool                        equal(Gem* Gem);
// 
// 	void                        setSick(bool s){}
// 
// 	void                        addScore(int count,bool isAffect){}
// 
// 	void                        idle(){}
// 
// 	void                        fallDownStart(){}; //���俪ʼ
// 
// 	void                        becomeSick(){};//����
// 
// 	void                        becomeWell(){};//����
// 
// 	void                        explode(MyPoint& myPoint,MyPoint& distPoint,int count,int index);
// 
// 	void                        affected(int count){}
// 
// 	void                        convergeMove( Point& point,MyPoint& myPoint, CallFuncN* callback);
// 
// 	void                        moveCallback( CallFuncN* callback);
// 
// 	void                        changeState(int state);
// 
// 	void                        changeStateCallback();
// };
// 
// class Grassguy : public Gem
// {
// public:
// 	Grassguy();
// 	bool                        createGemSprite( Node *layer,  Point point,bool visible);
// 	bool                        canMove();
// 	bool                        equal(Gem* Gem);
// 	void                        setSick(bool s){}
// 	void                        addScore(int count,bool isAffect){}
// 
// 	void                        idle(){};//����
// 
// 	void                        fallDownStart(){}; //���俪ʼ
// 
// 	void                        becomeSick(){};//����
// 
// 	void                        becomeWell(){};//����
// 
// 	bool                        grassguyMatchParrot(Gem* Gem,MyPoint parrotPoint,MyPoint grassguyPoint);
// 
// 	void                        removeNoCollect(bool playMusic);
// 
// 	void                        runDisappearAnimation();
// };


#endif  //__DRAGONEGGTRIPLE_GEM_H__