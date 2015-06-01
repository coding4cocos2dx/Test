#include "MapMatrix.h"


MapMatrix::MapMatrix(int **mm,Gem ***fm, int count,MapInfo* mapinfo)
	:_mapMatrix(mm)
	,_gemMatrix(fm)
	,edgeLength(count)
	,_mapInfo(mapinfo)
{
	int i,j;

	for(int i=0;i<kMatrixWidth;i++)
	{
		_newGemCounter[i] = 0;

		_newGemCounterFromMiddle[i] = 0;

		fallDownFlagArray[i]=0;
	}

	for(i=0;i<kMatrixWidth;i++)
	{
		for(j=0;j<kMatrixWidth;j++)
		{
			if(_mapMatrix[i][j]==NoneInMiddle||(j==kMatrixWidth-1&&_mapMatrix[i][j]!=NoneInEdge))
			{
				fallDownFlagArray[i]=1;
				continue;
			}
		}
	}

	int distance;

	for(i=0;i<kMatrixWidth;i++)
	{
		distance = kMatrixWidth;

		if(fallDownFlagArray[i]==0)
		{
			for(j=i+1;j<kMatrixWidth;j++)
			{
				if(fallDownFlagArray[j]==1&&distance>j-i+1)
				{
					distance=j-i+1;
					continue;
				}
			}
			for(j=i-1;j>=0;j--)
			{
				if(fallDownFlagArray[j]==1&&distance>i-j+1)
				{
					distance=i-j+1;
					continue;
				}
			}
			if(distance==kMatrixWidth)
			{
				fallDownFlagArray[i]=0;
			}
			else
			{
				fallDownFlagArray[i] = distance;
			}
		}
	}

}
MapMatrix::~MapMatrix()
{

}
void MapMatrix::findConnection()
{
	int i,j,_count;
	Gem* tempGem;

	connectionAreaVector.clear();
	for(i=0;i<edgeLength;i++)
	{
		for(j=0;j<edgeLength;j++)
		{
			if(!_gemMatrix[i][j])
			{
				continue;
			}
			else
			{
				tempGem = _gemMatrix[i][j];

				int _count = 0;

				do{
					j++;
					_count++;
				}while(j<edgeLength&&tempGem->equal(_gemMatrix[i][j]));

				if(_count>=3)
				{
					ConnectionArea ca;
					ca.startPoint.x = i;
					ca.startPoint.y = j - _count;
					ca.count = _count;
					ca.direction = Up;
					connectionAreaVector.push_back(ca);
				}
				if(j+1>=edgeLength)
				{
					break;
				}
				else
				{
					j--;
				}
			}

		}
	}

	for(j=0;j<edgeLength;j++)
	{
		for(int i=0;i<edgeLength;i++)
		{
			if(!_gemMatrix[i][j])
			{
				continue;
			}
			else
			{
				tempGem = _gemMatrix[i][j];

				_count = 0;

				do{
					i++;
					_count++;
				}while(i<edgeLength&&tempGem->equal(_gemMatrix[i][j]));

				if(_count>=3)
				{
					ConnectionArea ca;
					ca.startPoint.x = i - _count;
					ca.startPoint.y = j;
					ca.count = _count;
					ca.direction = Right;
					connectionAreaVector.push_back(ca);
				}
				if(i+1>=edgeLength)
				{
					break;
				}
				else
				{
					i--;
				}
			}
		}
	}
}

void MapMatrix::findPotential()
{
	int i,j;

	Gem *f,*f1,*f2,*f3,*f4;

	potentialAreaVector.clear();

	for(i=0;i<edgeLength;i++)
	{
		for(j=0;j<edgeLength;j++)
		{
			if(!_gemMatrix[i][j]||(_gemMatrix[i][j]&&_gemMatrix[i][j]->canMove()==false))
			{
				continue;
			}
			else
			{
				f = _gemMatrix[i][j];

				f1=f2=f3=f4 = NULL;

				if(i-1>=0&&j-1>=0&&_gemMatrix[i-1][j-1])
					f1 = _gemMatrix[i-1][j-1];
				if(i-1>=0&&j+1<edgeLength&&_gemMatrix[i-1][j+1])
					f2 = _gemMatrix[i-1][j+1];
				if(i+1<edgeLength&&j+1<edgeLength&&_gemMatrix[i+1][j+1])
					f3 = _gemMatrix[i+1][j+1];
				if(i+1<edgeLength&&j-1>=0&&_gemMatrix[i+1][j-1])
					f4 = _gemMatrix[i+1][j-1];
				if(f->equal(f1)&&f->equal(f2)&&_gemMatrix[i-1][j]&&_gemMatrix[i-1][j]->canMove())
				{
					PotentialArea pa;
					pa.movePoint.setPosition(i,j);
					pa.point1.setPosition(i-1, j-1);
					pa.point2.setPosition(i-1, j+1);
					pa.diretion = Left;
					potentialAreaVector.push_back(pa);
				}
				if(f->equal(f2)&&f->equal(f3)&&_gemMatrix[i][j+1]&&_gemMatrix[i][j+1]->canMove())
				{
					PotentialArea pa;
					pa.movePoint.setPosition(i, j);
					pa.point1.setPosition(i-1, j+1);
					pa.point2.setPosition(i+1, j+1);
					pa.diretion = Up;
					potentialAreaVector.push_back(pa);
				}
				if(f->equal(f3)&&f->equal(f4)&&_gemMatrix[i+1][j]&&_gemMatrix[i+1][j]->canMove())
				{
					PotentialArea pa;
					pa.movePoint.setPosition(i, j);
					pa.point1.setPosition(i+1, j+1);
					pa.point2.setPosition(i+1,j-1);
					pa.diretion = Right;
					potentialAreaVector.push_back(pa);
				}
				if(f->equal(f4)&&f->equal(f1)&&_gemMatrix[i][j-1]&&_gemMatrix[i][j-1]->canMove())
				{
					PotentialArea pa;
					pa.movePoint.setPosition(i, j);
					pa.point1.setPosition(i-1, j-1);
					pa.point2.setPosition(i+1, j-1);
					pa.diretion = Down;
					potentialAreaVector.push_back(pa);
				}

			}
		}
	}

	for(i=0;i<edgeLength;i++)
	{
		for(j=0;j<edgeLength;j++)
		{
			if(!_gemMatrix[i][j])
			{
				continue;
			}
			else
			{
				f = _gemMatrix[i][j];

				if(j+1<edgeLength&&f->equal(_gemMatrix[i][j+1]))
				{
					if(i+1<edgeLength&&j+2<edgeLength&&f->equal(_gemMatrix[i+1][j+2])&&_gemMatrix[i+1][j+2]->canMove())
					{
						if(_gemMatrix[i][j+2]&&_gemMatrix[i][j+2]->canMove())
						{
							PotentialArea pa;
							pa.movePoint.setPosition(i+1, j+2);
							pa.point1.setPosition(i, j);
							pa.point2.setPosition(i, j+1);
							pa.diretion = Left;
							potentialAreaVector.push_back(pa);
						}
					}
					if(j+3<edgeLength&&f->equal(_gemMatrix[i][j+3])&&_gemMatrix[i][j+3]->canMove())
					{
						if(_gemMatrix[i][j+2]&&_gemMatrix[i][j+2]->canMove())
						{
							PotentialArea pa;
							pa.movePoint.setPosition(i, j+3);
							pa.point1.setPosition(i, j);
							pa.point2.setPosition(i, j+1);
							pa.diretion = Down;
							potentialAreaVector.push_back(pa);
						}
					}
					if(i-1>=0&&j+2<edgeLength&&f->equal(_gemMatrix[i-1][j+2])&&_gemMatrix[i-1][j+2]->canMove())
					{
						if(_gemMatrix[i][j+2]&&_gemMatrix[i][j+2]->canMove())
						{
							PotentialArea pa;
							pa.movePoint.setPosition(i-1, j+2);
							pa.point1.setPosition(i, j);
							pa.point2.setPosition(i, j+1);
							pa.diretion = Right;
							potentialAreaVector.push_back(pa);
						}
					}

					if(i+1<edgeLength&&j-1>=0&&f->equal(_gemMatrix[i+1][j-1])&&_gemMatrix[i+1][j-1]->canMove())
					{
						if(_gemMatrix[i][j-1]&&_gemMatrix[i][j-1]->canMove())
						{
							PotentialArea pa;
							pa.movePoint.setPosition(i+1, j-1);
							pa.point1.setPosition(i, j);
							pa.point2.setPosition(i, j+1);
							pa.diretion = Left;
							potentialAreaVector.push_back(pa);
						}
					}
					if(j-2>=0&&f->equal(_gemMatrix[i][j-2])&&_gemMatrix[i][j-2]->canMove())
					{
						if(_gemMatrix[i][j-1]&&_gemMatrix[i][j-1]->canMove())
						{
							PotentialArea pa;
							pa.movePoint.setPosition(i, j-2);
							pa.point1.setPosition(i, j);
							pa.point2.setPosition(i, j+1);
							pa.diretion = Up;
							potentialAreaVector.push_back(pa);
						}
					}
					if(i-1>=0&&j-1>=0&&f->equal(_gemMatrix[i-1][j-1])&&_gemMatrix[i-1][j-1]->canMove())
					{
						if(_gemMatrix[i][j-1]&&_gemMatrix[i][j-1]->canMove())
						{
							PotentialArea pa;
							pa.movePoint.setPosition(i-1, j-1);
							pa.point1.setPosition(i, j);
							pa.point2.setPosition(i, j+1);
							pa.diretion = Right;
							potentialAreaVector.push_back(pa);
						}
					}
				}
			}
		}
	}

	for(j=0;j<edgeLength;j++)
	{
		for(i=0;i<edgeLength;i++)
		{
			if(!_gemMatrix[i][j])
			{
				continue;
			}
			else
			{
				f = _gemMatrix[i][j];

				if(i+1<edgeLength&&f->equal(_gemMatrix[i+1][j]))
				{
					if(i+2<edgeLength&&j-1>=0&&f->equal(_gemMatrix[i+2][j-1])&&_gemMatrix[i+2][j-1]->canMove())
					{
						if(_gemMatrix[i+2][j]&&_gemMatrix[i+2][j]->canMove())
						{
							PotentialArea pa;
							pa.movePoint.setPosition(i+2, j-1);
							pa.point1.setPosition(i, j);
							pa.point2.setPosition(i+1, j);
							pa.diretion = Up;
							potentialAreaVector.push_back(pa);
						}
					}
					if(i+3<edgeLength&&f->equal(_gemMatrix[i+3][j])&&_gemMatrix[i+3][j]->canMove())
					{
						if(_gemMatrix[i+2][j]&&_gemMatrix[i+2][j]->canMove())
						{
							PotentialArea pa;
							pa.movePoint.setPosition(i+3, j);
							pa.point1.setPosition(i, j);
							pa.point2.setPosition(i+1, j);
							pa.diretion = Left;
							potentialAreaVector.push_back(pa);
						}
					}
					if(i+2<edgeLength&&j+1<edgeLength&&f->equal(_gemMatrix[i+2][j+1])&&_gemMatrix[i+2][j+1]->canMove())
					{
						if(_gemMatrix[i+2][j]&&_gemMatrix[i+2][j]->canMove())
						{
							PotentialArea pa;
							pa.movePoint.setPosition(i+2, j+1);
							pa.point1.setPosition(i, j);
							pa.point2.setPosition(i+1, j);
							pa.diretion = Down;
							potentialAreaVector.push_back(pa);
						}
					}

					if(i-1>=0&&j-1>=0&&f->equal(_gemMatrix[i-1][j-1])&&_gemMatrix[i-1][j-1]->canMove())
					{
						if(_gemMatrix[i-1][j]&&_gemMatrix[i-1][j]->canMove())
						{
							PotentialArea pa;
							pa.movePoint.setPosition(i-1, j-1);
							pa.point1.setPosition(i, j);
							pa.point2.setPosition(i+1, j);
							pa.diretion = Up;
							potentialAreaVector.push_back(pa);
						}
					}
					if(i-2>=0&&f->equal(_gemMatrix[i-2][j])&&_gemMatrix[i-2][j]->canMove())
					{
						if(_gemMatrix[i-1][j]&&_gemMatrix[i-1][j]->canMove())
						{
							PotentialArea pa;
							pa.movePoint.setPosition(i-2, j);
							pa.point1.setPosition(i, j);
							pa.point2.setPosition(i+1, j);
							pa.diretion = Right;
							potentialAreaVector.push_back(pa);
						}
					}
					if(i-1>=0&&j+1<edgeLength&&f->equal(_gemMatrix[i-1][j+1])&&_gemMatrix[i-1][j+1]->canMove())
					{
						if(_gemMatrix[i-1][j]&&_gemMatrix[i-1][j]->canMove())
						{
							PotentialArea pa;
							pa.movePoint.setPosition(i-1, j+1);
							pa.point1.setPosition(i, j);
							pa.point2.setPosition(i+1, j);
							pa.diretion = Down;
							potentialAreaVector.push_back(pa);
						}
					}
				}

			}
		}
	}
}

void MapMatrix::fillEmptyNew(Layer* layer, int i, int j)
{
	if(j+1>=kMatrixWidth||-3==_mapMatrix[i][j+1])//���Ϸ��ﵽ�ϲ�������
	{
		int* counterArray;

		if(j+1>=edgeLength)
		{
			counterArray = _newGemCounter;
		}
		else if(-3==_mapMatrix[i][j+1])
		{
			counterArray = _newGemCounterFromMiddle;
		}

		Gem *gem = Gem::createRandomGem(layer,Point(kStartX + i * kElementSize + kElementSize/2, kStartY + (j+1) * kElementSize + kElementSize/2),false);

		counterArray[i]++;

		for(int k=1;k<counterArray[i];k++)
		{
			ActionInterval* action =GemAction::getInstance().getGemMoveAction(StandBy);

			gem->addAction(action);
		}
		CallFunc* actionFunc = CallFunc::create(gem,callfunc_selector(Gem::showSprite));

		gem->addAction(actionFunc);

		ActionInterval* actionDown = GemAction::getInstance().getGemMoveAction(Down);

		gem->addAction(actionDown);

		_gemMatrix[i][j] = gem;

		return;
	}

	if(_gemMatrix[i][j+1]&&_gemMatrix[i][j+1]->canMove())//���Ϸ�Ԫ�ز�Ϊ���ҿ����ƶ�
	{
		ActionInterval* actionDown = GemAction::getInstance().getGemMoveAction(Down);

		_gemMatrix[i][j+1]->addAction(actionDown);

		_gemMatrix[i][j] = _gemMatrix[i][j+1];

		_gemMatrix[i][j+1] = NULL;

		fillEmptyNew(layer, i, j+1);

		return;
	}

	if(!_gemMatrix[i][j+1]&&_mapMatrix[i][j+1]!=0)//���Ϸ�Ϊ�ղ��ҿ�����Ԫ��
	{
		fillEmptyNew(layer, i, j+1);

		if(_gemMatrix[i][j+1]&&_gemMatrix[i][j+1]->canMove())
		{
			ActionInterval* actionDown = GemAction::getInstance().getGemMoveAction(Down);

			_gemMatrix[i][j+1]->addAction(actionDown);

			_gemMatrix[i][j] = _gemMatrix[i][j+1];

			_gemMatrix[i][j+1] = NULL;

			fillEmptyNew(layer, i, j+1);

			return;
		}
	}
	//���˴���δ������������ϼ��������

	if(i-1>=0)
	{
		if(_gemMatrix[i-1][j+1]&&_gemMatrix[i-1][j+1]->canMove())//��಻Ϊ�ղ��ҿ����ƶ�
		{
			ActionInterval* actionRightDown = GemAction::getInstance().getGemMoveAction(RightDown);

			_gemMatrix[i-1][j+1]->addAction(actionRightDown);

			_gemMatrix[i][j] = _gemMatrix[i-1][j+1];

			_gemMatrix[i-1][j+1] = NULL;

			fillEmptyNew(layer, i-1, j+1);

			return;
		}
		if(!_gemMatrix[i-1][j+1]&&_mapMatrix[i-1][j+1]!=NoneInEdge&&_mapMatrix[i-1][j+1]!=NoneInMiddle)//���Ϊ���ҿ����
		{
			fillEmptyNew(layer, i-1, j+1);

			if(_gemMatrix[i-1][j+1]&&_gemMatrix[i-1][j+1]->canMove())
			{
				ActionInterval* actionRightDown = GemAction::getInstance().getGemMoveAction(RightDown);

				_gemMatrix[i-1][j+1]->addAction(actionRightDown);

				_gemMatrix[i][j] = _gemMatrix[i-1][j+1];

				_gemMatrix[i-1][j+1] = NULL;

				fillEmptyNew(layer, i-1, j+1);

				return;
			}
		}
		if(_mapMatrix[i-1][j+1]==NoneInMiddle)//���Ϸ�Ϊ�ϲ㣬�ݹ���ڣ������Ϸ�Ϊ�ϲ�ͬһ����
		{
			int* counterArray = _newGemCounterFromMiddle;

			Gem *gem = Gem::createRandomGem(layer,Point(kStartX + (i-1)* kElementSize + kElementSize/2, kStartY + (j+1) * kElementSize + kElementSize/2),false);

			counterArray[i]++;

			for(int k=1;k<counterArray[i];k++)
			{
				ActionInterval* action =GemAction::getInstance().getGemMoveAction(StandBy);

				gem->addAction(action);
			}
			CallFunc* actionFunc = CallFunc::create(gem,callfunc_selector(Gem::showSprite));

			gem->addAction(actionFunc);

			ActionInterval* actionDown = GemAction::getInstance().getGemMoveAction(RightDown);

			gem->addAction(actionDown);

			_gemMatrix[i][j] = gem;

			return;
		}
	}

	if(i+1<kMatrixWidth)
	{
		if(_gemMatrix[i+1][j+1]&&_gemMatrix[i+1][j+1]->canMove())
		{
			ActionInterval* actionLeftDown = GemAction::getInstance().getGemMoveAction(LeftDown);

			_gemMatrix[i+1][j+1]->addAction(actionLeftDown);

			_gemMatrix[i][j] = _gemMatrix[i+1][j+1];

			_gemMatrix[i+1][j+1] = NULL;

			fillEmptyNew(layer, i+1, j+1);

			return;
		}
		if(!_gemMatrix[i+1][j+1]&&_mapMatrix[i+1][j+1]!=NoneInEdge&&_mapMatrix[i+1][j+1]!=NoneInMiddle)//�Ҳ�Ϊ���ҿ������
		{
			//�������(i,j)
			fillEmptyNew(layer,i+1,j+1);

			if(_gemMatrix[i+1][j+1]&&_gemMatrix[i+1][j+1]->canMove())
			{
				ActionInterval* actionLeftDown = GemAction::getInstance().getGemMoveAction(LeftDown);

				_gemMatrix[i+1][j+1]->addAction(actionLeftDown);

				_gemMatrix[i][j] = _gemMatrix[i+1][j+1];

				_gemMatrix[i+1][j+1] = NULL;

				fillEmptyNew(layer, i+1, j+1);

				return;
			}
		}
		if(_mapMatrix[i+1][j+1]==NoneInMiddle)//����Ϊ�ϲ㣬�������
		{
			int* counterArray = _newGemCounterFromMiddle;

			Gem *gem = Gem::createRandomGem(layer,Point(kStartX + (i+1)* kElementSize + kElementSize/2, kStartY + (j+1) * kElementSize + kElementSize/2),false);

			counterArray[i]++;

			for(int k=1;k<counterArray[i];k++)
			{
				ActionInterval* action =GemAction::getInstance().getGemMoveAction(StandBy);

				gem->addAction(action);
			}
			CallFunc* actionFunc = CallFunc::create(gem,callfunc_selector(Gem::showSprite));

			gem->addAction(actionFunc);

			ActionInterval* actionDown = GemAction::getInstance().getGemMoveAction(LeftDown);

			gem->addAction(actionDown);

			_gemMatrix[i][j] = gem;

			return;
		}
	}

}
//ƴ�ӵ�ͼ��
SpriteBatchNode* MapMatrix::createMapFrame(Node* mapNode)
{
	SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile( "map_image.plist" );

	SpriteBatchNode* batchNode = SpriteBatchNode::create("map_image.png",1000);

	batchNode->setPosition(Vec2::ZERO);

	mapNode->addChild(batchNode,-1);

	int i,j;

	int tm[kMatrixWidth+2][kMatrixWidth+2]={0};

	for(i=0;i<kMatrixWidth;i++)
	{
		for(j=0;j<kMatrixWidth;j++)
		{
			if(0==_mapMatrix[i][j]||-3==_mapMatrix[i][j])
			{
				tm[i+1][j+1]=0;
			}
			else
			{
				tm[i+1][j+1]=1;
			}
		}
	}
	Sprite* backspr = NULL;

	for(i=1;i<kMatrixWidth+1;i++)
	{
		for(j=1;j<kMatrixWidth+1;j++)
		{
			if(tm[i][j])
			{
				if(i%2==0)
				{
					if(j%2==0)
					{
						backspr= Sprite::createWithSpriteFrameName("back_dark.png");
					}
					else
					{
						backspr= Sprite::createWithSpriteFrameName("back_light.png");
					}
				}
				else
				{
					if(j%2==0)
					{
						backspr= Sprite::createWithSpriteFrameName("back_light.png");
					}
					else
					{
						backspr= Sprite::createWithSpriteFrameName("back_dark.png");
					}
				}
				backspr->setScale(60.0/128);
				backspr->setPosition(Point(kStartX+(i-1+0.5)*kElementSize,kStartY+(j-1+0.5)*kElementSize));

				batchNode->addChild(backspr);
			}
		}
	}
	for(i=1;i<kMatrixWidth+1;i++)
	{
		for(j=1;j<kMatrixWidth+1;j++)
		{
			if(tm[i][j])
			{
				if(!tm[i-1][j]&&!tm[i][j-1])//��������
				{
					if(tm[i-1][j-1])
					{
						Sprite* spr = Sprite::createWithSpriteFrameName("back2x_1.png");

						spr->setPosition(Point(kStartX+(i-1)*kElementSize,kStartY+(j-1)*kElementSize));

						spr->setTag(13);

						spr->setScale(60.0/128);

						batchNode->addChild(spr);
					}
					else
					{
						Sprite* spr = Sprite::createWithSpriteFrameName("back1_4.png");

						spr->setPosition(Point(kStartX+(i-1)*kElementSize,kStartY+(j-1)*kElementSize));

						spr->setTag(4);

						spr->setScale(60.0/128);

						batchNode->addChild(spr);
					}
				}
				if (!tm[i-1][j]&&!tm[i][j+1])//��������
				{
					if(tm[i-1][j+1])
					{
						Sprite* spr = Sprite::createWithSpriteFrameName("back2x_2.png");

						spr->setPosition(Point(kStartX+(i-1)*kElementSize,kStartY+ j *kElementSize));

						spr->setTag(14);

						spr->setScale(60.0/128);

						batchNode->addChild(spr);
					}
					else
					{
						Sprite* spr = Sprite::createWithSpriteFrameName("back1_1.png");

						spr->setPosition(Point(kStartX+(i-1)*kElementSize,kStartY+ j *kElementSize));

						spr->setTag(1);

						spr->setScale(60.0/128);

						batchNode->addChild(spr);
					}

				}
				if (!tm[i+1][j]&&!tm[i][j-1])//��������
				{
					if(!tm[i+1][j-1])
					{
						Sprite* spr = Sprite::createWithSpriteFrameName("back1_3.png");

						spr->setPosition(Point(kStartX+ i *kElementSize,kStartY+ (j-1) *kElementSize));

						spr->setTag(3);

						spr->setScale(60.0/128);

						batchNode->addChild(spr);
					}

				}
				if (!tm[i+1][j]&&!tm[i][j+1])//��������
				{
					if(!tm[i+1][j+1])
					{
						Sprite* spr = Sprite::createWithSpriteFrameName("back1_2.png");

						spr->setPosition(Point(kStartX+ i *kElementSize,kStartY+ j *kElementSize));

						spr->setTag(2);

						spr->setScale(60.0/128);

						batchNode->addChild(spr);
					}

				}
			}
			else
			{
				if(tm[i-1][j]&&tm[i][j-1]&&tm[i-1][j-1])//��������
				{
					Sprite* spr = Sprite::createWithSpriteFrameName("back3_2.png");

					spr->setPosition(Point(kStartX+(i-1)*kElementSize,kStartY+(j-1)*kElementSize));

					spr->setTag(10);

					spr->setScale(60.0/128);

					batchNode->addChild(spr);
				}
				if (tm[i-1][j]&&tm[i][j+1]&&tm[i-1][j+1])//��������
				{
					Sprite* spr = Sprite::createWithSpriteFrameName("back3_3.png");

					spr->setPosition(Point(kStartX+(i-1)*kElementSize,kStartY+ j *kElementSize));

					spr->setTag(11);

					spr->setScale(60.0/128);

					batchNode->addChild(spr);
				}
				if (tm[i+1][j]&&tm[i][j-1]&&tm[i+1][j-1])//��������
				{
					Sprite* spr = Sprite::createWithSpriteFrameName("back3_1.png");

					spr->setPosition(Point(kStartX+ i *kElementSize,kStartY+ (j-1) *kElementSize));

					spr->setTag(9);

					spr->setScale(60.0/128);

					batchNode->addChild(spr);
				}
				if (tm[i+1][j]&&tm[i][j+1]&&tm[i+1][j+1])//��������
				{
					Sprite* spr = Sprite::createWithSpriteFrameName("back3_4.png");

					spr->setPosition(Point(kStartX+ i *kElementSize,kStartY+ j *kElementSize));

					spr->setTag(12);

					spr->setScale(60.0/128);

					batchNode->addChild(spr);
				}
			}
		}
	}

	int jIndex,iIndex,maxSize,k;

	for(i=1;i<=kMatrixWidth+1;i++)
	{
		for (j=1;j<=kMatrixWidth+1;j++)
		{
			jIndex = j;
			maxSize = 0;

			while(tm[i][jIndex]&&!tm[i-1][jIndex])
			{
				jIndex++;
				maxSize++;
			}
			if(maxSize>=2)
			{
				for(k=1;k<maxSize;k++)
				{
					Sprite* spr = Sprite::createWithSpriteFrameName("back2_2.png");

					spr->setPosition(Point(kStartX+ (i-1) *kElementSize,kStartY+ (j-1+k) *kElementSize));

					spr->setTag(6);

					spr->setScale(60.0/128);

					batchNode->addChild(spr);
				}

				j = jIndex;
				continue;
			}
		}
	}
	for(i=1;i<=kMatrixWidth+1;i++)
	{
		for (j=1;j<=kMatrixWidth+1;j++)
		{
			jIndex = j;
			maxSize =0;
			while(!tm[i][jIndex]&&tm[i-1][jIndex])
			{
				jIndex++;
				maxSize++;
			}
			if(maxSize>=2)
			{
				for(k=1;k<maxSize;k++)
				{
					Sprite* spr = Sprite::createWithSpriteFrameName("back2_4.png");

					spr->setPosition(Point(kStartX+ (i-1) *kElementSize,kStartY+ (j-1+k) *kElementSize));

					spr->setTag(8);

					spr->setScale(60.0/128);

					batchNode->addChild(spr);
				}
				j=jIndex;
				continue;
			}
		}
	}

	for(j=1;j<=kMatrixWidth+1;j++)
	{
		for (i=1;i<=kMatrixWidth+1;i++)
		{
			iIndex = i;
			maxSize = 0;

			while(tm[iIndex][j]&&!tm[iIndex][j-1])
			{
				iIndex++;
				maxSize++;
			}
			if(maxSize>=2)
			{
				for(k=1;k<maxSize;k++)
				{
					Sprite* spr = Sprite::createWithSpriteFrameName("back2_1.png");

					spr->setPosition(Point(kStartX+ (i-1+k) *kElementSize,kStartY+ (j-1) *kElementSize));

					spr->setTag(5);

					spr->setScale(60.0/128);

					batchNode->addChild(spr);
				}
				i=iIndex;
				continue;
			}
		}
	}
	for(j=1;j<=kMatrixWidth+1;j++)
	{
		for (i=1;i<=kMatrixWidth+1;i++)
		{
			iIndex = i;
			maxSize =0;
			while(!tm[iIndex][j]&&tm[iIndex][j-1])
			{
				iIndex++;
				maxSize++;
			}
			if(maxSize>=2)
			{
				for(k=1;k<maxSize;k++)
				{
					Sprite* spr = Sprite::createWithSpriteFrameName("back2_3.png");

					spr->setPosition(Point(kStartX+ (i-1+k) *kElementSize,kStartY+ (j-1) *kElementSize));

					spr->setTag(7);

					spr->setScale(60.0/128);

					batchNode->addChild(spr);
				}
				i=iIndex;
				continue;
			}
		}
	}
	return batchNode;
}

//���ݵݹ麯���õ��Ķ������У�����ص�������

void MapMatrix::clearCounter()
{
	for(int i=0;i<kMatrixWidth;i++)
	{
		_newGemCounter[i] = 0;

		_newGemCounterFromMiddle[i] = 0;
	}
}

void MapMatrix::markeMatrixChange()
{
	vector<ConnectionArea>::iterator it;

	int i,j;
	//����Ԫ�أ����ɶ�������

	//Ԥ��������������Ԫ�ر��
	for( it =connectionAreaVector.begin();it!=connectionAreaVector.end();it++)
	{
		if (it->direction == Up)
		{
			for (j = it->startPoint.y,i = it->startPoint.x; j < it->startPoint.y + it->count; j++)
			{
				if(_gemMatrix[i][j])
				{
					_gemMatrix[i][j]->setChange(Eliminate);
				}
			}
		}
		else
		{
			for (i = it->startPoint.x,j = it->startPoint.y; i < it->startPoint.x + it->count; i++)
			{
				if(_gemMatrix[i][j])
				{
					_gemMatrix[i][j]->setChange(Eliminate);
				}
			}
		}
	}
	//�����ΧӰ��Ԫ�أ����ɼ�¼Ӱ�����
	for( it =connectionAreaVector.begin();it!=connectionAreaVector.end();it++)
	{
		if (it->direction == Up)
		{
			for (j = it->startPoint.y,i = it->startPoint.x; j < it->startPoint.y + it->count; j++)
			{
				if(!_gemMatrix[i][j]->canMove())//�����Ԫ�ر��������߱�����������Ӱ����ΧԪ��
				{
					continue;
				}
				if(j==it->startPoint.y)
				{
					if(j-1>=0&&_gemMatrix[i][j-1])
					{
						_gemMatrix[i][j-1]->addChange();
					}
				}
				else if(j==it->startPoint.y+it->count-1)
				{
					if(j+1<kMatrixWidth&&_gemMatrix[i][j+1])
					{
						_gemMatrix[i][j+1]->addChange();
					}
				}

				if(i-1>=0&&_gemMatrix[i-1][j])
				{
					_gemMatrix[i-1][j]->addChange();
				}
				if(i+1<kMatrixWidth&&_gemMatrix[i+1][j])
				{
					_gemMatrix[i+1][j]->addChange();
				}
			}
		}
		else
		{
			for (i = it->startPoint.x,j = it->startPoint.y; i < it->startPoint.x + it->count; i++)
			{
				if(!_gemMatrix[i][j]->canMove())//�����Ԫ�ر��������߱�����������Ӱ����ΧԪ��
				{
					continue;
				}
				if(i ==it->startPoint.x)
				{
					if(i-1>=0&&_gemMatrix[i-1][j])
					{
						_gemMatrix[i-1][j]->addChange();
					}
				}
				else if(i==it->startPoint.x+it->count-1)
				{
					if(i+1<kMatrixWidth&&_gemMatrix[i+1][j])
					{
						_gemMatrix[i+1][j]->addChange();
					}
				}

				if(j-1>=0&&_gemMatrix[i][j-1])
				{
					_gemMatrix[i][j-1]->addChange();
				}
				if(j+1<kMatrixWidth&&_gemMatrix[i][j+1])
				{
					_gemMatrix[i][j+1]->addChange();
				}
			}
		}
	}

}

void Disjoinset::makeset()
{
	for(int i=0;i<kMatrixWidth*kMatrixWidth;i++)
	{
		pre[i] = i;
		rank[i]= 0;
	}
}

int Disjoinset::find(int x)
{
	if(x!=pre[x])
	{
		pre[x]=find(pre[x]);
	}
	return pre[x];
}

void Disjoinset::unionone(int a, int b)
{
	int t1 = find(a);

	int t2 = find(b);

	if(rank[t1]>rank[t2])
	{
		pre[t2] = t1;
	}
	else
	{
		pre[t1] = t2;
	}

	if(rank[t1]==rank[t2])
	{
		rank[t2]++;
	}
}

void MapMatrix::dealSick()
{
	int i,j,k,l;

	int sickGem[kMatrixWidth*kMatrixWidth] = {0};

	bool hasSick = false;

	for(k=0;k<connectionAreaVector.size();k++)
	{
		ConnectionArea ca = connectionAreaVector[k];

		if (ca.direction == Up)
		{
			for (j = ca.startPoint.y,i = ca.startPoint.x; j < ca.startPoint.y + ca.count; j++)
			{
				if(_gemMatrix[i][j]->getSick())
				{
					sickGem[k]=1;
					hasSick = true;
				}
			}
		}
		else
		{
			for (i = ca.startPoint.x,j = ca.startPoint.y; i < ca.startPoint.x + ca.count; i++)
			{
				if(_gemMatrix[i][j]->getSick())
				{
					sickGem[k]=1;
					hasSick = true;
				}
			}
		}
	}

	if(!hasSick) return;

	MyPoint crossPoint;

	Disjoinset ds = Disjoinset::getInstance();

	ds.makeset();

	GemType type;

	for(i=0;i<connectionAreaVector.size();i++)
	{
		MyPoint startPoint1 = connectionAreaVector[i].startPoint;

		type = _gemMatrix[startPoint1.x][startPoint1.y]->getGemType();

		for(j=i+1;j<connectionAreaVector.size();j++)
		{
			MyPoint startPoint2 = connectionAreaVector[j].startPoint;

			if(_gemMatrix[startPoint2.x][startPoint2.y]->getGemType()!=type)
			{
				continue;
			}

			if(connectionAreaVector[i].isCross(connectionAreaVector[j], crossPoint))
			{
				ds.unionone(i, j);
			}
		}
	}

	for(k=0;k<connectionAreaVector.size();k++)
	{
		if(sickGem[k]==1)
		{
			int index = ds.pre[k];

			for(l=0;l<connectionAreaVector.size();l++)
			{
				if(ds.pre[l]==index)
				{
					ConnectionArea ca = connectionAreaVector[l];

					MyPoint startPoint = ca.startPoint;

					if (ca.direction == Up)
					{
						for (j = startPoint.y,i = startPoint.x; j < startPoint.y + ca.count; j++)
						{
							_gemMatrix[i][j]->setSick(true);
						}
					}
					else
					{
						for (i = startPoint.x,j = startPoint.y; i < startPoint.x + ca.count; i++)
						{
							_gemMatrix[i][j]->setSick(true);
						}
					}
				}
			}
		}
	}
}


void MapMatrix::findCenterPoint(MyPoint &selectPoint, MyPoint &nextPoint)
{
	vector<ConnectionArea>::iterator it;

	int i,j;

	//�������������������ҵ�����λ�ã�����Ϊ�ƶ��㣬Ҳ����Ϊͷ��㣬�����ཻ��־Ϊ�գ�
	for(it=connectionAreaVector.begin();it!=connectionAreaVector.end();it++)
	{
		it->isInCross=false;

		if(it->isContain(selectPoint)||it->isContain(nextPoint))
		{
			if(it->isContain(selectPoint))
			{
				it->centerPoint.setPosition(selectPoint.x,selectPoint.y);
			}
			if(it->isContain(nextPoint))
			{
				it->centerPoint.setPosition(nextPoint.x,nextPoint.y);
			}
		}
		else//�����������ƶ���,����ұ��������ƶ��ĵ㣬����Ϊ���ĵ�
		{
			if (it->direction == Up)
			{
				for (j = it->startPoint.y,i = it->startPoint.x; j < it->startPoint.y + it->count; j++)
				{
					if(_gemMatrix[i][j]->getActionVectorSize()>0)
					{
						it->centerPoint.setPosition(i, j);
						break;
					}
					if(j==it->startPoint.y+it->count-1)
					{
						it->centerPoint.setPosition(it->startPoint.x, it->startPoint.y);
					}
				}
			}
			else
			{
				for (i = it->startPoint.x,j = it->startPoint.y; i < it->startPoint.x + it->count; i++)
				{
					if(_gemMatrix[i][j]->getActionVectorSize()>0)
					{
						it->centerPoint.setPosition(i, j);
						break;
					}
					if(i==it->startPoint.x+it->count-1)
					{
						it->centerPoint.setPosition(it->startPoint.x, it->startPoint.y);
					}
				}
			}
		}

	}
}

void MapMatrix::tigerEatAnimal(MyPoint& _selectPoint,MyPoint& _nextPoint)
{
	for (int i=0;i<kMatrixWidth; i++)
	{
		for(int j=0;j<kMatrixWidth;j++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemType()==tiger)
			{
// 				Tiger* tiger = (Tiger*)_gemMatrix[i][j];
// 
// 				MyPoint point(i,j);
// 
// 				tiger->eatAnimal(point,_selectPoint,_nextPoint );
			}
		}
	}
}

void MapMatrix::matchAllGem(vector<AnimationWraper>* animationWraperVector)
{
	AnimationWraper aw(NULL,e_aid_normal_explode,e_priority_normal_explode);

	animationWraperVector->push_back(aw);

	vector<ConnectionArea>::iterator it;

	int i,j;

	//ִ����������Ԫ�ص�explode���������ڲ��������ΧԪ�ص�Ӱ�캯��affect
	for( it =connectionAreaVector.begin();it!=connectionAreaVector.end();it++)
	{
		j = it->startPoint.y;
		i = it->startPoint.x;

		if (it->direction == Up)
		{
			for ( ;j < it->startPoint.y + it->count; j++)
			{
				MyPoint point(i,j);

				if (point.equal(it->centerPoint) && (it->isInCross || it->count > 3))
				{
					continue;
				}
				
				_gemMatrix[i][j]->explode(point,it->centerPoint,it->count-2,
					it->startPoint.y+it->count-j);
			}

		}
		else
		{
			for (; i < it->startPoint.x + it->count; i++)
			{
				MyPoint point(i,j);

				if (point.equal(it->centerPoint)&& (it->isInCross || it->count > 3))
				{
					continue;
				}

				if(it->startPoint.x>3)
				{
					_gemMatrix[i][j]->explode(point,it->centerPoint,it->count-2,
						i-it->startPoint.x);
				}
				else
				{
					_gemMatrix[i][j]->explode(point,it->centerPoint,it->count-2,
						it->startPoint.x+it->count-i);
				}
			}
		}
	}
}

void MapMatrix::renewGemCount()
{
	for(int i=0;i<kMatrixWidth;i++)
	{
		for(int j=0;j<kMatrixWidth;j++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getChange()!=Eliminate)
			{
				_gemMatrix[i][j]->addScore(0, false);
			}
		}
	}
}
void MapMatrix::growOnFertile()
{
	int i,j;
	//������������
	for(i=0;i<kMatrixWidth;i++)
	{
		for(j=0;j<kMatrixWidth;j++)
		{
			if(_gemMatrix[i][j])
			{
				_gemMatrix[i][j]->growOnFertile(i, j);
			}
		}
	}
}

void MapMatrix::afterMatch()
{
	int i,j;

	for(i=0;i<kMatrixWidth;i++)
	{
		for(j=0;j<kMatrixWidth;j++)
		{
			if(_gemMatrix[i][j])
			{
				_gemMatrix[i][j]->afterMatch(i, j);
			}
		}
	}
}

void MapMatrix::beforeMatchGem()
{
	int i,j;
	//Ԥ����,��, ����
	for(i=0;i<kMatrixWidth;i++)
	{
		for(j=0;j<kMatrixWidth;j++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==SpecialGem)
			{
				_gemMatrix[i][j]->beforeMatch(i, j);
			}
		}
	}
	//��������Ԫ�صĲ�̬/�ǲ�̬
	for(i=0;i<kMatrixWidth;i++)
	{
		for(j=0;j<kMatrixWidth;j++)
		{
			if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem/*&&!_mapInfo->getHeroMode()*/)
			{
				_gemMatrix[i][j]->beforeMatch(i, j);
			}
		}
	}
}

void MapMatrix::dealFallDown(Layer* layer)
{
	int i,j,k;

	for(k=0;k<kMatrixWidth;k++)
	{
		for(i=0;i<kMatrixWidth;i++)
		{
			if(fallDownFlagArray[i]==k)
			{
				for(j=0;j<kMatrixWidth;j++)
				{
					if(_mapMatrix[i][j]!=NoneInEdge&&_mapMatrix[i][j]!=NoneInMiddle&&_gemMatrix[i][j]==NULL)
					{
						fillEmptyNew(layer, i, j);
					}
				}
			}
		}
	}
}

void MapMatrix::beforeMatchGem(vector<AnimationWraper>* animationWraperVector,MyPoint& selectPoint,MyPoint& nextPoint)
{
	findCenterPoint(selectPoint, nextPoint);

/*	dealSick();*/

	int i,j,k,l,sum=0;

	int flagArray[kMatrixWidth*kMatrixWidth]={0};
	int sumFlagArray[kMatrixWidth*kMatrixWidth]={0};

	Disjoinset ds = Disjoinset::getInstance();

	ds.makeset();

	MyPoint crossPoint;

	vector<ConnectionArea>::iterator it;

	//boss���û���������1������2��ÿ������������Ӧ����
// 	if(_mapInfo->getIncreaseStep()>0)
// 	{
// 		for( it =connectionAreaVector.begin();it!=connectionAreaVector.end();it++)
// 		{
// 			j = it->startPoint.y;
// 			i = it->startPoint.x;
// 
// 			if(_mapInfo->getPassLevelCCNode(_gemMatrix[i][j]->getGemType())!=NULL)
// 			{
// 				_gemMatrix[i][j]->addScore(_mapInfo->getIncreaseStep());
// 			}
// 		}
// 	}

	for(i = 0;i<connectionAreaVector.size();i++)
	{
		MyPoint startPoint1 = connectionAreaVector[i].startPoint;

		GemType ftype = _gemMatrix[startPoint1.x][startPoint1.y]->getGemType();

		for (j=0; j<connectionAreaVector.size(); j++)
		{
			MyPoint startPoint2 = connectionAreaVector[j].startPoint;

			if(_gemMatrix[startPoint2.x][startPoint2.y]->getGemType()!=ftype)
			{
				continue;
			}
			if(connectionAreaVector[i].isCross(connectionAreaVector[j], crossPoint))
			{
				ds.unionone(i, j);

				connectionAreaVector[i].centerPoint.setPosition(crossPoint.x, crossPoint.y);

				connectionAreaVector[j].centerPoint.setPosition(crossPoint.x, crossPoint.y);
			}
		}
	}

	//���ɾۺ����
	for(i=0;i<connectionAreaVector.size();i++)
	{
		MyPoint startPoint = connectionAreaVector[i].startPoint;

		GemType type = _gemMatrix[startPoint.x][startPoint.y]->getGemType();

		if((type==butterfly||type==chicken)&&flagArray[i]==0)
		{
			int index = ds.pre[i];

			MyPoint centerPoint = connectionAreaVector[index].centerPoint;

			for(j=0;j<connectionAreaVector.size();j++)
			{
				if(ds.pre[j]==index)
				{
					connectionAreaVector[j].centerPoint = centerPoint;
					flagArray[j]=1;
				}
			}
		}
	}

	findParticlePoint(selectPoint, nextPoint);//��ʼ������Ч���ڵ�

	//��������ʮ��
	MyPoint particlePoint;
	int  unionSize = 0;
	for(i=0;i<connectionAreaVector.size();i++)
	{
		//�Ѿ��������
		if(1==flagArray[i])
		{
			continue;
		}
		//ȡ���ڵ�
		int rootIndex = ds.pre[i];

		unionSize = 0;

		//��ͬһ���������Ƿ��д���5�����ĳ���
		for(j=0;j<connectionAreaVector.size();j++)
		{
			if(0==flagArray[j]&&ds.pre[j]==rootIndex)
			{
				//���ڵ���5�������������ɶ���
				if(connectionAreaVector[j].count>=5)
				{
					particlePoint = connectionAreaVector[j].particlePoint;

					GemType ftype = _gemMatrix[particlePoint.x][particlePoint.y]->getGemType();
					//��ǵ�ͼ�������뱾����ͬ���Ԫ��ΪEliminate
// 					for(k=0;k<kMatrixWidth;k++)
// 					{
// 						for(l=0;l<kMatrixWidth;l++)
// 						{
// 							if(_gemMatrix[particlePoint.x][particlePoint.y]->equal(_gemMatrix[k][l]))
// 							{
// 								_gemMatrix[k][l]->setChange(Eliminate);
// 							}
// 						}
// 					}
					AnimationWraper aw(_gemMatrix[particlePoint.x][particlePoint.y],e_aid_particle_five,e_priority_particle_five);

					aw.state = connectionAreaVector[j].direction;

					aw.myDist1 = particlePoint;

					aw.index1 = j;

					animationWraperVector->push_back(aw);
					//����������ͼ�������뱾Ԫ��ͬ�����ͨ�鶼���Ϊ�Ѵ���
					for(k=0;k<connectionAreaVector.size();k++)
					{
						MyPoint p = connectionAreaVector[k].startPoint;

						if(_gemMatrix[p.x][p.y]->getGemType()==ftype)
						{
							flagArray[k]=1;

							sumFlagArray[k]=1;//���������һ��Ԫ�ر�����������������������������Ч��
						}
					}
					//��������������,��ͼ��������ͬԪ�ؼ�����������ɼ�����ʾ
					for(k=0;k<kMatrixWidth;k++)
					{
						for(l=0;l<kMatrixWidth;l++)
						{
							if(_gemMatrix[k][l]&&_gemMatrix[k][l]->getGemType()==ftype&&_gemMatrix[k][l]->getGemKind()==NormalGem&&!_gemMatrix[k][l]->getSick()&&_gemMatrix[k][l]->canMove())
							{
								sum+=_gemMatrix[k][l]->getScore();
							}
						}
					}
					if(sum>0)
					{
						if(_gemMatrix[particlePoint.x][particlePoint.y]/*&&_mapInfo->getPassLevelCCNode(_gemMatrix[particlePoint.x][particlePoint.y]->getGemType())!=NULL*/)
						{
							_gemMatrix[particlePoint.x][particlePoint.y]->setScoreSum(sum);
						}

						sum=0;
					}
				}
			}
		}

		//��ͬһ�����е�ʮ������
		for(j=0;j<connectionAreaVector.size();j++)
		{
			if(0==flagArray[j]&&ds.pre[j]==rootIndex)
			{
				unionSize++;
			}
		}

		if(unionSize>=2)
		{
			particlePoint = connectionAreaVector[rootIndex].centerPoint;

			AnimationWraper aw(_gemMatrix[particlePoint.x][particlePoint.y], e_aid_particle_cross,e_priority_particle_cross);

			aw.myDist1 = particlePoint;

			aw.index1 = rootIndex;

			MyPoint myCrossPoint;

			for(k=0;k<connectionAreaVector.size();k++)
			{
				if(ds.pre[k]==rootIndex)
				{
					flagArray[k]=1;

					if(k!=rootIndex&&connectionAreaVector[k].isCross(connectionAreaVector[rootIndex], myCrossPoint)&&myCrossPoint.equal(particlePoint))
					{
						aw.index2 = k;
					}
				}
			}

			animationWraperVector->push_back(aw);
		}
	}
	//��4��
	for(j=0;j<connectionAreaVector.size();j++)
	{
		if(0==flagArray[j])
		{
			if(connectionAreaVector[j].count==4)
			{
				particlePoint = connectionAreaVector[j].particlePoint;

				AnimationWraper aw(_gemMatrix[particlePoint.x][particlePoint.y],e_aid_particle_four,e_priority_particle_four);

				aw.state = connectionAreaVector[j].direction;

				aw.myDist1 = particlePoint;

				aw.index1 = j;

				animationWraperVector->push_back(aw);

				flagArray[j]=1;
			}
		}
	}
	//�����ռ�Ԫ���ܺͶ���,����5�����ۺϵĲ���
	for(i=0;i<connectionAreaVector.size();i++)
	{
		MyPoint startPoint = connectionAreaVector[i].startPoint;

		GemKind kind = _gemMatrix[startPoint.x][startPoint.y]->getGemKind();

		if(sumFlagArray[i]==0&&kind==NormalGem&&!_gemMatrix[startPoint.x][startPoint.y]->getSick())
		{
			int index = ds.pre[i];

			for(j=0;j<connectionAreaVector.size();j++)
			{
				if(ds.pre[j]==index)
				{
					ConnectionArea ca = connectionAreaVector[j];

					if (ca.direction == Up)
					{
						for (l = ca.startPoint.y,k = ca.startPoint.x; l < ca.startPoint.y + ca.count; l++)
						{
							if(_gemMatrix[k][l]->canMove())
							{
								sum+=_gemMatrix[k][l]->getScore();
							}
						}
					}
					else
					{
						for (k = ca.startPoint.x,l = ca.startPoint.y; k < ca.startPoint.x + ca.count; k++)
						{
							if(_gemMatrix[k][l]->canMove())
							{
								sum+=_gemMatrix[k][l]->getScore();
							}
						}
					}

					sumFlagArray[j]=1;
				}
			}
			if(sum>0)
			{
				MyPoint centerPoint = connectionAreaVector[index].centerPoint;

				if(_gemMatrix[centerPoint.x][centerPoint.y]/*&&_mapInfo->getPassLevelCCNode(_gemMatrix[centerPoint.x][centerPoint.y]->getGemType())!=NULL*/)
				{
					_gemMatrix[centerPoint.x][centerPoint.y]->setScoreSum(sum);
				}

				sum=0;
			}
		}
	}
}

void MapMatrix::findParticlePoint(MyPoint &selectPoint, MyPoint &nextPoint)
{
	vector<ConnectionArea>::iterator it;

	int i,j;

	//�������������������ҵ�����λ�ã�����Ϊ�ƶ��㣬Ҳ����Ϊͷ��㣬�����ཻ��־Ϊ�գ�
	for(it=connectionAreaVector.begin();it!=connectionAreaVector.end();it++)
	{
/*		it->isInCross=false;*/

		if(it->isContain(selectPoint)||it->isContain(nextPoint))
		{
			if(it->isContain(selectPoint))
			{
				it->particlePoint.setPosition(selectPoint.x,selectPoint.y);
			}
			if(it->isContain(nextPoint))
			{
				it->particlePoint.setPosition(nextPoint.x,nextPoint.y);
			}
		}
		else//�����������ƶ���,����ұ��������ƶ��ĵ㣬����Ϊ���ĵ�
		{
			if (it->direction == Up)
			{
				for (j = it->startPoint.y,i = it->startPoint.x; j < it->startPoint.y + it->count; j++)
				{
					if(it->count>=5)
					{
						it->particlePoint.setPosition(i,it->startPoint.y+2);
					}
					else if(it->count<5)
					{
						it->particlePoint.setPosition(i, it->startPoint.y+1);
					}
				}
			}
			else
			{
				for (i = it->startPoint.x,j = it->startPoint.y; i < it->startPoint.x + it->count; i++)
				{
					if(it->count>=5)
					{
						it->particlePoint.setPosition(it->startPoint.x+2,j);
					}
					else if(it->count<5)
					{
						it->particlePoint.setPosition(it->startPoint.x+1, j);
					}
				}
			}
		}

	}
}