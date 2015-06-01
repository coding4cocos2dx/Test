#include "Gem.h"

#include <time.h>

MapInfo*									Gem::_mapInfo = NULL;
Node*										Gem::_particleNode = NULL;
Gem***									Gem::_gemStoneMatrix=NULL;
int**											Gem::_mapMatrix = NULL;
Node*										Gem::_mapNode = NULL;
vector<AnimationWraper>*	Gem::_animationWraperVector = NULL;
vector<MyPoint>*					Gem::_emptyPointVector = NULL;
vector<ConnectionArea>*		Gem::_connectionAreaVector=NULL;
int											Gem::_grassguyCount = 0;

Gem::Gem()
	:_type((GemType)0)
	,_state(0)
	,_frozen(0)
	,_restrain(0)
	,_change(NoChange)
	,_score(1)
	,_sick(false)
	,_GemKind(NormalGem)
	,_frozenNode(NULL)
	,_restrainNode(NULL)
	,_flag(0)
	,_scoreSum(0)
	,_countLabel(NULL)
	,_labelBackGround(NULL)
{
	_actionArray = Array::create();  //  取消掉 

	CC_SAFE_RETAIN(_actionArray);
}
Gem::~Gem()
{
	if(_countLabel)
	{
		_countLabel->removeFromParentAndCleanup(true);
	}
	if(_labelBackGround)
	{
		_labelBackGround->removeFromParentAndCleanup(true);
	}
	if(_frozenNode&&_frozen>0)
	{
		_frozenNode->removeFromParentAndCleanup(true);
	}
	if(_restrainNode&&_restrain>0)
	{
		_restrainNode->removeFromParentAndCleanup(true);
	}
	if(_spr)
	{
		_spr->removeFromParentAndCleanup(true);
	}
	clearAllAction();
	CC_SAFE_RELEASE_NULL(_actionArray);
	this->removeAllChildrenWithCleanup(true);
	if (_vecActions.size())
	{
		_vecActions.clear();
	}
}

Gem* Gem::createRandomGem(Node* layer,Point point,bool visible)
{
	int index = 0;

	GemType fType;

	if(/*_mapInfo->isHasSeed()*/false)
	{
		// 		if(_grassguyCount<2)
		// 		{
		// 			fType = grassguy;
		// 		}
		// 		else
		// 		{
		// 			do{
		// 				index = arc4random()%_mapInfo->getNumOfGemType();
		// 				fType = (GemType)_mapInfo->getGemTypeArray()[index];
		// 			}while(fType==grassguy);
		// 		}
	}
	else
	{
		/*srand ((unsigned)time(nullptr));*/
		index = rand()%7/*_mapInfo->getNumOfGemType()*/;
		fType = (GemType)(index + 1)/*_mapInfo->getGemTypeArray()[index]*/;
	}
	Gem *fruit = createFixGem(fType,layer,point,visible);
	return fruit;
}

void Gem::setMapInfo(Node *particleNode,Node* mapNode, Gem ***matrix, MapInfo *mapInfo,vector<AnimationWraper>* aw,vector<MyPoint>* pvector,vector<ConnectionArea>* connectVector,int** mapMatrix)
{
	_particleNode = particleNode;

	_mapNode = mapNode;

	_gemStoneMatrix = matrix;

	_mapInfo = mapInfo;

	_mapMatrix = mapMatrix;

	_animationWraperVector = aw;

	_emptyPointVector = pvector;

	_connectionAreaVector = connectVector;

	_grassguyCount = 0;
}

void Gem::addCollectGemScore()
{
	int param = _type*1000+_score;

	NotificationCenter::sharedNotificationCenter()->postNotification(kMSG_UpdateCollecte,(Ref*)param);
}

void Gem::addCollectGemScoreUpdate()
{
	NotificationCenter::sharedNotificationCenter()->postNotification(kMSG_ShowCollecte,(Ref*)_type);
}

Gem* Gem::createFixGem(GemType type,Node *layer,Point point,bool visible)
{
	Gem *gem = NULL;

	switch (type) {
	case empty:
		return NULL;
		break;
	case red:
		gem = new RedGem();
		break;
	case blue:
		gem = new BlueGem();
		break;
	case green:
		gem = new GreenGem();
		break;
	case white:
		gem = new WhiteGem();
		break;
	case yellow:
		gem = new YellowGem();
		break;
	case purple:
		gem = new PurpleGem();
		break;
	case orange:
		gem = new OrangeGem();
		break;
// 	case tiger:
// 		gem = new Tiger();
// 		break;
// 	case fishinbubble:
// 		gem = new FishInBubble();
// 		break;
// 	case colorfullegg:
// 		gem = new ColorfullEgg();
// 		break;
// 	case henhouse:
// 		gem = new Henhouse();
// 		break;
// 	case butterfly:
// 		gem = new Butterfly();
// 		break;
// 	case chicken:
// 		gem = new Chicken();
// 		break;
// 	case grassguy:
// 		gem = new Grassguy();
// 		break;
	default:
		break;
	}

	if(gem&&gem->createGemSprite(layer, point,visible))
	{
		gem->autorelease();

		return gem;
	}
	CC_SAFE_DELETE(gem);

	return NULL;
}
void Gem::removeCollect(const Point& dist,float time,bool playMusic)
{
	Action* flyAction = GemAction::getInstance().getCollectGemAction(this->getPosition(),dist,time);

	GemAction* caller = &(GemAction::getInstance());

	CCCallFuncND* explodeRandomMusic=NULL;

	if(playMusic)
	{
		/*srand(time(0));*/
		explodeRandomMusic = CCCallFuncND::create(this, callfuncND_selector(Gem::playExplodeRandomMusic), (void*)(rand()%5));
	}
	else
	{
		explodeRandomMusic = CCCallFuncND::create(this, callfuncND_selector(Gem::playExplodeRandomMusic), (void*)-1);
	}

	this->runAction(Sequence::create(CallFunc::create(this,callfunc_selector(Gem::explosionParticle)),explodeRandomMusic,CallFunc::create(this,callfunc_selector(Gem::addCollectGemScore)),flyAction,CallFunc::create(this,callfunc_selector(Gem::addCollectGemScoreUpdate))/*,CCCallFuncND::create(caller,callfuncND_selector(GemAction::playEffectMusic),(void*)kMusic_Explode_Collect)*/,CallFunc::create(this,callfunc_selector(Gem::hideSprite)),CallFunc::create(this,callfunc_selector(Gem::removeGem)),NULL));
}
void Gem::removeNoCollect(bool playMusic)
{
	CCCallFuncND* explodeRandomMusic=NULL;

	if(playMusic)
	{
		srand ((unsigned)time(nullptr));
		explodeRandomMusic = CCCallFuncND::create(this, callfuncND_selector(Gem::playExplodeRandomMusic), (void*)(rand()%5));
	}
	else
	{
		explodeRandomMusic = CCCallFuncND::create(this, callfuncND_selector(Gem::playExplodeRandomMusic), (void*)-1);
	}

	this->runAction(Sequence::create(ScaleTo::create(kRemoveScaleLargeTime,1.2),explodeRandomMusic,ScaleTo::create(kRemoveScaleSmallTime,0.8),CallFunc::create(this,callfunc_selector(Gem::explosionParticle)),CallFunc::create(this,callfunc_selector(Gem::hideSprite)),CallFunc::create(this, callfunc_selector(Gem::removeGem)),NULL));
}
void Gem::removeGem()
{
	this->clearAllAction();

	this->removeFromParentAndCleanup(true);
}

void Gem::createGemSpriteByName(Node *layer,Point point,bool visible,const char* name)
{
	_spr = Sprite::create(name);

	_spr->setPosition(Point(0,-kElementSize/2));
	
	_spr->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

	this->addChild(_spr);

	this->setPosition(point);

	this->setVisible(visible);

	layer ->addChild(this);

	if(_GemKind==SpecialGem)
	{
		return;
	}
// 	_countLabel = LabelBMFont::create("", "item.fnt");
// 
// 	_countLabel->setPosition(ccp(17,17));//(ccp(kElementSize/3,kElementSize/5));
// 
// 	_countLabel->setScale(0.7);
// 
// 	_labelBackGround = CCSprite::createWithSpriteFrameName("yellow_back.png");
// 
// 	_labelBackGround->setPosition(ccp(kElementSize/3-2,kElementSize/5-2));
// 
// 	_labelBackGround->setScale(1.5);
// 
// 	_labelBackGround->addChild(_countLabel);
// 
// 	_spr->addChild(_labelBackGround);

	addScore(0,false);
	//判断生成时是否为病态

	if(/*!_mapInfo->getHeroMode()&&_mapInfo->isBadGemType(_type)*/false)
	{
		setSick(true);
		becomeSick();
	}
}

Node* Gem::createNodeFromFile(Node *father,Point point,const char* name)
{
// 	CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
// 
// 	CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
// 
// 	CCNode* spr = ccbReader->readNodeGraphFromFile(name);
// 
// 	spr->setPosition(point);
// 
// 	father->addChild(spr);
// 
// 	ccbReader->release();
	Node *spr = Node::create();
	return spr;
}
void Gem::addFrozen()
{
	if(_frozen==0)
	{
		_frozenNode = Gem::createNodeFromFile(this, Point(0,0) ,"ice.ccbi");

		_frozen = 1;
	}
}
void Gem::addRestrain()
{
	if(_restrain==0)
	{
		_restrainNode = Gem::createNodeFromFile(this, Point(0,0),"tengtiao.ccbi");

		_restrain = 1;
	}
}
void Gem::addChange(int count)
{
	if(_change!=Eliminate)
	{
		_change = (ChangeState)(_change+count);
	}
}
GemType Gem::getGemType()
{
	return _type;
}
bool Gem::canMove()
{
	if (_frozen==0&&_restrain==0)
	{
		return true;
	}
	return false;
}

void Gem::highLight()
{
	this->setZOrder(1);

	if(_frozen>0||_restrain>0)
	{
		this->setZOrder(0);

		ActionInterval* action = Sequence::create(RotateTo::create(0.2, 20),RotateTo::create(0.4,-20),RotateTo::create(0.28, 0),NULL);

		this->runAction(action);
	}
	else
	{
		FiniteTimeAction* spawn = Spawn::create(JumpBy::create(0.2, Point(0,0), kElementSize/2, 1),ScaleTo::create(0.2, 1,1),NULL);

		Sequence* seq = Sequence::create(ScaleTo::create(0.2,1.4,0.5),ScaleTo::create(0.2, 0.6,1.3),spawn,ScaleTo::create(0.12, 1.2,0.7),ScaleTo::create(0.12,0.8,1.1),ScaleTo::create(0.12,1.1,0.9),ScaleTo::create(0.12, 1,1),NULL);

		_spr->runAction(seq);
	}
}
void Gem::stopHighLight()
{
	this->setZOrder(0);

	this->stopAllActions();

	this->setRotation(0);

	this->setScale(1);

	this->brightCallback();

	_spr->stopAllActions();

	_spr->setPosition(Point(0,-kElementSize/2));

	_spr->setScale(1);
}
void Gem::idle()
{
	if(_sick||_frozen>0) return;

	if(_spr->numberOfRunningActions()>0||this->numberOfRunningActions()>0) return;

	this->runAnimition(NULL, (void*)kIdleAnimation);
}
void Gem::selected()
{
	if(_frozen>0||_restrain>0)
	{
		return;
	}
	zhayan();

	Sequence *seqAnimal = Sequence::create(ScaleTo::create(0.1, 1.2,0.8),ScaleTo::create(0.1,0.8, 1.2),ScaleTo::create(0.1, 1.1,0.9),ScaleTo::create(0.1,0.9, 1.1),ScaleTo::create(0.1,1),NULL);

	_spr->runAction(seqAnimal);

}
void Gem::zhayan()
{
	if(_GemKind==NormalGem&&!_sick)
	{
		this->runAnimition(NULL, (void*)kZhayan);
	}
}
void Gem::becomeSick()
{
// 	stopHighLight();
// 	this->runAnimition(NULL, (void*)kBecomeSick);
// 	GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Gem_sick);
}
void Gem::becomeWell()
{
// 	stopHighLight();
// 	this->runAnimition(NULL, (void*)kBecomeWell);
// 	GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Gem_curesick);
}
void Gem::fallDownStart()
{
	zhayan();

	if(_sick) return ;

	_spr->setScaleX(0.7);
}
void Gem::fallDownEnd()
{
	Sequence *seq = NULL;

	seq = Sequence::create(ScaleTo::create(0.1, 1.5 ,0.7),ScaleTo::create(0.1,0.8,1.3),ScaleTo::create(0.1 , 1.2 ,0.9),ScaleTo::create(0.1 ,1),CallFunc::create(this,callfunc_selector(Gem::sendFallDownEndMSG)),NULL);

	_spr->runAction(seq);
}
void Gem::sendFallDownEndMSG()
{
	NotificationCenter::sharedNotificationCenter()->postNotification(kMSG_GemFallDown);
}
void Gem::bright()
{
	GemAction::getInstance().setBright(_spr);

	Sequence* seq = Sequence::create(ScaleTo::create(kBrightScaleTime, 1.3),ScaleTo::create(kBrightScaleTime, 1),CallFunc::create(this,callfunc_selector(Gem::brightCallback)),NULL);

	this->runAction(seq);
}
void Gem::brightCallback()
{
	GemAction::getInstance().removeBright(_spr);
}
void Gem::hideScore()
{
// 	_countLabel->setVisible(false);
// 	_labelBackGround->setVisible(false);
}
void Gem::showScore()
{
// 	_countLabel->setVisible(true);
// 	_labelBackGround->setVisible(true);
}
void Gem::frozenOff()
{
	if(!_frozenNode) return;

// 	CCBAnimationManager* ccba = (CCBAnimationManager*)_frozenNode->getUserObject();
// 
// 	if(ccba)
// 	{
// 		ccba->setAnimationCompletedCallback(this, callfunc_selector(Gem::frozenOffCallback));
// 
// 		ccba->runAnimationsForSequenceNamed(kFrozenOff);
// 	}
}
void Gem::restrainOff()
{
	if(!_restrainNode) return;

// 	CCBAnimationManager* ccba = (CCBAnimationManager*)_restrainNode->getUserObject();
// 
// 	if(ccba)
// 	{
// 		ccba->setAnimationCompletedCallback(this, callfunc_selector(Gem::restrainOffCallback));
// 
// 		ccba->runAnimationsForSequenceNamed(kRestrainOff);
// 	}
}
void Gem::frozenOffCallback()
{
	_frozenNode->removeFromParentAndCleanup(true);
}
void Gem::restrainOffCallback()
{
	_restrainNode->removeFromParentAndCleanup(true);
}
void Gem::fallDownToEnd()
{
	if(_actionArray->count()==0)
	{
		return;
	}
	if (_vecActions.size() == 0)
	{
		return;
	}

	FiniteTimeAction* afterMoveAnimation = CallFunc::create(this,callfunc_selector(Gem::fallDownEnd));

	addAction(afterMoveAnimation);

	this->runAction(Sequence::create(CallFunc::create(this,callfunc_selector(Gem::fallDownStart)),Sequence::create(_vecActions),NULL));

}

void Gem::addAction(FiniteTimeAction* action)
{
	_actionArray->addObject(action);
	_vecActions.pushBack(action);
}

void Gem::runAnimition(Node* sender,void *animitionName)
{
// 	CCBAnimationManager* ccba = (CCBAnimationManager*)_spr->getUserObject();
// 
// 	ccba->runAnimationsForSequenceNamed((char*)animitionName);
}

void Gem::swap(Direction direction, bool reverse,bool isSelected,CallFunc* callbackAction)
{
	FiniteTimeAction *firstAction,*reverseAction,*swapAction,*selectScale1,*selectScale2,*nextScale1,*nextScale2,*delayTime;

	swapAction = GemAction::getInstance().getGemChangeAction(direction);

	delayTime = DelayTime::create(0.08);

	GemAction* caller = &(GemAction::getInstance());

	if(reverse)
	{
		if(direction==Up||direction==Down)
		{
			selectScale1 = ScaleTo::create(kChangeScaleFirstStepTime,0.7,1.5);
			selectScale2 = ScaleTo::create(kchangeScaleSecondStepTime,1,1);
		}
		else
		{
			selectScale1 = ScaleTo::create(kChangeScaleFirstStepTime,1.5,0.7);
			selectScale2 = ScaleTo::create(kchangeScaleSecondStepTime,1,1);
		}

		firstAction = Spawn::create(Sequence::create(selectScale1,selectScale2,NULL),swapAction,NULL);


		if(isSelected)
		{
			reverseAction =Spawn::create(Sequence::create(selectScale1,selectScale2,NULL),EaseExponentialIn::create(GemAction::getInstance().getGemChangeAction(Direction(Down-direction))),NULL);

			this->runAction(Sequence::create(firstAction,delayTime,reverseAction,callbackAction,/*CCCallFuncND::create(caller,callfuncND_selector(GemAction::playEffectMusic),(void*)kMusic_Gem_Switch),*/NULL));
		}
		else
		{
			this->setZOrder(-1);

			nextScale1 = ScaleTo::create(kChangeScaleFirstStepTime, 0.3,0.3);
			nextScale2 = ScaleTo::create(kchangeScaleSecondStepTime, 1,1);

			reverseAction =Spawn::create(Sequence::create(nextScale1,nextScale2,NULL),EaseExponentialIn::create(GemAction::getInstance().getGemChangeAction(Direction(Down-direction))),NULL);

			this->runAction(Sequence::create(firstAction,delayTime,reverseAction,CallFunc::create(this, callfunc_selector(Gem::swapCallback)),callbackAction/*,CCCallFuncND::create(caller,callfuncND_selector(GemAction::playEffectMusic),(void*)kMusic_Gem_Switch)*/,NULL));
		}
	}
	else
	{
		if(isSelected)
		{
			if(direction==Up||direction==Down)
			{
				selectScale1 = ScaleTo::create(kChangeScaleFirstStepTime,0.7,1.5);
				selectScale2 = ScaleTo::create(kchangeScaleSecondStepTime,1,1);
			}
			else
			{
				selectScale1 = ScaleTo::create(kChangeScaleFirstStepTime,1.5,0.7);
				selectScale2 = ScaleTo::create(kchangeScaleSecondStepTime,1,1);
			}

			firstAction = Spawn::create(Sequence::create(selectScale1,selectScale2,NULL),swapAction,NULL);

			this->runAction(Sequence::create(firstAction,callbackAction/*,CCCallFuncND::create(caller,callfuncND_selector(GemAction::playEffectMusic),(void*)kMusic_Gem_Switch)*/,NULL));
		}
		else
		{
			nextScale1 = ScaleTo::create(kChangeScaleFirstStepTime, 0.3,0.3);
			nextScale2 = ScaleTo::create(kchangeScaleSecondStepTime, 1,1);

			firstAction = Spawn::create(Sequence::create(nextScale1,nextScale2,NULL),swapAction,NULL);
			this->setZOrder(-1);

			this->runAction(Sequence::create(firstAction,CallFunc::create(this, callfunc_selector(Gem::swapCallback)),DelayTime::create(0.2),callbackAction/*,CCCallFuncND::create(caller,callfuncND_selector(GemAction::playEffectMusic),(void*)kMusic_Gem_Switch)*/,NULL));
		}
	}
}

void Gem::swapCallback()
{
	this->setZOrder(0);
}
void Gem::clearAllAction()
{
	if(_actionArray&&_actionArray->count()>0)
	{
		_actionArray->removeAllObjects();
	}
	if (_vecActions.size())
	{
		_vecActions.clear();
	}
}
//单个消除效果
void Gem::explosionParticle()
{
// 	CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
// 
// 	CCBReader *ccbReader1 = new  CCBReader(ccNodeLoaderLibrary);
// 	CCParticleSystemQuad * node1 = (CCParticleSystemQuad*)ccbReader1->readNodeGraphFromFile(kAfterEliminateParticle1);
// 	ccbReader1->release();
// 	node1->setAutoRemoveOnFinish(true);
// 
// 	node1->setStartColor(ccc4FFromccc3B(_particleColor1));
// 	node1->setEndColor(ccc4FFromccc3B(_particleColor1));
// 	node1->setPosition(this->getPosition());
// 	node1->setScale(1);
// 	_mapNode->addChild(node1);
// 
// 	CCBReader *ccbReader2 = new  CCBReader(ccNodeLoaderLibrary);
// 	CCParticleSystemQuad * node2 = (CCParticleSystemQuad*)ccbReader2->readNodeGraphFromFile(kAfterEliminateParticle2);
// 	ccbReader2->release();
// 	node2->setAutoRemoveOnFinish(true);
// 	node2->setStartColor(ccc4FFromccc3B(_particleColor1));
// 	node2->setEndColor(ccc4FFromccc3B(_particleColor1));
// 	node2->setPosition(this->getPosition());
// 	node2->setScale(1.2);
// 	_mapNode->addChild(node2);

	this->setZOrder(1);
}
void Gem::fiveMatchEffect(Node* sender,void* data)
{
	bright();

	Sprite* spr = (Sprite*)sender;

	spr->removeFromParentAndCleanup(true);
}

//四消效果
void Gem::fourMatchEffect(Node* sender,void* data)
{
	bright();

	affected(1);

	Sprite* spr = (Sprite*)sender;

	Sprite* sparkle = Sprite::create("sparkle3.png");

	sparkle->setPosition(this->getPosition());

	_particleNode->addChild(sparkle);

	Sequence* seq1 = Sequence::create(DelayTime::create(kBrightScaleTime),ScaleTo::create(0.2, 4),ScaleTo::create(0.1,1),CallFunc::create(sparkle,callfunc_selector(Sprite::removeFromParent)),CallFunc::create(spr,callfunc_selector(Sprite::removeFromParent)),NULL);

	Sequence* seq2 = Sequence::create(DelayTime::create(kBrightScaleTime*2),ScaleTo::create(0.3, 1.3),ScaleTo::create(0.1, 1),NULL);

	sparkle->runAction(seq1);

	this->runAction(seq2);
}
void Gem::addScore(int count,bool isAffect)
{
	if(count<1)
	{
		hideScore();

		char str[5]={0};

		std::sprintf(str,"%d",0);

		/*_countLabel->setString(str);*/

		_score = 1;

		return;
	}
	else
	{
		char str[5]={0};

		std::sprintf(str,"*%d",_score+count);

		/*_countLabel->setString(str);*/

		showScore();

		_score+=count;
	}

	Sequence* seq1 = Sequence::create(ScaleTo::create(0.1, 3),ScaleTo::create(0.1, 1.5),NULL);
	/*_labelBackGround->runAction(seq1);*/
}
bool Gem::equal(Gem *fruit)
{
	return (fruit!=NULL&&_type==fruit->getGemType());
}

//设置生病和病好，生成动画加入队列
void Gem::setSick(bool sick)
{
	if(_sick&&!sick)
	{
		AnimationWraper aw(this,e_aid_normal_becomewell, e_priority_normal_becomewell);
		_animationWraperVector->push_back(aw);
	}
	else if(!_sick&&sick)//生病
	{
		AnimationWraper aw(this,e_aid_normal_becomesick, e_priority_normal_becomesick);
		_animationWraperVector->push_back(aw);
		addScore(0,false);//抹去角标
	}

	_sick = sick;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//三个元素相连，针对每个调用此方法，出入聚合中心点及是否变病态                                                        //
//1,判断_state，_frozen,_restrain                                                                           //
//1)冰冻或者束缚大于0，则减一，执行动画                                                                         //
//2)冰冻或束缚等于0，减一，判断是不是可消元素，将动画加入队列                                                      //
//3)聚合根据聚合目的地，创建聚合动画,加入队列，如果本聚合元素下一动作为可消除，则将飞走动作加入队列                      //
//4)病态的将得分设置为0,如果之前是非病态，调用变病态动画                                                         //
//5)相连元素其中有一个是病态的，则本排所有元素变为病态                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Gem::explode(MyPoint &myPoint,MyPoint &distPoint,int count,int index)
{
	//冰冻或者束缚
	if(_frozen>0||_restrain>0)
	{
		if(_frozen>0)
		{
			AnimationWraper aw(this,e_aid_normal_frozenoff,e_priority_normal_frozenoff);

			_animationWraperVector->push_back(aw);

			_frozen--;
		}
		else if(_restrain>0)
		{
			AnimationWraper aw(this,e_aid_normal_restrainoff,e_priority_normal_restrainoff);

			_animationWraperVector->push_back(aw);

			_restrain--;
		}
		return;
	}

	//普通元素，非冰冻/束缚
	//单个消除不对周围产生影响
	if(-1!=_state)//判断是否已经被消除，不能同时消除两次
	{
		if(index==1)
		{
			setFlag(1);
		}

		_state=-1;
	}

	if(count>0)
	{
		int i = myPoint.x,j=myPoint.y;

		if(i-1>=0&&_gemStoneMatrix[i-1][j])
		{
			_gemStoneMatrix[i-1][j]->affected(count);
		}
		if(i+1<kMatrixWidth&&_gemStoneMatrix[i+1][j])
		{
			_gemStoneMatrix[i+1][j]->affected(count);
		}
		if(j-1>=0&&_gemStoneMatrix[i][j-1])
		{
			_gemStoneMatrix[i][j-1]->affected(count);
		}
		if(j+1<kMatrixWidth&&_gemStoneMatrix[i][j+1])
		{
			_gemStoneMatrix[i][j+1]->affected(count);
		}
	}
}
//1,判断自己是不是相连，如果相连元素则返回
//2,判断是不是病态，病态元素返回
//3,设置change字段为被影响
//4,普通元素改变累积值
//5,固定元素和派生元素改变状态，并执行动画
//6,变成病态
void Gem::affected(int count)
{
	//病态元素和消除元素直接返回
	if(_sick||_change==Eliminate||_frozen>0||_restrain>0||_state==-1)
	{
		return;
	}

	if(/*_mapInfo->getPassLevelCCNode(_type)*/true) //判断是否是收集的元素
	{
		_change = Changed;

		AnimationWraper aw(this,e_aid_normal_addscore,e_priority_normal_affectaddscore);

		aw.score = count;

		_animationWraperVector->push_back(aw);
	}
}

void Gem::beforeMatch(int i, int j)
{
	//根据土地属性更改本元素sick值
	if(_sick&&_mapMatrix[i][j]==Fertile)
	{
		setSick(false);
	}
	else if(!_sick&&_mapMatrix[i][j]==InFertile)
	{
		setSick(true);
	}
}
void Gem::afterMatch(int i, int j)
{
	if(_change<=Eliminate&&_state!=-1)
	{
		_change = NoChange;
	}
	if(_change>=Changed)
	{
		_change = NoChange;
	}
}
void Gem::growOnFertile(int i, int j)
{
// 	if(_mapMatrix[i][j]==Fertile&&_GemKind==NormalGem&&
// 		_mapInfo->getPassLevelCCNode(_type)&&this->canMove())
// 	{
// 		AnimationWraper aw(this,e_aid_normal_addscore,e_priority_normal_fertileaddscore);
// 
// 		aw.score = 2;
// 
// 		_animationWraperVector->push_back(aw);
// 	}

}

int Gem::getUniqueNumber(int max)
{
	static int flag[kMatrixWidth*kMatrixWidth] = {0};

	if(max==-1)
	{
		for(int i=0;i<kMatrixWidth*kMatrixWidth;i++)
		{
			flag[i] = i;
		}

		return 0;
	}
	else
	{
		if(max<1) return flag[0];

		srand ((unsigned)time(nullptr));

		int t = rand()%max;

		int result = flag[t];

		flag[t] = flag[max-1];

		return result;
	}
}

MyPoint Gem::getCurrentIndex(const Point& point)
{
	return MyPoint((point.x-kStartX)/kElementSize,(point.y-kStartY)/kElementSize);
}

int Gem::numbersOfSprRunningAction()
{
	return _spr->numberOfRunningActions();
}

void Gem::winnerModeStart()
{
	bright();

	Sprite* sparkle = Sprite::create("sparkle3.png");

	sparkle->setPosition(this->getPosition());

	_particleNode->addChild(sparkle);

	Sequence* seq1 = Sequence::create(DelayTime::create(kBrightScaleTime),ScaleTo::create(0.2, 4),ScaleTo::create(0.1,1),CallFunc::create(sparkle,callfunc_selector(Sprite::removeFromParent)),NULL);

	Sequence* seq2 = Sequence::create(DelayTime::create(kBrightScaleTime*2),ScaleTo::create(0.3, 1.3),ScaleTo::create(0.1, 1),NULL);

	sparkle->runAction(seq1);

	this->runAction(seq2);

// 	CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
// 
// 	CCBReader *ccbReader1 = new  CCBReader(ccNodeLoaderLibrary);
// 
// 	CCNode* node = (CCNode*)ccbReader1->readNodeGraphFromFile("heropoint.ccbi");
// 
// 	ccbReader1->release();
// 
// 	node->setPosition(this->getPosition());
// 
// 	_particleNode->addChild(node);
}
void Gem::winnerMode()
{
// 	CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
// 
// 	CCBReader *ccbReader1 = new  CCBReader(ccNodeLoaderLibrary);
// 
// 	CCNode * node1 = (CCNode*)ccbReader1->readNodeGraphFromFile(kWinnerModeLine);
// 
// 	ccbReader1->release();

	MyPoint p = getCurrentIndex(this->getPosition());

	if(p.x%2==0)
	{
/*		node1->setRotation(90)*/;

		for(int i=p.x,j=0;j<kMatrixWidth;j++)
		{
			if(_gemStoneMatrix[i][j])
			{
				_gemStoneMatrix[i][j]->bright();
			}
		}
	}
	else
	{
		for(int i=0,j=p.y;i<kMatrixWidth;i++)
		{
			if(_gemStoneMatrix[i][j])
			{
				_gemStoneMatrix[i][j]->bright();
			}
		}
	}

// 	node1->setPosition(this->getPosition());
// 
// 	_particleNode->addChild(node1);
}

void Gem::propSingle()
{
// 	CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
// 
// 	CCBReader *ccbReader1 = new  CCBReader(ccNodeLoaderLibrary);
// 
// 	CCNode * node1 = (CCNode*)ccbReader1->readNodeGraphFromFile("mobang.ccbi");
// 
// 	ccbReader1->release();
// 
// 	node1->setPosition(this->getPosition());
// 
// 	_particleNode->addChild(node1);
// 
// 	this->runAction(CCSequence::create(CCDelayTime::create(1),CCCallFunc::create(this, callfunc_selector(Gem::bright)),NULL));
}

void Gem::playExplodeRandomMusic(Node* sender,void* data)
{
	int kind = (int)data;

	switch (kind) {
// 	case 0:
// 		GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Explode_Random1);
// 		break;
// 	case 1:
// 		GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Explode_Random2);
// 		break;
// 	case 2:
// 		GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Explode_Random3);
// 		break;
// 	case 3:
// 		GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Explode_Random4);
// 		break;
// 	case 4:
// 		GemAction::getInstance().playEffectMusic(NULL, (void*)kMusic_Explode_Random5);
// 		break;
	default:
		break;
	}
}

