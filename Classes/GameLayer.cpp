#include "GameLayer.h"
#include "SwallowLayer.h"
#include <math.h>
#include <time.h>

GameLayer::GameLayer()
	:_isMoveDone(true)
	,_isStartMove(false)
	,_isFallDownEnd(false)
	,_fallGemCount(0)
	,_edgeBlankDevidedSize(10)
	,_isWinnerModeStart(false)
	,_propkind(PropNone)
	,_propCar(NULL)
	,_propStarstNode(NULL)
	,_winnerModeStarsNode(NULL)
	,_matchCollectTimes(0)
	,_matchRepeatReminder(NULL)
	,_continueMatchTimes(0)
/*	,_classGuidLayer(NULL)//自身负责移除*/
	,_hightLightSwitch(true)
	,_bossNode(NULL)
	,_upIndex(NULL)
	,_removeGemSwitch(true)
	,_matchDownMSGSwitch(false)
{
	highLightMovePoint.setPosition(-1, -1);
	highLightPoint1.setPosition(-1, -1);
	highLightPoint2.setPosition(-1, -1);
	_selectPoint.reset();
	_nextPoint.reset();
}

GameLayer::~GameLayer()
{
	for(int i=0;i<kMatrixWidth;i++)
	{
		for(int j=0;j<kMatrixWidth;j++)
		{
			if(_gemMatrix[i][j])
			{
				_gemMatrix[i][j]->removeFromParentAndCleanup(true);
				_gemMatrix[i][j]=NULL;
			}
		}
	}

	for(int i=0;i<kMatrixWidth;i++)
	{
		free(_gemMatrix[i]);
		free(_mapMatrix[i]);
	}
	free(_gemMatrix);
	free(_mapMatrix);

	if(_mapLayer)
	{
		delete _mapLayer;
	}

	if(_particleNode)
	{
		_particleNode->removeFromParentAndCleanup(true);
	}
	if(_winnerModeStarsNode)
	{
		_winnerModeStarsNode->removeFromParentAndCleanup(true);
	}
	if(_propStarstNode)
	{
		_propStarstNode->removeFromParentAndCleanup(true);
	}
	if(_mapNode)
	{
		_mapNode->removeFromParentAndCleanup(true);
	}
	this->removeAllChildrenWithCleanup(true);
	this->removeFromParentAndCleanup(true);
}

bool GameLayer::init()
{
	if (!Layer::init()) {
		return false;
	}


// 	GemAction::getInstance().playBackgroudMusic(NULL, (void*)kMusic_Backgroud_GameLayer);
// 	// 根据地图 产生水果 矩阵
// 	XMLParser* _xmlParser = XMLParser::GetInstance();
// 
// 	_mapInfo = _xmlParser->getSelectMap();
// 
// 	int** tempMatrix = _mapInfo->getMap();

	_mapMatrix = (int**)malloc(sizeof(int*)*kMatrixWidth);

	for(int i=0;i<kMatrixWidth;i++)
	{
		_mapMatrix[i] = (int*)malloc(sizeof(int)*kMatrixWidth);

		for(int j=0;j<kMatrixWidth;j++)
		{
			_mapMatrix[i][j]=MAP_TEST[i][j];
		}
	}

	Size winSize = Director::getInstance()->getWinSize();

	float screenRate = winSize.height/winSize.width;

	if(screenRate>1.7)
	{
		_edgeBlankDevidedSize = 50;
	}
	else if(screenRate<1.4)
	{
		_edgeBlankDevidedSize = 10;
	}
	else
	{
		_edgeBlankDevidedSize = 20;
	}

	float edgeBlank = winSize.width/_edgeBlankDevidedSize;

	this->setContentSize(CCSizeMake(winSize.width-edgeBlank*2,winSize.height-edgeBlank*2));

	this->setAnchorPoint(Point(0, 0));

	_scaleFactor = (winSize.width-edgeBlank*2)/(kElementSize*kMatrixWidth);

	this->setScale(_scaleFactor);

	_startPoint=calculateStartPoint(edgeBlank, winSize, _scaleFactor);

	_particleNode = Node::create();//粒子效果节点,z=1

	_mapNode = Node::create();//地图节点z=-1

	Point startPoint =Point(-winSize.width*kGameLayerStartMultiple,(winSize.height-(winSize.width-edgeBlank*2))/2);

	Point endPoint = _startPoint;//ccp(edgeBlank,(winSize.height-(winSize.width-edgeBlank*2))/2);

	this->addChild(_particleNode,kParticleZOrder);

	this->addChild(_mapNode,kMapZOrder);

	_gemMatrix = (Gem***)malloc(sizeof(Gem**)*kMatrixWidth);

	for(int i=0;i<kMatrixWidth;i++)
	{
		_gemMatrix[i] = (Gem**)malloc(sizeof(Gem*)*kMatrixWidth);

		for(int j=0;j<kMatrixWidth;j++)
		{
			_gemMatrix[i][j]=NULL;
		}
	}

	int **fruitMap = _mapInfo->getGemMap();

// 	int fruitMap[kMatrixWidth][kMatrixWidth] = 
// 	{
// 		{1,2,3,4,5,6,7,1},
// 		{2,3,4,5,6,7,1,2},
// 		{3,4,5,6,7,1,2,3},
// 		{4,5,6,7,1,2,3,4},
// 		{5,6,7,1,2,3,4,5},
// 		{6,7,1,2,3,4,5,6},
// 		{7,1,2,3,4,5,6,7},
// 		{1,2,3,4,5,6,7,1},
// 	};

	_mapLayer = new MapMatrix(_mapMatrix, _gemMatrix, kMatrixWidth,_mapInfo);

	_mapLayer->createMapFrame(_mapNode);

	Gem::setMapInfo(_particleNode,_mapNode, _gemMatrix, _mapInfo,&_animationWraperVector,&_emptyPointVector,&(_mapLayer->connectionAreaVector),_mapMatrix);

	for(int i = 0;i<kMatrixWidth;i++)
	{
		for(int j = 0;j<kMatrixWidth;j++)
		{
			GemType type =(GemType)fruitMap[i][j];
/*			CCLOG("%d",fruitMap[i][j]);*/
			_gemMatrix[i][j] = Gem::createFixGem(type,this,Point(kStartX + i * kElementSize + kElementSize/2, kStartY + j * kElementSize + kElementSize/2),true);

		}
	}
	if(/*_mapInfo->getClassKind()==ClassNone||(_mapInfo->getClassKind()>=ClassPropSingle&&_mapInfo->getClassKind()<=ClassPropCureSick)*/true)
	{
		this->firstTimeInit();
	}
	//放鸟窝，冰冻，藤条
// 	for (int i=0; i<kMatrixWidth; i++)
// 	{
// 		for(int j=0;j<kMatrixWidth;j++)
// 		{
// 			if(_mapInfo->isTengtiaoMapHas(i, j)>0)
// 			{
// 				_gemMatrix[i][j]->addRestrain();
// 			}
// 
// 			if(_mapInfo->isFrozenMapHas(i, j)>0)
// 			{
// 				_gemMatrix[i][j]->addFrozen();
// 			}
// 
// 			if(_mapInfo->isBirdNestMapHas(i, j))
// 			{
// 				CCSprite* birdNest = CCSprite::createWithSpriteFrameName("coloreggnest.png");
// 				birdNest->setPosition(ccp(kStartX + i * kElementSize + kElementSize/2, kStartY + j * kElementSize + kElementSize/2));
// 				_mapNode->addChild(birdNest,1);
// 			}
// 			if(_mapMatrix[i][j]==Fertile)
// 			{
// 				Gem::createNodeFromFile(_mapNode, ccp(kStartX+(i+0.5)*kElementSize,kStartY+(j+0.5)*kElementSize), "rich_soil.ccbi");
// 			}
// 			else if(_mapMatrix[i][j]==InFertile)
// 			{
// 				Gem::createNodeFromFile(_mapNode, ccp(kStartX+(i+0.5)*kElementSize,kStartY+(j+0.5)*kElementSize), "poor_soil.ccbi");
// 			}
// 		}
// 	}

	this->setPosition(startPoint);

	this->setTouchDisable();

	this->runAction(Sequence::create(EaseBackInOut::create(MoveTo::create(kGameLayerFlyTime, endPoint)),CallFunc::create(this,callfunc_selector(GameLayer::shining)),NULL));

	_listener_touch = EventListenerTouchOneByOne::create();
	_listener_touch->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan,this);
	_listener_touch->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved,this);
	_listener_touch->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded,this);
	_listener_touch->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener_touch, this);

	return true;
}

void GameLayer::onEnter()
{
	Layer::onEnter();

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::winnerMode), kMSG_WinnerModeStart, NULL);

// 	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::propUseStart),kMSG_PropUseStart, NULL);
// 
// 	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::propUseCancel),kMSG_PropUseCancel, NULL);

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::outStage),kMSG_GameLayer_BeforeOutStage, NULL);

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::classGuidMove),kMSG_ClassGuid_MoveGem, NULL);

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::gemFallNotification),kMSG_GemFallDown, NULL);

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameLayer::buyFiveSteps),kMSG_BuyFiveSteps, NULL);
}

void GameLayer::onExit()
{
	NotificationCenter::getInstance()->removeObserver(this, kMSG_WinnerModeStart);
	NotificationCenter::getInstance()->removeObserver(this, kMSG_PropUseStart);
	NotificationCenter::getInstance()->removeObserver(this, kMSG_PropUseCancel);
	NotificationCenter::getInstance()->removeObserver(this, kMSG_GameLayer_BeforeOutStage);
	NotificationCenter::getInstance()->removeObserver(this, kMSG_ClassGuid_MoveGem);
	NotificationCenter::getInstance()->removeObserver(this, kMSG_GemFallDown);
	NotificationCenter::getInstance()->removeObserver(this, kMSG_BuyFiveSteps);

	_eventDispatcher->removeEventListenersForTarget(this);

	Layer::onExit();
}

bool GameLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	_isMoveDone = true;

	_location = pTouch->getLocation();

	int a = _location.x-kStartX-_startPoint.x;

	int b = _location.y-kStartY-_startPoint.y;

	if(b<0||a<0) return false;

	int i = a/(kElementSize*_scaleFactor);
	int j = b/(kElementSize*_scaleFactor);

	if(i>=0&&i<kMatrixWidth&&j>=0&&j<kMatrixWidth&&_gemMatrix[i][j])
	{
		if(_propkind!=PropNone)
		{
			setTouchDisable();

			MyPoint point(i,j);

			if(!propUse(point))
			{
				setTouchEnable();
			}

			return false;
		}

		stopHighLight();

		_isMoveDone = false;

		_gemMatrix[i][j]->selected();

		_selectPoint.setPosition(i, j);

		return true;
	}
	else
	{
		_isMoveDone = true;

		return false;
	}
}

void GameLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	int a = _location.x-kStartX-_startPoint.x;

	int b = _location.y-kStartY-_startPoint.y;

	if(b<0||a<0) return;

	int i = a/(kElementSize*_scaleFactor);
	int j = b/(kElementSize*_scaleFactor);

	Point moveDirection = pTouch->getLocation();

	float distance = ccpDistance(_location, moveDirection);

	if(distance<kElementSize*_scaleFactor*0.2)
	{
		return;
	}
	if (!_isMoveDone)
	{

		if (fabs(moveDirection.x - _location.x) > fabs(moveDirection.y - _location.y))
		{
			if (moveDirection.x - _location.x > 0)
			{
				i += 1;
				_moveDirection = Right;
			}
			else
			{
				i -= 1;
				_moveDirection = Left;
			}
		}
		else
		{
			if (moveDirection.y - _location.y > 0)
			{
				j += 1;
				_moveDirection = Up;
			}
			else
			{
				j -= 1;
				_moveDirection = Down;
			}
		}

		if(i>=0&&i<kMatrixWidth&&j>=0&&j<kMatrixWidth)
		{
			_isMoveDone = true;

			_nextPoint.setPosition(i, j);
		}
		else
		{
			return;
		}

		Gem *sGem = _gemMatrix[_selectPoint.x][_selectPoint.y];

		Gem *nGem = _gemMatrix[_nextPoint.x][_nextPoint.y];

		if (sGem&&nGem&&sGem->canMove()&&nGem->canMove())
		{
			_hightLightSwitch = false;

			stopHighLight();

			setTouchDisable();

			changeGem(_moveDirection);
		}
	}
}
void GameLayer::changeGem(Direction direction)
{
	PotentialArea potentialArea;

/*	Grassguy* gg = NULL;*/

	bool canChange = false,grassguyMatch=false;

	_continueMatchTimes = 0;

// 	if(_gemMatrix[_selectPoint.x][_selectPoint.y]->getGemType()==grassguy)
// 	{
// 		gg = (Grassguy*)_gemMatrix[_selectPoint.x][_selectPoint.y];
// 
// 		grassguyMatch = gg->grassguyMatchParrot(_gemMatrix[_nextPoint.x][_nextPoint.y],_nextPoint,_selectPoint);
// 	}
// 	else if(_gemMatrix[_nextPoint.x][_nextPoint.y]->getGemType()==grassguy)
// 	{
// 		gg = (Grassguy*)_gemMatrix[_nextPoint.x][_nextPoint.y];
// 
// 		grassguyMatch = gg->grassguyMatchParrot(_gemMatrix[_selectPoint.x][_selectPoint.y],_selectPoint,_nextPoint);
// 	}

	for(int i=0;i<_mapLayer->potentialAreaVector.size();i++)
	{
		potentialArea = _mapLayer->potentialAreaVector[i];

		bool selectRight =potentialArea.movePoint.x == _selectPoint.x&&potentialArea.movePoint.y == _selectPoint.y&&_moveDirection==potentialArea.diretion;

		bool nextRight = potentialArea.movePoint.x == _nextPoint.x&&potentialArea.movePoint.y == _nextPoint.y&&Direction(Down-_moveDirection) == potentialArea.diretion;

		if(selectRight||nextRight)
		{
			canChange = true;
			break;
		}
	}

	if(grassguyMatch)
	{
		_matchDownMSGSwitch = true;

		Gem *tempGem = _gemMatrix[_selectPoint.x][_selectPoint.y];

		_gemMatrix[_selectPoint.x][_selectPoint.y] = _gemMatrix[_nextPoint.x][_nextPoint.y];

		_gemMatrix[_nextPoint.x][_nextPoint.y] = tempGem;

		CCCallFuncND* callbackAction = CCCallFuncND::create(this,callfuncND_selector(GameLayer::runAllAnimationAndAction),(void*)1);

		_gemMatrix[_nextPoint.x][_nextPoint.y]->swap(_moveDirection, false, false,callbackAction);

		_gemMatrix[_selectPoint.x][_selectPoint.y]->swap(Direction(Down-_moveDirection), false,true, NULL);

		_isStartMove = true;

/*		_mapInfo->moveOneStep();*/   //步数 -1

		NotificationCenter::getInstance()->postNotification(kMSG_UpdateMoves);
	}
	else if(canChange)
	{
		_matchDownMSGSwitch = true;

		Gem *tempGem = _gemMatrix[_selectPoint.x][_selectPoint.y];

		_gemMatrix[_selectPoint.x][_selectPoint.y] = _gemMatrix[_nextPoint.x][_nextPoint.y];

		_gemMatrix[_nextPoint.x][_nextPoint.y] = tempGem;

		if(_mapMatrix[_selectPoint.x][_selectPoint.y]==Fertile)
		{
			_gemMatrix[_selectPoint.x][_selectPoint.y]->setSick(false);
		}
		else if(_mapMatrix[_selectPoint.x][_selectPoint.y]==InFertile)
		{
			_gemMatrix[_selectPoint.x][_selectPoint.y]->setSick(true);
		}
		if(_mapMatrix[_nextPoint.x][_nextPoint.y]==Fertile)
		{
			_gemMatrix[_nextPoint.x][_nextPoint.y]->setSick(false);
		}
		else if(_mapMatrix[_nextPoint.x][_nextPoint.y]==InFertile)
		{
			_gemMatrix[_nextPoint.x][_nextPoint.y]->setSick(true);
		}
		CallFunc* callbackAction = CallFunc::create(this,callfunc_selector(GameLayer::beforeMatch));

		_gemMatrix[_nextPoint.x][_nextPoint.y]->swap(_moveDirection, false, true,callbackAction);

		_gemMatrix[_selectPoint.x][_selectPoint.y]->swap(Direction(Down-_moveDirection),false, false, NULL);

		_isStartMove = true;

/*		_mapInfo->moveOneStep();*/   //步数 -1

		NotificationCenter::getInstance()->postNotification(kMSG_UpdateMoves);
	}
	else
	{
		CallFunc* callbackAction = CallFunc::create(this,callfunc_selector(GameLayer::findPotentialOrRenew));

		_gemMatrix[_selectPoint.x][_selectPoint.y]->swap(_moveDirection,true,true,callbackAction);

		_gemMatrix[_nextPoint.x][_nextPoint.y]->swap(Direction(Down-_moveDirection), true,false,NULL);

		_hightLightSwitch = true;
	}
}


void GameLayer::highLightGem(float dt)
{
	_mapLayer->findPotential();

	if(!_mapLayer->potentialAreaVector.empty()&&_hightLightSwitch)
	{
		highLightMovePoint = _mapLayer->potentialAreaVector[0].movePoint;
		highLightPoint1 = _mapLayer->potentialAreaVector[0].point1;
		highLightPoint2 = _mapLayer->potentialAreaVector[0].point2;

		_gemMatrix[highLightMovePoint.x][highLightMovePoint.y]->highLight();
		_gemMatrix[highLightPoint1.x][highLightPoint1.y]->highLight();
		_gemMatrix[highLightPoint2.x][highLightPoint2.y]->highLight();
	}
}
void GameLayer::stopHighLight()
{
	if(!_mapLayer->potentialAreaVector.empty())
	{
		highLightMovePoint = _mapLayer->potentialAreaVector[0].movePoint;
		highLightPoint1 = _mapLayer->potentialAreaVector[0].point1;
		highLightPoint2 = _mapLayer->potentialAreaVector[0].point2;

		_gemMatrix[highLightMovePoint.x][highLightMovePoint.y]->stopHighLight();
		_gemMatrix[highLightPoint1.x][highLightPoint1.y]->stopHighLight();
		_gemMatrix[highLightPoint2.x][highLightPoint2.y]->stopHighLight();
	}
}
void GameLayer::renewAllGem()
{
	NotificationCenter::getInstance()->postNotification(kMSG_RenewAll);

	MyPoint pointArray[kMatrixWidth*kMatrixWidth];

	Gem* gemArray[kMatrixWidth*kMatrixWidth];

	int i,j,max_size=0;

	for(i=0;i<kMatrixWidth*kMatrixWidth;i++)
	{
		pointArray[i].setPosition(0, 0);
		gemArray[i]=NULL;
	}

	for(j=0;j<kMatrixWidth;j++)
	{
		for(i=0;i<kMatrixWidth;i++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->canMove()&&_gemMatrix[i][j]->getGemType()!=colorfullegg)
			{
				gemArray[max_size] = _gemMatrix[i][j];
				pointArray[max_size] = MyPoint(i,j);
				max_size++;
			}
		}
	}
	srand ((unsigned)time(nullptr));;
	while(_mapLayer->potentialAreaVector.empty())
	{
		do{
			for(i=max_size;i>0;i--)
			{
				int index = rand()%i;

				MyPoint p = pointArray[index];

				_gemMatrix[p.x][p.y] = gemArray[i-1];

				pointArray[index].swap(pointArray[i-1]);
			}

			_mapLayer->findConnection();

		}while(!_mapLayer->connectionAreaVector.empty());

		_mapLayer->findPotential();
	}

	GemAction* caller = &(GemAction::getInstance());

	FiniteTimeAction* action = NULL;

	for(i =0;i<max_size;i++)
	{
		if(i==0)
		{
			action = Sequence::create(DelayTime::create(kRenewGemWaitTime)/*,CCCallFuncND::create(caller,callfuncND_selector(GemAction::playEffectMusic),(void*)kMusic_Gem_RenewAll)*/,GemAction::getInstance().getGemRenewAction(pointArray[i]),DelayTime::create((rand()%3)*0.2),CallFunc::create(gemArray[i],callfunc_selector(Gem::highLight)),NULL);
		}
		else
		{
			action = Sequence::create(DelayTime::create(kRenewGemWaitTime),GemAction::getInstance().getGemRenewAction(pointArray[i]),DelayTime::create((rand()%3)*0.2),CallFunc::create(gemArray[i],callfunc_selector(Gem::highLight)),NULL);
		}

		if(max_size-1==i)
		{
			FiniteTimeAction* actionWithCallback = Sequence::create(CallFunc::create(this,callfunc_selector(GameLayer::addShieldLayer)),action,DelayTime::create(0.5),CallFunc::create(this,callfunc_selector(GameLayer::removeShieldLayer)),CallFunc::create(this, callfunc_selector(GameLayer::beforeMatch)),NULL);

			gemArray[i]->runAction(actionWithCallback);

			return;
		}
		gemArray[i]->runAction(action);
	}

// 	MapInfo* mapInfo = XMLParser::GetInstance()->getSelectMap();
// 
// 	char levelNum[5];
// 
// 	memset(levelNum, 0, sizeof(levelNum));
// 
// 	sprintf(levelNum, "%d",mapInfo->getLevelNum());
// 
// 	JniGameForAndroidNS::sendGoogleTrackerMsg(kGA_category2, "重排",levelNum, 0);
}
void GameLayer::findPotentialOrRenew()
{
	_mapLayer->findPotential();

	if(_mapLayer->potentialAreaVector.empty())
	{
		renewAllGem();
		return ;
	}
	else
	{
		this->setTouchEnable();
		return ;
	}
}
void GameLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
}

void GameLayer::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
}

void GameLayer::setTouchEnable()
{
	this->removeChildByTag(9999);

	this->unschedule(schedule_selector(GameLayer::highLightGem));

	this->unschedule(schedule_selector(GameLayer::runIdleAnimation));

	this->schedule(schedule_selector(GameLayer::highLightGem),3);

	this->schedule(schedule_selector(GameLayer::runIdleAnimation), 4);
}
void GameLayer::setTouchDisable()
{
	SwallowLayer *noTouchLayer = SwallowLayer::create(); // 屏蔽层

	this->addChild(noTouchLayer,999,9999);

	this->unschedule(schedule_selector(GameLayer::highLightGem));

	this->unschedule(schedule_selector(GameLayer::runIdleAnimation));
}

void GameLayer::stopRemindAndIdle()
{
	this->unschedule(schedule_selector(GameLayer::highLightGem));

	this->unschedule(schedule_selector(GameLayer::runIdleAnimation));
}
void GameLayer::runIdleAnimation(float dt)
{
	int i,j,k=0;
	srand ((unsigned)time(nullptr));
	while(k<3)
	{
		do
		{
			i = rand()%kMatrixWidth;

			j = rand()%kMatrixWidth;

		}while(!_gemMatrix[i][j]);

		if(_gemMatrix[i][j])
		{
			_gemMatrix[i][j]->idle();
		}

		k++;
	}
}

void GameLayer::fiveMatchEffect(int index,MyPoint& point,CCCallFuncND* callback)
{
	//1，播放聚集能量粒子效果,同时本排元素高亮放大
	//2，找到屏幕中可以被影响的元素，生成光点飞向他们。
	//3，击中元素播放高亮放大效果
	//4，聚合类不播放该粒子效果

	vector<ConnectionArea>::iterator it;

	ConnectionArea ca = _mapLayer->connectionAreaVector[index];
	MyPoint particlePoint = ca.particlePoint;
	Point startPoint = _gemMatrix[particlePoint.x][particlePoint.y]->getPosition();

	GemType type = _gemMatrix[particlePoint.x][particlePoint.y]->getGemType();

	int flagMatrix[kMatrixWidth][kMatrixWidth] = {0};

	int i,j,max=0;

	Sprite* spriteArray[kMatrixWidth*kMatrixWidth]={0};

	MyPoint distArray[kMatrixWidth*kMatrixWidth];
	//地图中与5连相同种类元素，并且是可以移动的元素标记（非冰冻，束缚）
	for(i=0;i<kMatrixWidth;i++)
	{
		for (j=0;j<kMatrixWidth; j++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemType()==type&&_gemMatrix[i][j]->canMove())
			{
				flagMatrix[i][j]=1;
			}
		}
	}

	for(it=_mapLayer->connectionAreaVector.begin();it!=_mapLayer->connectionAreaVector.end();it++)
	{
		if (it->direction == Up)
		{
			for (j = it->startPoint.y,i = it->startPoint.x; j < it->startPoint.y + it->count; j++)
			{
				if(_gemMatrix[i][j]->getGemType()==type)
				{
					flagMatrix[i][j]=0;
				}
			}
		}
		else
		{
			for (i = it->startPoint.x,j = it->startPoint.y; i < it->startPoint.x + it->count; i++)
			{
				if(_gemMatrix[i][j]->getGemType()==type)
				{
					flagMatrix[i][j]=0;
				}
			}
		}
	}
	//高亮闪
	if (ca.direction == Up)
	{
		for (j = ca.startPoint.y,i = ca.startPoint.x; j < ca.startPoint.y + ca.count; j++)
		{
			_gemMatrix[i][j]->bright();

		}
	}
	else
	{
		for (i = ca.startPoint.x,j = ca.startPoint.y; i < ca.startPoint.x + ca.count; i++)
		{
			_gemMatrix[i][j]->bright();
		}
	}

// 	for(i=0;i<kMatrixWidth;i++)
// 	{
// 		for (j=0; j<kMatrixWidth; j++)
// 		{
// 			if(flagMatrix[i][j]==1)
// 			{
// 				Sprite* spr = Sprite::create("sparkle2.png");
// 				spr->setPosition(startPoint);
// 				_particleNode->addChild(spr);
// 				spriteArray[max] = spr;
// 				distArray[max++].setPosition(i, j);
// 			}
// 		}
// 	}

	if(max==0)
	{
		this->runAction(callback);
	}

// 	for (i=0; i<max; i++)
// 	{
// 		Gem* distGem =_gemMatrix[distArray[i].x][distArray[i].y];
// 
// 		Sequence* seq=NULL;
// 		ccBezierConfig config;
// 		Point controlPoint = GemAction::getInstance().getControlPoint(startPoint, distGem->getPosition(),(Director::getInstance()->getWinSize().width)*kParticleBezierScale);
// 		config.controlPoint_1 = controlPoint;
// 		config.controlPoint_2 = controlPoint;
// 		config.endPosition = distGem->getPosition();
// 
// 		if(i==max-1)
// 		{
// 			seq = Sequence::create(Spawn::create(RotateTo::create(kSparkleFlyTime, 60),BezierTo::create(kSparkleFlyTime, config),NULL),callback,CCCallFuncND::create(distGem, callfuncND_selector(Gem::fiveMatchEffect),(void*)NULL),NULL);
// 		}
// 		else
// 		{
// 			seq = Sequence::create(Spawn::create(RotateTo::create(kSparkleFlyTime, 60),BezierTo::create(kSparkleFlyTime, config),NULL),CCCallFuncND::create(distGem, callfuncND_selector(Gem::fiveMatchEffect), (void*)NULL),NULL);
// 		}
// 
// 		spriteArray[i]->runAction(seq);
// 
// 		distGem->explode(distArray[i], distArray[i], 0, 0);
// 	}
}

void GameLayer::fourMatchEffect(int index,cocos2d::CCCallFuncND *callback)
{
	//1,播放粒子效果
	//2,飞出光点
	vector<ConnectionArea>::iterator it;

	ConnectionArea ca = _mapLayer->connectionAreaVector[index];

	MyPoint particlePoint = ca.particlePoint;

	Point startPoint = _gemMatrix[particlePoint.x][particlePoint.y]->getPosition();

	Gem* fruitArray[kMatrixWidth]={0};

	int i,j,max=0;

	//四消除粒子
// 	DDParticleFour* fourEffect = DDParticleFour::create(_particleNode);
// 
// 	fourEffect->setPosition(startPoint);
// 
// 	fourEffect->setScale(kParticleScaleParm);
// 
// 	if(ca.direction==Up)
// 	{
// 		fourEffect->setRotation(90);
// 	}
// 
// 	_particleNode->addChild(fourEffect);

	//高亮闪
	if (ca.direction == Up)
	{
		for (j = ca.startPoint.y,i = ca.startPoint.x; j < ca.startPoint.y + ca.count; j++)
		{
			_gemMatrix[i][j]->bright();
		}
		for(i=ca.startPoint.x,j=0;j<ca.startPoint.y;j++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
			{
				fruitArray[max++] = _gemMatrix[i][j];
			}
		}
		for(i=ca.startPoint.x,j=ca.startPoint.y+ca.count;j<kMatrixWidth;j++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
			{
				fruitArray[max++] = _gemMatrix[i][j];
			}
		}
	}
	else
	{
		for (i = ca.startPoint.x,j = ca.startPoint.y; i < ca.startPoint.x + ca.count; i++)
		{
			_gemMatrix[i][j]->bright();
		}
		for(i=0,j=ca.startPoint.y;i<ca.startPoint.x;i++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
			{
				fruitArray[max++] = _gemMatrix[i][j];
			}
		}
		for(i=ca.startPoint.x+ca.count,j=ca.startPoint.y;i<kMatrixWidth;i++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
			{
				fruitArray[max++] = _gemMatrix[i][j];
			}
		}
	}
	if(max==0)
	{
		this->runAction(Sequence::create(DelayTime::create(kSparkleFlyTime),callback,NULL));
	}
	this->runAction(Sequence::create(/*DelayTime::create(kSparkleFlyTime),*/callback,NULL));
// 	for(i=0;i<max;i++)
// 	{
// 		Sequence* seq = NULL;
// 
// 		Sprite* spr = Sprite::create("sparkle1.png");
// 
// 		spr->setPosition(startPoint);
// 
// 		spr->setScale(3);
// 
// 		_particleNode->addChild(spr);
// 
// 		ccBezierConfig config;
// 
// 		Point controlPoint = GemAction::getInstance().getControlPoint(spr->getPosition(), fruitArray[i]->getPosition(), (Director::getInstance()->getWinSize().width)*kParticleBezierScale);
// 
// 		config.controlPoint_1 = controlPoint;
// 
// 		config.controlPoint_2 = controlPoint;
// 
// 		config.endPosition = fruitArray[i]->getPosition();
// 
// 		if(i==max-1)
// 		{
// 			seq = Sequence::create(BezierTo::create(kSparkleFlyTime, config),callback,CCCallFuncND::create(fruitArray[i], callfuncND_selector(Gem::fourMatchEffect), (void*)NULL),NULL);
// 		}
// 		else
// 		{
// 			seq = Sequence::create(BezierTo::create(kSparkleFlyTime, config),CCCallFuncND::create(fruitArray[i], callfuncND_selector(Gem::fourMatchEffect), (void*)NULL),NULL);
// 		}
// 
// 		spr->runAction(seq);
// 	}

}
void GameLayer::crossMatchEffect(int index1,int index2, CCCallFuncND* callback)
{
	vector<ConnectionArea>::iterator it;

	ConnectionArea ca1 = _mapLayer->connectionAreaVector[index1];

	ConnectionArea ca2 = _mapLayer->connectionAreaVector[index2];

	MyPoint particlePoint = ca1.centerPoint;

	Point startPoint = _gemMatrix[particlePoint.x][particlePoint.y]->getPosition();

	int fruitArray[kMatrixWidth][kMatrixWidth]={0};

	int i,j;

	int max = 0;

	//十字消除效果
// 	DDParticleTen* crossEffect = DDParticleTen::create(_particleNode);
// 
// 	crossEffect->setPosition(startPoint);
// 
// 	crossEffect->setScale(kParticleScaleParm);
// 
// 	_particleNode->addChild(crossEffect);

	if (ca1.direction == Up)
	{
		for (j = ca1.startPoint.y,i = ca1.startPoint.x; j < ca1.startPoint.y + ca1.count; j++)
		{
			_gemMatrix[i][j]->bright();
		}
		for(i=ca1.startPoint.x,j=0;j<ca1.startPoint.y;j++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
			{
				fruitArray[i][j] = 1;
				max++;
			}
		}
		for(i=ca1.startPoint.x,j=ca1.startPoint.y+ca1.count;j<kMatrixWidth;j++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
			{
				fruitArray[i][j]= 1;
				max++;
			}
		}
	}
	else
	{
		for (i = ca1.startPoint.x,j = ca1.startPoint.y; i < ca1.startPoint.x + ca1.count; i++)
		{
			_gemMatrix[i][j]->bright();
		}
		for(i=0,j=ca1.startPoint.y;i<ca1.startPoint.x;i++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
			{
				fruitArray[i][j] = 1;
				max++;
			}
		}
		for(i=ca1.startPoint.x+ca1.count,j=ca1.startPoint.y;i<kMatrixWidth;i++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
			{
				fruitArray[i][j] = 1;
				max++;
			}
		}
	}

	if (ca2.direction == Up)
	{
		for (j = ca2.startPoint.y,i = ca2.startPoint.x; j < ca2.startPoint.y + ca2.count; j++)
		{
			_gemMatrix[i][j]->bright();
		}
		for(i=ca2.startPoint.x,j=0;j<ca2.startPoint.y;j++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
			{
				fruitArray[i][j]=1;
				max++;
			}
		}
		for(i=ca2.startPoint.x,j=ca2.startPoint.y+ca2.count;j<kMatrixWidth;j++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
			{
				fruitArray[i][j]=1;
				max++;
			}
		}
	}
	else
	{
		for (i = ca2.startPoint.x,j = ca2.startPoint.y; i < ca2.startPoint.x + ca2.count; i++)
		{
			_gemMatrix[i][j]->bright();
		}
		for(i=0,j=ca2.startPoint.y;i<ca2.startPoint.x;i++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
			{
				fruitArray[i][j]=1;
				max++;
			}
		}
		for(i=ca2.startPoint.x+ca2.count,j=ca2.startPoint.y;i<kMatrixWidth;i++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
			{
				fruitArray[i][j]=1;
				max++;
			}
		}
	}
	if(max==0)
	{
		this->runAction(Sequence::create(DelayTime::create(kSparkleFlyTime),callback,NULL));
	}
	this->runAction(Sequence::create(/*DelayTime::create(kSparkleFlyTime),*/callback,NULL));
// 	for(i=0;i<kMatrixWidth;i++)
// 	{
// 		for(j=0;j<kMatrixWidth;j++)
// 		{
// 			if(fruitArray[i][j]==1)
// 			{
// 				max--;
// 
// 				Sequence* seq = NULL;
// 				Sprite* spr = Sprite::create("sparkle1.png");
// 				spr->setPosition(startPoint);
// 				spr->setScale(3);
// 				_particleNode->addChild(spr);
// 
// 				ccBezierConfig config;
// 				Point controlPoint = GemAction::getInstance().getControlPoint(spr->getPosition(), _gemMatrix[i][j]->getPosition(), (Director::getInstance()->getWinSize().width)*kParticleBezierScale);
// 				config.controlPoint_1 = controlPoint;
// 				config.controlPoint_2 = controlPoint;
// 				config.endPosition = _gemMatrix[i][j]->getPosition();
// 
// 				if(max==0)
// 				{
// 					seq = Sequence::create(BezierTo::create(kSparkleFlyTime, config),callback,CCCallFuncND::create(_gemMatrix[i][j], callfuncND_selector(Gem::fourMatchEffect),(void*)NULL),NULL);
// 				}
// 				else
// 				{
// 					seq = Sequence::create(BezierTo::create(kSparkleFlyTime, config),CCCallFuncND::create(_gemMatrix[i][j], callfuncND_selector(Gem::fourMatchEffect),(void*)NULL),NULL);
// 				}
// 
// 				spr->runAction(seq);
// 			}
// 		}
// 	}
}
void GameLayer::firstTimeInit()
{
	MyPoint pointArray[kMatrixWidth*kMatrixWidth];

	Gem* fruitArray[kMatrixWidth*kMatrixWidth];

	int i,j,max_size=0;

	for(i=0;i<kMatrixWidth*kMatrixWidth;i++)
	{
		pointArray[i].setPosition(0, 0);

		fruitArray[i]=NULL;
	}

	for(j=0;j<kMatrixWidth;j++)
	{
		for(i=0;i<kMatrixWidth;i++)
		{
			if(_gemMatrix[i][j]&&(_gemMatrix[i][j]->getGemKind()==NormalGem||_gemMatrix[i][j]->getGemType()==butterfly||_gemMatrix[i][j]->getGemType()==chicken))
			{
				fruitArray[max_size] = _gemMatrix[i][j];

				pointArray[max_size] = MyPoint(i,j);

				max_size++;
			}
		}
	}

	_mapLayer->findConnection();
	srand ((unsigned)time(nullptr));
	while(!_mapLayer->connectionAreaVector.empty())
	{
		do{
			for(i=max_size;i>0;i--)
			{
				int index = rand()%i;

				MyPoint p = pointArray[index];

				_gemMatrix[p.x][p.y] = fruitArray[i-1];

				pointArray[index].swap(pointArray[i-1]);
			}

			_mapLayer->findPotential();
			
		}while(_mapLayer->potentialAreaVector.empty());

		_mapLayer->findConnection();
	}

	for(i =0;i<max_size;i++)
	{
		FiniteTimeAction* action = GemAction::getInstance().getGemMoveToAction(pointArray[i]);

		fruitArray[i]->runAction(action);
	}
}
//按照优先级，先将动画组织成队列，然后在队列尾部动画上加入回调，最后执行此序列所有动画

void GameLayer::runAllAnimationAndAction(Node* sender, void* param)
{
	AnimationPriority priority = (AnimationPriority)(int)param;
	if(_animationWraperVector.empty())
	{
		afterMatch();
		return;
	}
	if(priority==1)
	{
		_removeGemSwitch = true;
	}
	vector<AnimationWraper> tempVector;

	vector<AnimationWraper>::iterator it;
	//加入队列
	for(it=_animationWraperVector.begin();it!=_animationWraperVector.end();)
	{
		if(it->priority==priority)
		{
			if(it->isSingleAnimation)
			{
				tempVector.push_back(*it);

				_animationWraperVector.erase(it);

				break;
			}
			else
			{
				tempVector.push_back(*it);

				it = _animationWraperVector.erase(it);
			}
		}
		else
		{
			it++;
		}
	}

	if(tempVector.empty())
	{
		this->runAction(CCCallFuncND::create(this,callfuncND_selector(GameLayer::runAllAnimationAndAction),(void*)(priority+1)));
		return;
	}

	//playEffectMusic(NULL, (void*)tempVector[0].animationID);//播放同类声音

	//对同一优先级的动画同时执行，最后一个加入回调
	for(it = tempVector.begin();it!=tempVector.end();it++)
	{
		if(it==tempVector.end()-1)
		{
			if(it->isSingleAnimation)
			{
				runOneKindofAnimation(*it, CCCallFuncND::create(this,callfuncND_selector(GameLayer::runAllAnimationAndAction),(void*)(priority)));
			}
			else
			{
				runOneKindofAnimation(*it, CCCallFuncND::create(this,callfuncND_selector(GameLayer::runAllAnimationAndAction),(void*)(priority+1)));
			}
		}
		else
		{
			runOneKindofAnimation(*it, NULL);
		}
	}
	tempVector.clear();
}
//移动之后，和填空之后，均会执行beforematch函数
void GameLayer::beforeMatch()
{
	_mapLayer->findConnection();


	if(!_mapLayer->connectionAreaVector.empty())
	{
		//标记所有相连的为Eliminate,标记周围的为Changed,标记changed采用累加形式，派生者根据累加次数调整状态
		_mapLayer->markeMatrixChange();
	}
	//处理特殊元素（窝，蛋）及病态元素
	/*_mapLayer->beforeMatchGem();*/

	if(_isMoveDone&&_selectPoint.x!=-1&&_nextPoint.x!=-1)
	{
		_mapLayer->tigerEatAnimal(_selectPoint,_nextPoint);
	}

	//填空之后如果无相连,执行（病态,非病态切换，鸟窝切换
	if(_mapLayer->connectionAreaVector.empty())
	{
		_mapLayer->growOnFertile();
	}
	else
	{
		_continueMatchTimes++;

		playContinueMatchMusic(_continueMatchTimes);

		_mapLayer->beforeMatchGem(&_animationWraperVector, _selectPoint, _nextPoint);
		//清空积累值
		if(_isStartMove&&_selectPoint.x!=-1&&_selectPoint.y!=-1/*&&!_mapInfo->getHeroMode()*/)
		{
			_mapLayer->renewGemCount();
		}
		_mapLayer->matchAllGem(&_animationWraperVector);

		_mapLayer->afterMatch();
	}

	//执行动画
	runAllAnimationAndAction(NULL, (void*)1);
}
void GameLayer::afterMatch()
{
	int i,j;

	_fallGemCount=0;

	_particleNode->removeAllChildrenWithCleanup(true);

	//清理移用于标记的移动点
	_selectPoint.reset();

	_nextPoint.reset();

	//所有被清除的点在_emptyPointVector中,将地图中其所在位置设置为空

	for(i=0;i<_emptyPointVector.size();i++)
	{
		MyPoint p = _emptyPointVector[i];

		_gemMatrix[p.x][p.y]=NULL;
	}
	//对所有非空元素清空动作序列
	for(i=0;i<kMatrixWidth;i++)
	{
		for(j=0;j<kMatrixWidth;j++)
		{
			if(_gemMatrix[i][j])
			{
				_gemMatrix[i][j]->stopHighLight();
				_gemMatrix[i][j]->clearAllAction();
			}
		}
	}
	_mapLayer->dealFallDown(this);
	for(i=0;i<kMatrixWidth;i++)
	{
		for(j=0;j<kMatrixWidth;j++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getActionVectorSize()>0)
			{
				_fallGemCount++;
			}
		}
	}
	//如果此轮没有空出现，则进入等待输入流程
	if(_fallGemCount==0)
	{   //根据联消次数不同，播放不同动画
// 		if(_matchCollectTimes>=kMatch_great_level&&_propkind==PropNone&&!_mapInfo->getHeroMode())
// 		{
// 			CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
// 
// 			CCBReader *ccbReader = new  CCBReader(ccNodeLoaderLibrary);
// 
// 			if(_matchCollectTimes>=kMatch_awesome_level)
// 			{
// 				_matchRepeatReminder = (CCNode*)ccbReader->readNodeGraphFromFile(kMatch_awesome);
// 				GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Match_Excelent);
// 			}
// 			else if(_matchCollectTimes>=kMatch_perfect_level)
// 			{
// 				_matchRepeatReminder = (CCNode*)ccbReader->readNodeGraphFromFile(kMatch_perfect);
// 				GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Match_Perfect);
// 			}
// 			else if(_matchCollectTimes>=kMatch_great_level)
// 			{
// 				_matchRepeatReminder = (CCNode*)ccbReader->readNodeGraphFromFile(kMatch_great);
// 				GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Match_Well);
// 			}
// 
// 			CCBAnimationManager* animationManager = ((CCBAnimationManager*)_matchRepeatReminder->getUserObject());
// 
// 			animationManager->setAnimationCompletedCallback(this, callfunc_selector(GameLayer::matchRepeatCallback));
// 
// 			ccbReader->release();
// 
// 
// 
// 			this->getParent()->addChild(_matchRepeatReminder);
// 
// 			_matchCollectTimes = 0;
// 
// 			return;
// 		}

		_matchCollectTimes = 0;


		if(/*!_mapInfo->getHeroMode()&&_mapInfo->getRestStep()>0*/true)
		{
			_hightLightSwitch = true;

			findPotentialOrRenew();
		}

		if(_propkind!=PropNone)
		{
			propUseOver();
		}

		//已经开始移动，并且没有使用道具
		if(_isStartMove&&_propkind==PropNone&&_matchDownMSGSwitch)
		{
			_matchDownMSGSwitch = false;

			/*CCNotificationCenter::getInstance()->postNotification(kMSG_MatchDown);*/
		}
	}
	else//有空出现，填空，填空完成后执行回调
	{
		_isFallDownEnd = false;

		_isStartMove = true;

		if(_fallGemCount<=0)
		{
			CallFunc* callback = CallFunc::create(this, callfunc_selector(GameLayer::gemFallCallback));
			this->runAction(callback);
		}
		else
		{
			this->fallDownToEnd(NULL, (void*)0);
		}

		_emptyPointVector.clear();
	}

// 	if(!_mapInfo->getHard())     进度条更新 
// 	{
// 		CCNotificationCenter::getInstance()->postNotification(kMSG_UpdateStars);
// 	}
}

void GameLayer::runOneKindofAnimation(AnimationWraper aw,CCCallFuncND* callback)
{
	switch (aw.animationID) {
	case e_aid_grassguy_disappear://草地消失
		{
			aw.node->removeNoCollect(false);
			if(callback)
			{
				this->runAction(Sequence::create(DelayTime::create(kPriorityInterval_GrassguyDissapear),callback,NULL));
			}
		}
		break;
	case e_aid_normal_explode://元素消失（飞走或者移除）
		{
			if(_removeGemSwitch)
			{
				_removeGemSwitch =false;

				this->runAction(CCCallFuncND::create(this, callfuncND_selector(GameLayer::removeGem), (void*)(kMatrixWidth-1)));
			}
		}
		break;
	case e_aid_grassguy_generategrass://生成草地
		{
			Gem::createNodeFromFile(_mapNode,aw.dist1, "rich_soil.ccbi");
			if(callback)
			{
				this->runAction(Sequence::create(DelayTime::create(kGenerateGrassTimeDelayTime),callback,NULL));
			}
		}
		break;
	case e_aid_normal_becomesick:
		{
			aw.node->becomeSick();
			if(callback)
			{
				this->runAction(Sequence::create(DelayTime::create(kPriority5_delaytime),callback,NULL));
			}
		}
		break;
	case e_aid_normal_becomewell:
		{
			aw.node->becomeWell();
			if(callback)
			{
				this->runAction(Sequence::create(DelayTime::create(kPriority5_delaytime),callback,NULL));
			}
		}
		break;
	case e_aid_normal_addscore:
		{
			if(aw.node!=NULL)
			{
				aw.node->addScore(aw.score, true);
				if(aw.priority==e_priority_normal_fertileaddscore)
				{
					/*GemAction::getInstance().playEffectMusic(NULL, (void*)KMusic_Gem_Addscore);*/
				}
				this->runAction(Sequence::create(DelayTime::create(0.1),CallFunc::create(aw.node,callfunc_selector(Gem::bright)),DelayTime::create(0.1),callback,NULL));
			}
		}
		break;
	case e_aid_normal_frozenoff:
		{
			aw.node->frozenOff();
			if(callback)
			{
				this->runAction(Sequence::create(DelayTime::create(kPriority5_delaytime),callback,NULL));
			}
		}
		break;
	case e_aid_normal_restrainoff:
		{
			aw.node->restrainOff();
			if(callback)
			{
				this->runAction(Sequence::create(DelayTime::create(kPriority5_delaytime),callback,NULL));
			}
		}
		break;
	case e_aid_henhouse_threeeggs:
		{
			aw.node->runAnimition(NULL, (void*)kHenhouse_EggAppear);
			if(callback)
			{
				this->runAction(Sequence::create(DelayTime::create(0.1),callback,NULL));
			}
		}
		break;
	case e_aid_henhouse_eggbroken:
		{
			aw.node->runAnimition(NULL, (void*)kHenhouse_EggBroken);
			/*GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Henhouse_State1);*/
			if(callback)
			{
				this->runAction(Sequence::create(DelayTime::create(0.1),callback,NULL));
			}
		}
		break;
	case e_aid_henhouse_birdappear:
		{
			aw.node->runAnimition(NULL, (void*)kHenhouse_ParrotAppear);
			/*GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Henhouse_State2);*/
			if(callback)
			{
				this->runAction(Sequence::create(DelayTime::create(0.1),callback,NULL));
			}
		}
		break;
	case e_aid_henhouse_birdfly:
		{
// 			Henhouse* henhouse = (Henhouse*)aw.node;
// 			GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Henhouse_Birdfly);
// 			henhouse->throwParrot(aw,callback);
			break;
		}
	case e_aid_fish_match:
		{
// 			FishInBubble* fish = (FishInBubble*)aw.node;
// 
// 			fish->changeState(aw.state);
// 			if(callback)
// 			{
// 				this->runAction(CCSequence::create(CCDelayTime::create(kPriority5_delaytime),callback,NULL));
// 			}
		}
		break;
	case e_aid_converge_move:
		{
// 			Gem* fruit = (Gem*)aw.node;
// 			if(fruit->getGemType()==butterfly)
// 			{
// 				((Butterfly*)fruit)->convergeMove(aw.dist1,aw.myDist1,callback);
// 			}
// 			else if(fruit->getGemType()==chicken)
// 			{
// 				((Chicken*)fruit)->convergeMove(aw.dist1,aw.myDist1,callback);
// 			}
		}
		break;
	case e_aid_converge_changestate:
		{
// 			Gem* fruit = (Gem*)aw.node;
// 
// 			if(fruit->getGemType()==butterfly)
// 			{
// 				((Butterfly*)fruit)->changeState(aw.state);
// 			}
// 			else if(fruit->getGemType()==chicken)
// 			{
// 				((Chicken*)fruit)->changeState(aw.state);
// 			}
// 			if(callback)
// 			{
// 				this->runAction(CCSequence::create(CCDelayTime::create(kConvergeChangeTime),callback,NULL));
// 			}
		}
		break;
	case e_aid_tiger_changestate:
		{
// 			Tiger* tiger = (Tiger*)aw.node;
// 
// 			tiger->changeState(aw.state);
// 
// 			if(callback)
// 			{
// 				this->runAction(CCSequence::create(CCDelayTime::create(kPriority5_delaytime),callback,NULL));
// 			}
		}
		break;
	case e_aid_tiger_eatanimal:
		{
// 			Tiger* tiger =(Tiger*)aw.node;
// 
// 			tiger->doEatAimation(aw.fruit1,aw.myDist1,callback);
		}
		break;
	case e_aid_particle_five:
		{
			/*GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Match_Five);*/

			this->fiveMatchEffect(aw.index1,aw.myDist1, callback);
		}
		break;
	case e_aid_particle_four:
		{
			/*GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Match_Cross_Four);*/

			this->fourMatchEffect(aw.index1, callback);
		}
		break;
	case e_aid_particle_cross:
		{
			/*GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Match_Cross_Four);*/
			this->crossMatchEffect(aw.index1, aw.index2, callback);
		}
		break;
	case e_aid_winnermode_starfly:
		{
			NotificationCenter::getInstance()->postNotification(kMSG_UpdateMoves);

// 			CCNode* dist = _mapInfo->getPassLevelCCNode((GemType)0);
// 
// 			CCPoint startPosition = this->convertToNodeSpace(ccp(dist->getPosition().x,dist->getPosition().y+CCDirector::sharedDirector()->getWinSize().height));

			Point startPosition = Vec2::ZERO;

			for(int i=0;i<aw.flagArrayLength;i++)
			{
				int index = aw.flagArray[i];

				MyPoint distPoint(index/kMatrixWidth,index%kMatrixWidth);

				Sprite* sparkle = Sprite::create("fireball.png");

				Point endPosition = _gemMatrix[distPoint.x][distPoint.y]->getPosition();

				GemAction* caller = &(GemAction::getInstance());

				/*CCCallFuncND* playEffectAction = CCCallFuncND::create(caller, callfuncND_selector(GemAction::playEffectMusic), (void*)kMusic_WinnerMode_Hit);*/

				float t1 = startPosition.y-endPosition.y;
				float t2 = startPosition.x-endPosition.x;
				double tan = (atan(t2/t1)*360)/(2*3.14159);
				sparkle->setZOrder(1);
				sparkle->setPosition(startPosition);
				sparkle->setAnchorPoint(Point(0.5,0));
				sparkle->setRotation(tan);
				sparkle->setVisible(false);
				_particleNode->addChild(sparkle);

				Sequence* seq=NULL;

				if(i==aw.flagArrayLength-1)
				{
					seq = Sequence::create(DelayTime::create(i*0.1),CCCallFuncN::create(this, callfuncN_selector(GameLayer::showSparkle)),MoveTo::create(kPriority5_delaytime-0.3, _gemMatrix[distPoint.x][distPoint.y]->getPosition()),/*playEffectAction,*/CallFunc::create(sparkle,callfunc_selector(Sprite::removeFromParent)),CallFunc::create(_gemMatrix[distPoint.x][distPoint.y],callfunc_selector(Gem::winnerModeStart)),callback,NULL);
				}
				else
				{
					seq = Sequence::create(DelayTime::create(i*0.1),CCCallFuncN::create(this, callfuncN_selector(GameLayer::showSparkle)),MoveTo::create(kPriority5_delaytime-0.3, _gemMatrix[distPoint.x][distPoint.y]->getPosition()),/*playEffectAction,*/CallFunc::create(sparkle,callfunc_selector(Sprite::removeFromParent)),CallFunc::create(_gemMatrix[distPoint.x][distPoint.y],callfunc_selector(Gem::winnerModeStart)),NULL);
				}

				sparkle->runAction(seq);
			}
			if(aw.flagArrayLength==0)
			{
				this->runAction(Sequence::create(DelayTime::create(0.5),callback,NULL));
			}
		}
		break;
	case e_aid_winnermode_start:
		{
			for(int i=0;i<aw.flagArrayLength;i++)
			{
				int index = aw.flagArray[i];

				MyPoint distPoint(index/kMatrixWidth,index%kMatrixWidth);

				_gemMatrix[distPoint.x][distPoint.y]->winnerMode();

				Sequence* seq = NULL;

				if(i==aw.flagArrayLength-1)
				{
					seq = Sequence::create(DelayTime::create(i*0.1),CallFunc::create(_gemMatrix[distPoint.x][distPoint.y],callfunc_selector(Gem::winnerMode)),callback,NULL);
				}
				else
				{
					seq = Sequence::create(DelayTime::create(i*0.1),CallFunc::create(_gemMatrix[distPoint.x][distPoint.y],callfunc_selector(Gem::winnerMode)),NULL);
				}

				this->runAction(seq);
			}

			if(aw.flagArrayLength==0)
			{
				this->runAction(Sequence::create(DelayTime::create(1),callback,NULL));
			}
		}
		break;
	case e_aid_beforematch:

		_animationWraperVector.clear();

		this->runAction(CallFunc::create(this, callfunc_selector(GameLayer::beforeMatch)));

		break;
	default:
		{
			if(callback)
			{
				this->runAction(Sequence::create(DelayTime::create(0.1),callback,NULL));
			}
		}
		break;
	}
}

void GameLayer::gemFallNotification(Ref *obj)
{
	gemFallCallback();
}

void GameLayer::gemFallCallback()
{
	if(_isFallDownEnd) return;

	_fallGemCount--;

	if(_fallGemCount<=0)
	{
		_isFallDownEnd = true;

		this->runAction(CallFunc::create(this, callfunc_selector(GameLayer::beforeMatch)));
	}
}

Point GameLayer::calculateStartPoint(float edgeWidth,Size& winSize,float scaleFactor)
{
	int leftIndex=kMatrixWidth,rightIndex=0,upIndex=0,downIndex=kMatrixWidth;

	for(int i=0;i<kMatrixWidth;i++)
	{
		for(int j=0;j<kMatrixWidth;j++)
		{
			if(_mapMatrix[i][j]!=NoneInEdge&&_mapMatrix[i][j]!=NoneInMiddle)
			{
				if(i<leftIndex)
				{
					leftIndex = i;
				}
				if(i>rightIndex)
				{
					rightIndex = i;
				}
				if(j>upIndex)
				{
					upIndex = j;
				}
				if(j<downIndex)
				{
					downIndex = j;
				}
			}
		}
	}
	int startX = kMatrixWidth-rightIndex-leftIndex-1;
	int startY = kMatrixWidth-upIndex-downIndex-1;

	return Point(edgeWidth+startX*0.5*scaleFactor*kElementSize,(winSize.height-(winSize.width-edgeWidth*2))/2+startY*0.5*scaleFactor*kElementSize);
}

void GameLayer::addShieldLayer()
{
	SwallowLayer *noTouchLayer = SwallowLayer::create();

	this->addChild(noTouchLayer,-129,9999);
}

void GameLayer::removeShieldLayer()
{
	this->removeChildByTag(9999);
}

void GameLayer::shining()
{
	initExplodeFlyTime();//初始化收集元素飞行时间

	Vector<FiniteTimeAction*> actionArray;

	int i,j,k;

	for(k=1;k<=kMatrixWidth;k++)
	{
		for(i=0;i<k;i++)
		{
			if(_gemMatrix[i][i+kMatrixWidth-k])
			{
				CallFunc* callAction1 = CallFunc::create(_gemMatrix[i][i+kMatrixWidth-k], callfunc_selector(Gem::bright));
				actionArray.pushBack(callAction1);
			}
		}

		actionArray.pushBack(DelayTime::create(0.02));
	}

	for(k=kMatrixWidth-1;k>=1;k--)
	{
		for(i=kMatrixWidth-k;i<kMatrixWidth;i++)
		{
			if(_gemMatrix[i][i-kMatrixWidth+k])
			{
				CallFunc* callAction2 = CallFunc::create(_gemMatrix[i][i-kMatrixWidth+k], callfunc_selector(Gem::bright));

				actionArray.pushBack(callAction2);
			}
		}
		actionArray.pushBack(DelayTime::create(0.02));
	}

	actionArray.pushBack(DelayTime::create(0.3));

	for(i=0;i<kMatrixWidth;i++)
	{
		for(j=0;j<kMatrixWidth;j++)
		{
			if(_gemMatrix[i][j])
			{
				CallFunc* callAction3 = CallFunc::create(_gemMatrix[i][j], callfunc_selector(Gem::bright));
				actionArray.pushBack(callAction3);
			}
		}
	}

	actionArray.pushBack(DelayTime::create(3*kBrightScaleTime));

	CallFunc* callback = NULL;

	if(/*_mapInfo->getHard()*/false)
	{
		callback = CallFunc::create(this, callfunc_selector(GameLayer::bossAppear));

		actionArray.pushBack(callback);
	}
	else
	{
		callback = CallFunc::create(this, callfunc_selector(GameLayer::beforeMatch));

		actionArray.pushBack(callback);
	}

	actionArray.pushBack(CallFunc::create(this, callfunc_selector(GameLayer::addClassLayer)));

	this->runAction(Sequence::create(actionArray));
}

void GameLayer::winnerMode(Ref *obj)
{
	if(_winnerModeStarsNode==NULL)
	{
		_winnerModeStarsNode = Node::create();

		this->addChild(_winnerModeStarsNode,kParticleZOrder);

		winnerModeShiningInBackgroud(NULL, (void*)0);
	}

	this->setTouchDisable();

	if(!_isWinnerModeStart)
	{
		for(int i=0;i<kMatrixWidth;i++)
		{
			for(int j=0;j<kMatrixWidth;j++)
			{
				if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getSick())
				{
					_gemMatrix[i][j]->setSick(false);
				}
			}
		}
		_isWinnerModeStart = true;
	}

	//生成winnermode动画
	//1,根据当前的moves值随机找到不大于6个的位置，生成飞过去动画,更新moves
	//2,地图中被选中的元素执行横排/竖排消除,生成动画,调用这些元素的explode函数
	//3,执行runallanimation.

	int flagArray[kMatrixWidth*kMatrixWidth]={0};
	int tempArray[kMatrixWidth*kMatrixWidth]={0};

	int i,j,max=0,index,k,l;

	for(i=0;i<kMatrixWidth;i++)
	{
		for(j=0;j<kMatrixWidth;j++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getState()!=-1&&_gemMatrix[i][j]->canMove()&&_gemMatrix[i][j]->getGemKind()==NormalGem)
			{
				flagArray[i*kMatrixWidth+j]=1;
			}
		}
	}
	//取出所有可以替换的位置
	for(i=0;i<kMatrixWidth*kMatrixWidth;i++)
	{
		if(flagArray[i]==1)
		{
			tempArray[max++]=i;
		}
	}

	int moves = 6;
// 	int moves = _mapInfo->getRestStep()>6?6:_mapInfo->getRestStep();
// 
	moves = moves>max?max:moves;
// 
// 	_mapInfo->setRestStep(_mapInfo->getRestStep()-moves);

	Gem::getUniqueNumber(-1);
	//所有位置生成动画
	AnimationWraper aw(NULL,e_aid_winnermode_starfly,e_priority_winnermode_starfly);

	if(max<=0)
	{
		aw.flagArrayLength = 0;
	}
	else
	{
		aw.flagArrayLength = moves;
	}

	for(i=0;i<moves&&max>0;i++)
	{
		index = Gem::getUniqueNumber(max--);

		aw.flagArray[i]=tempArray[index];

		MyPoint p(tempArray[index]/kMatrixWidth,tempArray[index]%kMatrixWidth);

		if(p.x%2==0)
		{
			for(k=p.x,l=0;l<kMatrixWidth;l++)
			{
				if(_gemMatrix[k][l]&&_gemMatrix[k][l]->getGemKind()==NormalGem)
				{
					MyPoint myPoint(k,l);

					_gemMatrix[k][l]->explode(myPoint,myPoint, 0, 0 );
				}
			}
		}
		else
		{
			for(k=0,l=p.y;k<kMatrixWidth;k++)
			{
				if(_gemMatrix[k][l]&&_gemMatrix[k][l]->getGemKind()==NormalGem)
				{
					MyPoint myPoint(k,l);

					_gemMatrix[k][l]->explode(myPoint,myPoint, 0, 0 );
				}
			}
		}
	}


	_animationWraperVector.push_back(aw);

	aw.priority = e_priority_winnermode_start;

	aw.animationID = e_aid_winnermode_start;

	_animationWraperVector.push_back(aw);

	AnimationWraper aw2(NULL, e_aid_normal_explode, e_priority_normal_explode);

	_animationWraperVector.push_back(aw2);

	_matchDownMSGSwitch = true;

	runAllAnimationAndAction(NULL, (void*)1);
}

void GameLayer::propUseStart(Ref* object)
{
	_matchDownMSGSwitch = true;

	int t = (int)object;

	_propkind = (PropKind)t;

	stopRemindAndIdle();//停止提示和空闲动作

	if(_propkind==PropAddOne||_propkind==PropCureSick)
	{
		MyPoint p(0,0);

		propUse(p);
	}
	else
	{
		if(_propStarstNode!=NULL)
		{
			_propStarstNode->setVisible(true);
			return;
		}
		else
		{
			_propStarstNode = Node::create();

			this->addChild(_propStarstNode,kParticleZOrder);
		}

		propShiningInBackgroud(NULL,(void*)0);
	}

}

bool GameLayer::propUse(MyPoint &point)
{
	switch (_propkind) {
	case PropSingle:
		{
			if(_gemMatrix[point.x][point.y])
			{
				if((_gemMatrix[point.x][point.y]->getGemType()==butterfly||
					_gemMatrix[point.x][point.y]->getGemType()==chicken)&&_gemMatrix[point.x][point.y]->getState()==0)
				{
					return false;
				}

				/*GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Prop_Single);*/

				if(_gemMatrix[point.x][point.y]->getGemKind()==NormalGem||_gemMatrix[point.x][point.y]->getGemType()==butterfly||
					_gemMatrix[point.x][point.y]->getGemType()==chicken)
				{
					_gemMatrix[point.x][point.y]->explode(point, point, 0, 0);

					_gemMatrix[point.x][point.y]->propSingle();
				}
				else if(_gemMatrix[point.x][point.y]->getGemType()==henhouse)
				{
					_gemMatrix[point.x][point.y]->addChange();

					_gemMatrix[point.x][point.y]->beforeMatch(point.x,point.y);

					_gemMatrix[point.x][point.y]->propSingle();
				}else if(_gemMatrix[point.x][point.y]->getGemType()==fishinbubble)
				{
					_gemMatrix[point.x][point.y]->affected(1);

					_gemMatrix[point.x][point.y]->propSingle();
				}

				if(_gemMatrix[point.x][point.y]->getState()!=-1)
				{
					_gemMatrix[point.x][point.y]->afterMatch(point.x, point.y);
				}

				AnimationWraper aw(NULL, e_aid_normal_explode, e_priority_normal_explode);

				_animationWraperVector.push_back(aw);

				AnimationWraper aw1(NULL,e_aid_beforematch,e_priority_beforematch);

				_animationWraperVector.push_back(aw1);

				this->runAction(Sequence::create(DelayTime::create(1),CCCallFuncND::create(this, callfuncND_selector(GameLayer::runAllAnimationAndAction),(void*)1),NULL));
			}
			else
			{
				return false;
			}
		}
		break;
	case PropOneKind:
		{
			if(_gemMatrix[point.x][point.y]&&_gemMatrix[point.x][point.y]->getGemKind()==NormalGem)
			{
				/*GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Prop_OneKind);*/

				int i,j,max=0,maxTime=0;

				Sprite* spriteArray[kMatrixWidth*kMatrixWidth]={0};

				MyPoint distArray[kMatrixWidth*kMatrixWidth];

				int flagMatrix[kMatrixWidth][kMatrixWidth] ={0};

				GemType type = _gemMatrix[point.x][point.y]->getGemType();

				for(i=0;i<kMatrixWidth;i++)
				{
					for (j=0;j<kMatrixWidth; j++)
					{
						if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemType()==type&&_gemMatrix[i][j]->canMove())
						{
							flagMatrix[i][j]=1;
						}
					}
				}

				for(i=0;i<kMatrixWidth;i++)
				{
					for (j=0; j<kMatrixWidth; j++)
					{
						if(flagMatrix[i][j]==1)
						{
							//加入流星,设置旋转
							Point starBeginPoint = Point(kStartX+(i*4)*kElementSize,kStartY+(2*kMatrixWidth)*kElementSize);

							Sprite* spr = Sprite::createWithSpriteFrameName("skillmeteor.png");
							spr->setPosition(starBeginPoint);
							_particleNode->addChild(spr, 10);
							spriteArray[max] = spr;
							distArray[max++].setPosition(i, j);

						}
					}
				}
				for (i=0; i<max; i++)
				{
					Sequence* seq = NULL;

					Gem* distGem =_gemMatrix[distArray[i].x][distArray[i].y];

					Point startPosition = spriteArray[i]->getPosition();

					Point endPosition = distGem->getPosition();

					float time = ccpDistance(startPosition, endPosition)/KCollectMoveSpeed;

					if(maxTime<time)
					{
						maxTime = time;
					}

					float t1 = startPosition.y-endPosition.y;
					float t2 = startPosition.x-endPosition.x;
					double tan = (atan(t2/t1)*360)/(2*3.14159);

					spriteArray[i]->setPosition(startPosition);
					spriteArray[i]->setAnchorPoint(Point(0.5,0));
					spriteArray[i]->setRotation(tan);

					GemAction* caller = &(GemAction::getInstance());

					seq = Sequence::create(MoveTo::create(time,endPosition),/*CCCallFuncND::create(caller,callfuncND_selector(GemAction::playEffectMusic),(void*)kMusic_WinnerMode_Hit),*/CCCallFuncND::create(distGem, callfuncND_selector(Gem::fiveMatchEffect), (void*)NULL),NULL);

					spriteArray[i]->runAction(seq);

					distGem->explode(distArray[i], distArray[i], 0, 0);
				}
				AnimationWraper aw(NULL, e_aid_normal_explode, e_priority_normal_explode);
				_animationWraperVector.push_back(aw);

				this->runAction(Sequence::create(DelayTime::create(maxTime+1),CCCallFuncND::create(this, callfuncND_selector(GameLayer::runAllAnimationAndAction), (void*)1),NULL));

			}
			else
			{
				return false;
			}
		}
		break;
	case PropRow:
		{
			if(_gemMatrix[point.x][point.y])
			{
				/*GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Prop_Row);*/

				_propCar = Node::create();

// 				CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
// 
// 				CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
// 
// 				CCNode* node = ccbReader->readNodeGraphFromFile("car.ccbi");
// 
// 				_propCar->addChild(node);

				_propCar->setPosition(Point(kStartX,kStartY+(point.y+0.5)*kElementSize));

/*				ccbReader->release();*/

				_particleNode->addChild(_propCar);

				_propCar->setZOrder(1);

				schedule(schedule_selector(GameLayer::propRemoveRow));

				Sequence* seq = Sequence::create(MoveTo::create(1.5, Point(kStartX+(kMatrixWidth+4)*kElementSize,kStartY+(point.y+0.5)*kElementSize)),CCCallFuncND::create(this, callfuncND_selector(GameLayer::runAllAnimationAndAction), (void*)1),CallFunc::create(this,callfunc_selector(GameLayer::propUnschedule)),NULL);

				_propCar->runAction(seq);

			}
			else
			{
				return false;
			}
		}
		break;
	case PropAddOne:
		{
			/*GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Prop_AddOne);*/

			for(int i=0;i<kMatrixWidth;i++)
			{
				for(int j=0;j<kMatrixWidth;j++)
				{
					if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem)
					{
						_gemMatrix[i][j]->affected(1);
					}
				}
			}
			runAllAnimationAndAction(NULL, (void*)1);
		}
		break;
	case PropCureSick:
		{
			bool isUsed = false;

			for(int i=0;i<kMatrixWidth;i++)
			{
				for(int j=0;j<kMatrixWidth;j++)
				{
					if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getSick())
					{
						_gemMatrix[i][j]->setSick(false);

						_gemMatrix[i][j]->beforeMatch(i , j);

						isUsed = true;
					}
				}
			}

			runAllAnimationAndAction(NULL, (void*)1);

			if(isUsed)
			{
				/*GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Prop_CureSick);*/
			}
			else
			{
				NotificationCenter::getInstance()->postNotification(kMSG_PropUseNoCandidate);
			}
		}
		break;
	default:
		break;
	}

	return true;

}
void GameLayer::propRemoveRow(float dt )
{
	Point point = _propCar->getPosition();

	MyPoint p((point.x-kStartX)/kElementSize,(point.y-kStartY)/kElementSize);

	if(p.x>=0&&p.x<kMatrixWidth&&p.y>=0&&p.y<kMatrixWidth&&_gemMatrix[p.x][p.y]&&_gemMatrix[p.x][p.y]->getGemKind()==NormalGem&&_gemMatrix[p.x][p.y]->canMove())
	{
		Point dist = _explodeDistArray[_gemMatrix[p.x][p.y]->getGemType()];

		if(!dist.equals(Point(-1,-1))&&!_gemMatrix[p.x][p.y]->getSick())
		{
			_gemMatrix[p.x][p.y]->removeCollect(dist,getExplodeFlyTime(p),false);
		}
		else
		{
			_gemMatrix[p.x][p.y]->removeNoCollect(false);
		}

		_gemMatrix[p.x][p.y]=NULL;

		_emptyPointVector.push_back(p);
	}
}
void GameLayer::propUnschedule()
{
	unschedule(schedule_selector(GameLayer::propRemoveRow));
}
void GameLayer::propUseCancel()
{
	_propkind = PropNone;

	if(_propStarstNode)
	{
		_propStarstNode->setVisible(false);
	}
}

void GameLayer::propUseOver()
{
	_propkind = PropNone;

	if(_propStarstNode)
	{
		_propStarstNode->setVisible(false);
	}

	NotificationCenter::getInstance()->postNotification(kMSG_PropUseOver);
}

void GameLayer::propShiningInBackgroud(Node* sender,void* data)
{

	int flag = (int)data;

	int flag1 = flag/10;
	int flag2 = flag%10;

	int i,j;

	Point point;

	int flagMatrix[kMatrixWidth+2][kMatrixWidth+2]={0};

	for(i=0;i<kMatrixWidth;i++)
	{
		for(j=0;j<kMatrixWidth;j++)
		{
			if(0==_mapMatrix[i][j]||-3==_mapMatrix[i][j])
			{
				flagMatrix[i+1][j+1]=0;
			}
			else
			{
				flagMatrix[i+1][j+1]=1;
			}
		}
	}

	for(int i=0;i<kMatrixWidth+2;i++)
	{
		for(int j=0;j<kMatrixWidth+2;j++)
		{
			int index = i*kMatrixWidth+j;

			if(flagMatrix[i][j]==0&&index%3==flag1)
			{
				//左侧不为空
				if(i-1>=0&&flagMatrix[i-1][j])
				{
					if(flag2==0)
					{
						point = Point(kStartX+(i-0.75)*kElementSize,kStartY+(j-0.25)*kElementSize);
					}
					else
					{
						point = Point(kStartX+(i-0.75)*kElementSize,kStartY+(j-0.75)*kElementSize);
					}


// 					CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
// 
// 					CCBReader *ccbReader = new  CCBReader(ccNodeLoaderLibrary);
// 
// 					CCNode * node = (CCNode*)ccbReader->readNodeGraphFromFile("border.ccbi");
// 
// 					ccbReader->release();
// 
// 					node->setPosition(point);
// 
// 					_propStarstNode->addChild(node);
				}
				//下不为空
				if(j-1>=0&&flagMatrix[i][j-1])
				{
					if(flag2==0)
					{
						point = Point(kStartX+(i-0.75)*kElementSize,kStartY+(j-0.75)*kElementSize);
					}
					else
					{
						point = Point(kStartX+(i-0.25)*kElementSize,kStartY+(j-0.75)*kElementSize);
					}


// 					CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
// 
// 					CCBReader *ccbReader = new  CCBReader(ccNodeLoaderLibrary);
// 
// 					CCNode * node = (CCNode*)ccbReader->readNodeGraphFromFile("border.ccbi");
// 
// 					ccbReader->release();
// 
// 					node->setPosition(point);
// 
// 					_propStarstNode->addChild(node);
				}
				//右侧不为空
				if(i+1<kMatrixWidth+2&&flagMatrix[i+1][j])
				{
					if(flag2==0)
					{
						point = Point(kStartX+(i-0.25)*kElementSize,kStartY+(j-0.25)*kElementSize);
					}
					else
					{
						point = Point(kStartX+(i-0.25)*kElementSize,kStartY+(j-0.75)*kElementSize);
					}


// 					CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
// 
// 					CCBReader *ccbReader = new  CCBReader(ccNodeLoaderLibrary);
// 
// 					CCNode * node = (CCNode*)ccbReader->readNodeGraphFromFile("border.ccbi");
// 
// 					ccbReader->release();
// 
// 					node->setPosition(point);
// 
// 					_propStarstNode->addChild(node);
				}
				//上不为空
				if(j+1<kMatrixWidth+2&&flagMatrix[i][j+1])
				{
					if(flag2==0)
					{
						point = Point(kStartX+(i-0.75)*kElementSize,kStartY+(j-0.25)*kElementSize);
					}
					else
					{
						point = Point(kStartX+(i-0.25)*kElementSize,kStartY+(j-0.25)*kElementSize);
					}


// 					CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
// 
// 					CCBReader *ccbReader = new  CCBReader(ccNodeLoaderLibrary);
// 
// 					CCNode * node = (CCNode*)ccbReader->readNodeGraphFromFile("border.ccbi");
// 
// 					ccbReader->release();
// 
// 					node->setPosition(point);
// 
// 					_propStarstNode->addChild(node);
				}
			}
		}
	}
	if(flag1==0&&flag2==0)
	{
		this->runAction(Sequence::create(DelayTime::create(0.2),CCCallFuncND::create(this, callfuncND_selector(GameLayer::propShiningInBackgroud),(void*)1),NULL));
	}
	else if(flag1==0&&flag2==1)
	{
		this->runAction(Sequence::create(DelayTime::create(0.2),CCCallFuncND::create(this, callfuncND_selector(GameLayer::propShiningInBackgroud),(void*)10),NULL));
	}
	else if(flag1==1&&flag2==0)
	{
		this->runAction(Sequence::create(DelayTime::create(0.2),CCCallFuncND::create(this, callfuncND_selector(GameLayer::propShiningInBackgroud),(void*)11),NULL));
	}
	else if(flag1==1&&flag2==1)
	{
		this->runAction(Sequence::create(DelayTime::create(0.2),CCCallFuncND::create(this, callfuncND_selector(GameLayer::propShiningInBackgroud),(void*)20),NULL));
	}
	else if(flag1==2&&flag2==0)
	{
		this->runAction(Sequence::create(DelayTime::create(0.2),CCCallFuncND::create(this, callfuncND_selector(GameLayer::propShiningInBackgroud),(void*)21),NULL));
	}
}

void GameLayer::winnerModeShiningInBackgroud(cocos2d::Node *sender, void *data)
{

	int flag = (int)data;

	int flag1 = flag/10;
	int flag2 = flag%10;

	int i,j;

	Point point;

	int flagMatrix[kMatrixWidth+2][kMatrixWidth+2]={0};

	for(i=0;i<kMatrixWidth;i++)
	{
		for(j=0;j<kMatrixWidth;j++)
		{
			if(0==_mapMatrix[i][j]||-3==_mapMatrix[i][j])
			{
				flagMatrix[i+1][j+1]=0;
			}
			else
			{
				flagMatrix[i+1][j+1]=1;
			}
		}
	}

	for(int i=0;i<kMatrixWidth+2;i++)
	{
		for(int j=0;j<kMatrixWidth+2;j++)
		{
			int index = i*kMatrixWidth+j;

			if(flagMatrix[i][j]==0&&index%3==flag1)
			{
				//左侧不为空
				if(i-1>=0&&flagMatrix[i-1][j])
				{
					if(flag2==0)
					{
						point = Point(kStartX+(i-0.75)*kElementSize,kStartY+(j-0.25)*kElementSize);
					}
					else
					{
						point = Point(kStartX+(i-0.75)*kElementSize,kStartY+(j-0.75)*kElementSize);
					}


// 					CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
// 
// 					CCBReader *ccbReader = new  CCBReader(ccNodeLoaderLibrary);
// 
// 					CCNode * node = (CCNode*)ccbReader->readNodeGraphFromFile("border2.ccbi");
// 
// 					ccbReader->release();
// 
// 					node->setPosition(point);
// 
// 					_winnerModeStarsNode->addChild(node);
				}
				//下不为空
				if(j-1>=0&&flagMatrix[i][j-1])
				{
					if(flag2==0)
					{
						point = Point(kStartX+(i-0.75)*kElementSize,kStartY+(j-0.75)*kElementSize);
					}
					else
					{
						point = Point(kStartX+(i-0.25)*kElementSize,kStartY+(j-0.75)*kElementSize);
					}


// 					CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
// 
// 					CCBReader *ccbReader = new  CCBReader(ccNodeLoaderLibrary);
// 
// 					CCNode * node = (CCNode*)ccbReader->readNodeGraphFromFile("border2.ccbi");
// 
// 					ccbReader->release();
// 
// 					node->setPosition(point);
// 
// 					_winnerModeStarsNode->addChild(node);
				}
				//右侧不为空
				if(i+1<kMatrixWidth+2&&flagMatrix[i+1][j])
				{
					if(flag2==0)
					{
						point = Point(kStartX+(i-0.25)*kElementSize,kStartY+(j-0.25)*kElementSize);
					}
					else
					{
						point = Point(kStartX+(i-0.25)*kElementSize,kStartY+(j-0.75)*kElementSize);
					}


// 					CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
// 
// 					CCBReader *ccbReader = new  CCBReader(ccNodeLoaderLibrary);
// 
// 					CCNode * node = (CCNode*)ccbReader->readNodeGraphFromFile("border2.ccbi");
// 
// 					ccbReader->release();
// 
// 					node->setPosition(point);
// 
// 					_winnerModeStarsNode->addChild(node);
				}
				//上不为空
				if(j+1<kMatrixWidth+2&&flagMatrix[i][j+1])
				{
					if(flag2==0)
					{
						point = Point(kStartX+(i-0.75)*kElementSize,kStartY+(j-0.25)*kElementSize);
					}
					else
					{
						point = Point(kStartX+(i-0.25)*kElementSize,kStartY+(j-0.25)*kElementSize);
					}


// 					CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
// 
// 					CCBReader *ccbReader = new  CCBReader(ccNodeLoaderLibrary);
// 
// 					CCNode * node = (CCNode*)ccbReader->readNodeGraphFromFile("border2.ccbi");
// 
// 					ccbReader->release();
// 
// 					node->setPosition(point);
// 
// 					_winnerModeStarsNode->addChild(node);
				}
			}
		}
	}
	if(flag1==0&&flag2==0)
	{
		this->runAction(Sequence::create(DelayTime::create(0.2),CCCallFuncND::create(this, callfuncND_selector(GameLayer::winnerModeShiningInBackgroud),(void*)1),NULL));
	}
	else if(flag1==0&&flag2==1)
	{
		this->runAction(Sequence::create(DelayTime::create(0.2),CCCallFuncND::create(this, callfuncND_selector(GameLayer::winnerModeShiningInBackgroud),(void*)10),NULL));
	}
	else if(flag1==1&&flag2==0)
	{
		this->runAction(Sequence::create(DelayTime::create(0.2),CCCallFuncND::create(this, callfuncND_selector(GameLayer::winnerModeShiningInBackgroud),(void*)11),NULL));
	}
	else if(flag1==1&&flag2==1)
	{
		this->runAction(Sequence::create(DelayTime::create(0.2),CCCallFuncND::create(this, callfuncND_selector(GameLayer::winnerModeShiningInBackgroud),(void*)20),NULL));
	}
	else if(flag1==2&&flag2==0)
	{
		this->runAction(Sequence::create(DelayTime::create(0.2),CCCallFuncND::create(this, callfuncND_selector(GameLayer::winnerModeShiningInBackgroud),(void*)21),NULL));
	}
}

void GameLayer::outStage(Ref* object)
{
	this->setTouchDisable();

	int flag = (int)object;

	GemAction* caller = &(GemAction::getInstance());

	Point startPosition = this->getPosition();

	Point endPosition =Point(Director::getInstance()->getWinSize().width*kGameLayerStartMultiple,startPosition.y);

	Sequence* seq = Sequence::create(CCEaseBackIn::create(MoveTo::create(kGameLayerFlyTime,endPosition)),CallFunc::create(this,callfunc_selector(GameLayer::sendOutStageMessage)),/*CallFunc::create(caller,callfunc_selector(GemAction::stopBackgroudMusic)),*/CallFunc::create(this,callfunc_selector(GameLayer::removeGameLayer)),NULL);

	_particleNode->removeAllChildrenWithCleanup(true);
	if(_propStarstNode)
	{
		_propStarstNode->removeAllChildrenWithCleanup(true);
	}
	if(_winnerModeStarsNode)
	{
		_winnerModeStarsNode->removeAllChildrenWithCleanup(true);
	}

	if(flag==0)
	{
		this->runAction(seq);

		return;
	}
	else
	{
		Vector<FiniteTimeAction*> array ;

		int i,j;

		for(i=0;i<kMatrixWidth;i++)
		{
			for(j=0;j<kMatrixWidth;j++)
			{
				if(_gemMatrix[i][j])
				{
					CallFunc* action = CallFunc::create(_gemMatrix[i][j], callfunc_selector(Gem::highLight));
					array.pushBack(action);
				}
			}
			array.pushBack(DelayTime::create(0.05));
		}
		array.pushBack(DelayTime::create(1));

		array.pushBack(seq);

		this->runAction(Sequence::create(array));
	}
}

void GameLayer::sendOutStageMessage()
{
	NotificationCenter::getInstance()->postNotification(kMSG_GameLayer_AfterOutStage);
}
void GameLayer::matchRepeatCallback()
{
	runAllAnimationAndAction(NULL, (void*)1);

	_matchRepeatReminder->removeFromParentAndCleanup(true);
}

void GameLayer::playContinueMatchMusic(int matchTimes)
{
// 	switch (matchTimes) {
// 	case 2:
// 		GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Match_Continue1);
// 		break;
// 	case 3:
// 		GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Match_Continue2);
// 		break;
// 	case 4:
// 		GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Match_Continue3);
// 		break;
// 	case 5:
// 		GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Match_Continue4);
// 		break;
// 	case 6:
// 		GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Match_Continue5);
// 		break;
// 	case 7:
// 		GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Match_Continue6);
// 		break;
// 	case 8:
// 		GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Match_Continue7);
// 		break;
// 
// 	}
}

void GameLayer::initExplodeFlyTime()
{
// 	std::map<GemType,int>::iterator it;
// 
// 	float sum=0,count=1;
// 
// 	for(int i=0;i<kMatrixWidth;i++)
// 	{
// 		for(int j=0;j<kMatrixWidth;j++)
// 		{
// 			CCPoint startPoint = ccp(kStartX+(i+0.5)*kElementSize,kStartY+(j+0.5)*kElementSize);
// 
// 			for(it=_mapInfo->getPassLevelMap().begin();it!=_mapInfo->getPassLevelMap().end();it++)
// 			{
// 				CCNode* dist = _mapInfo->getPassLevelCCNode(it->first);
// 
// 				CCPoint tempPoint = ccp(dist->getPosition().x,dist->getPosition().y+CCDirector::sharedDirector()->getWinSize().height);
// 
// 				CCPoint endPoint = this->convertToNodeSpace(tempPoint);
// 
// 				sum+=ccpDistance(startPoint, endPoint);
// 
// 				count++;
// 			}
// 
// 			float time = (sum/count)/KCollectMoveSpeed;
// 
// 			_explodeFlyTime[i][j]=time;
// 		}
// 	}
// 
// 	for(int i=0;i<maxfruit;i++)
// 	{
// 		CCNode* node = _mapInfo->getPassLevelCCNode((GemType)i);
// 
// 		if(node)
// 		{
// 			CCPoint tempPoint = ccp(node->getPosition().x,node->getPosition().y+CCDirector::sharedDirector()->getWinSize().height);
// 
// 			_explodeDistArray[i]= this->convertToNodeSpace(tempPoint);
// 		}
// 		else
// 		{
// 			_explodeDistArray[i].setPoint(-1, -1);
// 		}
// 	}
}
float GameLayer::getExplodeFlyTime(MyPoint &point)
{
	float time;

	if(point.x>=0&&point.y>=0&&point.x<kMatrixWidth&&point.y<kMatrixWidth)
	{
		time = _explodeFlyTime[point.x][point.y];
	}
	else
	{
		time = _explodeFlyTime[0][0];
	}

	return time;
}
void GameLayer::removeGem(Node *sender, void *data)
{
	int t = (int)data;

	int i = t/kMatrixWidth,j=t%kMatrixWidth;

	Action* seq=NULL;

	if(j<0)
	{
		Sequence* seq = Sequence::create(DelayTime::create(0.02),CCCallFuncND::create(this,callfuncND_selector(GameLayer::runAllAnimationAndAction),(void*)(e_priority_normal_explode+1)),NULL);

		this->runAction(seq);

		return;
	}
	int next=0;
	float time = 0.02;

	if(i==kMatrixWidth-1&&j>=0)
	{
		next = j-1;
	}
	else if(i<kMatrixWidth)
	{
		next = (i+1)*kMatrixWidth+j;
	}
	if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getState()<0)
	{
		/*CCPoint dist = _explodeDistArray[_gemMatrix[i][j]->getGemType()];*/

		MyPoint p(i,j);

// 		if(!dist.equals(CCPoint(-1,-1))&&!_gemMatrix[i][j]->getSick())
// 		{
// 			if(_gemMatrix[i][j]->getScoreSum()>0)
// 			{
// 				char str[5]={0};
// 
// 				std::sprintf(str, "*%d",_gemMatrix[i][j]->getScoreSum());
// 
// // 				CCLabelBMFont* label = CCLabelBMFont::create(str,"item.fnt");
// // 
// // 				label->setPosition(_gemMatrix[i][j]->getPosition());
// // 
// // 				_particleNode->addChild(label);
// // 
// // 				label->runAction(CCScaleTo::create(kExplodeSumShowTime, 3));
// 			}
// 			_matchCollectTimes+=_gemMatrix[i][j]->getScore();
// 
// 			_gemMatrix[i][j]->removeCollect(dist, getExplodeFlyTime(p),_gemMatrix[i][j]->getFlag()==1);
// 		}
// 		else
// 		{
			_gemMatrix[i][j]->removeNoCollect(_gemMatrix[i][j]->getFlag()==1);
/*		}*/

		_emptyPointVector.push_back(p);

		seq = Sequence::create(DelayTime::create(time),CCCallFuncND::create(this, callfuncND_selector(GameLayer::removeGem), (void*)next),NULL);
	}
	else
	{
		seq = CCCallFuncND::create(this, callfuncND_selector(GameLayer::removeGem), (void*)next);
	}

	this->runAction(seq);
}

void GameLayer::addClassLayer()
{
// 	if(_mapInfo->getClassKind()!=ClassNone)
// 	{
// 		_classGuidLayer = ClassGuideLayer::create();
// 
// 		_classGuidLayer->initClassGuideLayer(_startPoint, _scaleFactor);
// 
// 		this->getParent()->addChild(_classGuidLayer,10);
// 	}

}
void GameLayer::classGuidMove(cocos2d::Ref *obj)
{
	int d = (int)obj;

	Direction direction = (Direction)d;

// 	_selectPoint = _mapInfo->getClassFirstPoint();
// 
// 	_nextPoint = _mapInfo->getClassSecondPoint();
// 
// 	_moveDirection = direction;
// 
// 	setTouchDisable();
// 
// 	changeGem(direction);
// 
// 	int level = DDMenuDataCenter::SharedMenuData()->getCurrSelLevelIdx();
// 
// 	_mapInfo->mapInfoClassAlert(level, ClassNone);
}
void GameLayer::removeGameLayer()
{
	this->removeFromParentAndCleanup(true);
}

void GameLayer::showSparkle(cocos2d::Node *sender)
{
	if(sender)
	{
		sender->setVisible(true);
	}
}
void GameLayer::fallDownToEnd(cocos2d::Node *sender,void *data)
{
	int j=(int)data;
	for(int i=0;i<kMatrixWidth;i++)
	{
		if(_gemMatrix[i][j])
		{
			_gemMatrix[i][j]->fallDownToEnd();
		}
	}
	if(j+1<kMatrixWidth)
	{
		this->runAction(Sequence::create(DelayTime::create(0.015),CCCallFuncND::create(this, callfuncND_selector(GameLayer::fallDownToEnd), (void*)(j+1)),NULL));
	}
	else
	{
		_mapLayer->clearCounter();
	}
}

void GameLayer::bossAppear()
{
// 	CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
// 
// 	CCBReader *ccbReader = new  CCBReader(ccNodeLoaderLibrary);
// 
// 	_bossNode = (CCNode*)ccbReader->readNodeGraphFromFile("dropmonster.ccbi");
// 
// 	CCBAnimationManager* animationManager = ((CCBAnimationManager*)_bossNode->getUserObject());
// 
// 	animationManager->setAnimationCompletedCallback(this, callfunc_selector(GameLayer::bossAppearCallback));
// 
// 	ccbReader->release();
// 
// 	this->getParent()->addChild(_bossNode);

}

void GameLayer::bossAppearCallback()
{
	if(_bossNode)
	{
		_bossNode->removeFromParentAndCleanup(true);
	}

	NotificationCenter::getInstance()->postNotification(kMSG_GameLayer_LayerInitComplete);

	beforeMatch();
}
void GameLayer::buyFiveSteps(Ref *obj)
{
	_hightLightSwitch = true;
	findPotentialOrRenew();
}
