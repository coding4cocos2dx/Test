#ifndef __DRAGONEGGTRIPLE_GAMELAYER_H__
#define __DRAGONEGGTRIPLE_GAMELAYER_H__

#include <iostream>
#include "cocos2d.h"
#include "Gem.h"
#include "MapMatrix.h"
//#include "XMLParser.h"
#include "MapInfo.h"
#include "GemAction.h"
// #include "NoTouchLayer.h"
// #include "ClassGuideLayer.h"

USING_NS_CC;

class GameLayer : public Layer
{
private:
	MapMatrix            *_mapLayer;
	MapInfo             *_mapInfo;
/*	XMLParser           *_xmlParser;*/
	Gem               ***_gemMatrix;
	int                 **_mapMatrix;
	Point             _location;
	Direction           _moveDirection;
	bool                _isMoveDone;
	MyPoint             _selectPoint;
	MyPoint             _nextPoint;

	PropKind            _propkind;

	MyPoint             highLightMovePoint;
	MyPoint             highLightPoint1;
	MyPoint             highLightPoint2;

	Node*             _particleNode;//Ч���ڵ�
	Node*             _mapNode;//��ͼ�򣬱����񣬷������غ�ƶ�����

	Point             _startPoint;
	float               _scaleFactor;

	bool                _hightLightSwitch;

	bool                _removeGemSwitch;

	vector<AnimationWraper> _animationWraperVector;

	vector<MyPoint> _emptyPointVector;

	bool                _isStartMove;

	bool                _isWinnerModeStart;

	bool                _isFallDownEnd;

	int                 _fallGemCount;

	int                 _edgeBlankDevidedSize;

	Node*             _propCar;

	Node*             _propStarstNode;

	Node*             _winnerModeStarsNode;

	Node*             _matchRepeatReminder;

	int                 _matchCollectTimes;

	int                 _continueMatchTimes;

	float               _explodeFlyTime[kMatrixWidth][kMatrixWidth];

	Point            _explodeDistArray[maxgem];

/*	ClassGuideLayer*   _classGuidLayer;*/

	Node*            _bossNode;

	int                _upIndex;

	bool               _matchDownMSGSwitch;

	EventListenerTouchOneByOne* _listener_touch;
public:
	GameLayer();
	~GameLayer();

	virtual bool init();

	virtual void onEnter();
	virtual void onExit();

	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);

	void changeGem(Direction direction);
	void runIdleAnimation(float dt);

	void findPotentialOrRenew();
	void highLightGem(float dt);
	void stopHighLight();

	void renewAllGem();
	void firstTimeInit();
	void setTouchEnable();
	void setTouchDisable();
	void stopRemindAndIdle();

	//����״̬������Ŷ���
	void runAllAnimationAndAction(Node* sender,void* param);

	void beforeMatch();

	void afterMatch();

	void runOneKindofAnimation(AnimationWraper aw,CCCallFuncND* callback);

	void gemFallCallback();//����ص����¼���

	void gemFallNotification(Ref *obj);

	void fiveMatchEffect(int index,MyPoint& point,CCCallFuncND* callback);

	void fourMatchEffect(int index,CCCallFuncND* callback);

	void crossMatchEffect(int index1,int index2,CCCallFuncND* callback);

	Point calculateStartPoint(float edgeWidth,Size& winSize,float scaleFactor);

	void addShieldLayer();

	void removeShieldLayer();

	void shining();

	void winnerMode(Ref *obj);

	void outStage(Ref* object);

	void sendOutStageMessage();

	void matchRepeatCallback();
	CREATE_FUNC(GameLayer);
	//����
	void eraseSingle(int i,int j);//ȥ������

	void eraseRow(int row);//ȥ��һ��

	void eraseOneType(GemType type);//ȥ��һ��

	void addOneToCollect();//�ռ��ļ�1

	void cureSick();//�����ı��

	void propUseStart(Ref* object);

	void propUseCancel();

	void propUseOver();

	bool propUse(MyPoint& point);

	void propRemoveRow(float dt);

	void propUnschedule();

	void propShiningInBackgroud(Node* sender,void* data);

	void winnerModeShiningInBackgroud(Node* sender,void* data);

	void playContinueMatchMusic(int matchTimes);

	void initExplodeFlyTime();

	float getExplodeFlyTime(MyPoint& point);

	void removeGem(Node* sender,void* data);

	void buyFiveSteps(Ref *obj);
	//����̳�
	void addClassLayer();

	//�̳����ƶ�С����
	void classGuidMove(Ref* obj);

	void removeGameLayer();

	void showSparkle(Node* sender);

	void fallDownToEnd(Node* sender,void* data);

	void bossAppear();

	void bossAppearCallback();


};

#endif	//__DRAGONEGGTRIPLE_GAMELAYER_H__

