#ifndef __DRAGONEGGTRIPLE_DATATOOLS_H__
#define __DRAGONEGGTRIPLE_DATATOOLS_H__

#include "cocos2d.h"
USING_NS_CC ;

#define kElementSize   60.0f//128.0f //160.0f
#define kMatrixWidth 8
#define kStartX 0
#define kStartY 0
#define kMapZOrder -1
#define kGemZOrder 0
#define kParticleZOrder 10
#define kEdgeBlankDevidedSize 10//����߿�ռ�ܿ�ȷ���
#define kGameLayerStartMultiple 5
#define kGameLayerFlyTime 0.8
#define kRenewGemWaitTime 0.3
#define kBrightScaleTime 0.12
#define kExplodeMiddleTime 0.067//ͬһ�����Ƴ�ǰ��ʱ����
#define kRenewGemTime 0.5//���ų���ʱ��

#define kChangeWaitTime 0.3//�������󷵻�֮ǰ�ȴ���ʱ��
#define kChangeMoveTime 0.21//����ʱ�ƶ���ʱ��
#define kChangeScaleFirstStepTime 0.08//�����仯��һ��
#define kchangeScaleSecondStepTime 0.06//�����仯�ڶ���
#define kFallMoveTime 0.07//����һ������ʱ��
#define kFallMoveCorner 0.1//б��
#define KCollectMoveSpeed 1500//�ռ�Ԫ���ƶ���Ŀ�ĵ���ʱ
#define kCollectMoveJumpHeight 200
#define kHighLightFrequnce 4//������ʾƵ��
#define kConvergeMoveTime 0.3//�ۺ�Ԫ���ƶ�ʱ��
#define kConvergeChangeTime 0.8//�ۺ�Ԫ��״̬�仯ʱ��
#define kRemoveScaleLargeTime 0.1//�����Ŵ�ʱ��
#define kRemoveScaleSmallTime 0.1//������Сʱ��
#define kRemoveCollectDelayTime 0.4
#define kRemoveeNoCollectDelayTime 0.4
#define kParticleScaleParm 1.2
#define kHenhouseBirdflyTime 0.8
#define kTigerJumpTime 0.8

//���ֶ������ʱ��
#define kPriorityInterval_GrassguyDissapear 0.7
#define kPriorityInterval_SickChange 0.8
#define kTigerEatDelayTime 0.7
#define kFiveMatchDelayTime 0.7
#define kCrossMatchDelayTime 0.7
#define kExplodeSumShowTime 0.5
#define kSparkleFlyTime 0.8
#define kParticleBezierScale 0.6
#define kGenerateGrassTimeDelayTime 0.2
#define kFrozenAndRestrainOffTime 0.4
#define kWinnerModeStarFlyDelayTime 1

#define kPriority5_delaytime 0.9

#define kGameConfigNull "kong"

#define kAdsIntervalTime 120

//����Ԫ�ض��еĶ���
//���������ƶ���ѡ��

//��ͨԪ�ض���
//�����ƶ��������ƶ������俪ʼ/��������ʾ,ѡ��
//
//գ�ۣ����У��䲡̬����̬��ã���Ӱ��

#define kZhayan "zhayan"//���
#define kIdleAnimation "idle"//����
#define kBecomeSick "becomesick"  //����
#define kBecomeWell "becomewell" //����
#define kFrozenOff "frozenoff" //ȥ������
#define kRestrainOff "restrainoff" //ȥ������
#define kFrozenon "frozenon"//���ӱ���
#define kResttainOn "restrainon"//��������

//�ۺ��ද��(���ܣ�������
#define kButterfly_Converge1 "converge1"//Ӽ���Ѷ���
#define kButterfly_Converge2 "converge2"//�������ֶ���
#define kFrog_Converge1 "converge1"
#define kFrog_Converge2 "converge2"
#define kFrog_Converge3 "converge3"
#define kFrog_Converge1_Appear1 "appear1"//û�в�
#define kFrog_Converge2_Appear2 "appear2"//�ѿ�

//grassguy
#define kGrassguy_Disappear "disappear"//��ʧ
#define kGrassguy_Disappear_Waittime 1.5//��ʧʱ��

//������
#define kHenhouse_EggAppear "eggappear"
#define kHenhouse_EggBroken "eggbroken"
#define kHenhouse_ParrotAppear "parrotappear"

//���������
#define kFishinbubble_match1 "start"//����һ��
#define kFishinbubble_match2 "middle"//��������
#define kFishinbubble_match3 "last"//�������Σ��ռ�״̬
#define kFishinbubble_idle1  "idle1"
#define kFishinbubble_idle2  "idle2"
#define kFishinbubble_idle3  "idle3"

//�ϻ�
#define KTiger_Appear "appear"//�ϻ�����
#define kTiger_Dizzy1 "dizzy"//�ϻ�ͷ��һ����
#define kTiger_JumpToAnimal "tigerjump" //�ϻ���С����
#define kTiger_EatAnimal  "tigereat" //�ϻ��Զ���

//����������ʾ
#define kMatch_great "Great. bi"
#define kMatch_perfect "Perfect. bi"
#define kMatch_awesome "Awesome. bi"
#define kMatch_great_level 13
#define kMatch_perfect_level 16
#define kMatch_awesome_level 20

//Ч��(����Ч������)
#define kAfterEliminateParticle1 "remove. bi"//��������֮��Ч��1
#define kAfterEliminateParticle2 "disappear. bi"//��������֮��Ч��2
#define kFourOrMoreMatchParticle "sixiaochu. bi" //�Ļ�������Ч��
#define kCrossMatchParticle "shizixiaochu. bi" //ʮ������Ч��
#define kWinnerModeLine "line. bi"//Ӣ��ģʽ b
#define kParticleStart "appear" //Ч��ִ�У�Ϊ�˷���ص���

#define kEmptyFunction " "

//nitofication
#define kMSG_GemFallDown  "msg_Gem_falldown"
#define kMSG_UpdateCollecte "msg_update_collect"
#define kMSG_ShowCollecte  "msg_show_collect"
#define kMSG_UpdateMoves "msg_update_moves"
#define kMSG_Add5Moves  "add_5_moves"
#define kMSG_UpdateStars "msg_updata_stars"
#define kMSG_MatchDown "msg_match_down"  // ƥ�����
#define kMSG_RenewAll "msg_renewall" //renew����ʱ��Ϊ kRenewGemTime
#define kMSG_WinnerModeStart "msg_winner_mode_start"//Ӣ��ģʽ����
#define kMSG_PropUseStart "msg_props_start"//ʹ�õ���
#define kMSG_PropUseCancel "msg_props_cancel" //����ȡ��
#define kMSG_PropUseOver "msg_props_over"//����ʹ�����
#define kMSG_PropUseNoCandidate "msg_props_nocandidate" //����û������
#define kMSG_GameLayer_BeforeOutStage "msg_gamelayer_beforeout" //֪ͨ��Ϸ����������0ʧ�ܣ�����1�ɹ�
#define kMSG_GameLayer_AfterOutStage  "msg_gamelayer_afterout"//��Ϸ������layer�ɳ�֮��֪ͨǰ̨�л�����
#define kMSG_GameLayer_LayerInitComplete "msg_gamelayer_complete"
#define kMSG_ClassGuid_PropSelectOver "msg_classguid_propselectover"
#define kMSG_ClassGuid_MoveGem      "msg_classguid_moveGem"
#define kMSG_BuyFiveSteps             "msg_buyfivesteps"
#define kMSG_BuyDimonds               "msg_buydimonds"
#define kMSG_CheckBtn               "msg_checkbtn"

#define kMSG_AvatarMenu               "msg_avatar_menu"
#define kMSG_FacebookLoginOk           "msg_fb_login_ok"
#define kMSG_RemoveGift     "msg_remove_gift"

//��ɫ
#define kColor1  Color3B(146,0,146)//��
#define kColor2  Color3B(255,252,0)//���
#define kColor3  Color3B(0,255,0)//����
#define kColor4  Color3B(0,146,255)//����
#define kColor5  Color3B(255,145,0)//��ɫ
#define kColor6  Color3B(0,255,246)//��ɫ
#define kColor7  Color3B(156,156,0)//��ɫ
//google analytic category
#define kGA_category1 "�ؿ�����"
#define kGA_category2 "�ؿ�����2"
#define kGA_Login  "��½����"
#define kGA_Facebook  "FACEBOOK����"
#define kGA_buyLevelData "����ʱ�ؿ�"

//��Ч����
#define kMusic_Backgroud_Switch  "backgroud_switch"
#define kMusic_Effect_Switch  "effect_switch"

//��Ч�ļ�

#define kIsReceiptNotification  "is_receipt_notification"
#define kResetData              "reset_data"

#define kExitLayerTag  246
#define kIsHaveDialogLayer "is_have_dialog_layer"
#define kUpdateFriendsNum  "update_friends_num"
#define kUpdateUIdate      "update_ui_date"
#define kAddProfileLayer   "add_profile_layer"
#define kAddInviteLayer   "add_invite_layer"
#define kAddFriut4GiveHeart "add_Gem_4_give_heart"
#define kTodayNumInYear    "today_num_in_year"
#define kInitGiftIsVisible  "init_gift_is_visible"
#define kDailyMissionMessage "daily_mission_message"
#define kUpdateGoodsType  "update_goods_type"

#define kUpdateFbMsgdate      "update_fbmsg_date"

#define kIsFirstPurcharAchieve "is_first_purchar_achieve"
#define kIsCustomBuyAnyAcount  "is_custom_buy_any_acount"


#define kMsgUpdataInviteTable "msg_updata_invite_table"
#define kMsgSendHeartProfile "msg_send_heart_profile"
#define kMsgSendHeartLeaderBoard "msg_send_heart_leaderboard"
#define kMsgSendHeartMessage "msg_send_heart_message"
#define kMsgIsAddRate2UI     "msg_is_add_rate_2_ui"

#define kBuyDiamondNoTouch "buy_diamond_no_touch"

#define HEARTTIME 1800

#define WORLD_MAP_UI_LAYER_TAG 123

class Gem;

struct MyPoint
{
	int x,y;
	MyPoint(){}
	MyPoint(int i,int j){x=i;y=j;}
	void setPosition(int i,int j){x=i;y=j;}
	void swap(MyPoint& p){int tempX = p.x; int tempY = p.y; p.setPosition(x, y); setPosition(tempX, tempY);}
	bool operator ==(const MyPoint& p ){if(p.x==x&&p.y==y) return true; else return false;}
	void reset(){x=-1;y=-1;}
	bool equal(MyPoint &point){if(x==point.x&&y==point.y) return true; else return false;}
	bool isOrigin(){return (x==0)&&(y==0);}
};

typedef enum
{
	e_priority_normal_becomesick=1,
	e_priority_normal_becomewell=1,
	e_priority_henhouse_changestate=2,
	e_priority_henhouse_birdfly=3,
	e_priority_normal_fertileaddscore=4,
	e_priority_grassguy_disappear=5,
	e_priority_grassguy_generategrass=5,
	e_priority_tiger_eatanimal=5,
	e_priority_normal_frozenoff=6,
	e_priority_normal_restrainoff=6,
	e_priority_fish_changestate=6,
	e_priority_tiger_changestate=6,
	e_priority_winnermode_starfly=6,
	e_priority_winnermode_start=7,
	e_priority_colorfullegg_disappear=7,
	e_priority_fish_remove=7,
	e_priority_colorfullegg_fly=8,
	e_priority_particle_five=9,
	e_priority_particle_cross=10,
	e_priority_particle_four=10,
	e_priority_converge_move=11,
	e_priority_converge_changestate=12,
	e_priority_converge_remove=13,
	e_priority_normal_explode=14,
	e_priority_normal_explode_sum=15,
	e_priority_normal_affectaddscore=16,
	e_priority_beforematch=17,
}AnimationPriority;

typedef enum
{
	e_aid_grassguy_disappear,//grassguy��ʧ
	e_aid_grassguy_generategrass,//����5��ݵ�
	e_aid_normal_becomesick,//����Ԫ������
	e_aid_normal_becomewell,//����Ԫ�ز���
	e_aid_normal_explode,//����Ԫ����ʧ
	e_aid_normal_addscore,//��ͨԪ�����ӻ���ֵ
	e_aid_normal_frozenoff,//����ȥ��
	e_aid_normal_restrainoff,//����ȥ��
	e_aid_colorfullegg_disappear,//�ʵ���ʧ
	e_aid_henhouse_threeeggs,//����������
	e_aid_henhouse_eggbroken,//����
	e_aid_henhouse_birdappear,//�����
	e_aid_henhouse_birdfly,//�ɳ�����
	e_aid_fish_match,//�㱻Ӱ��
	e_aid_fish_remove,//���Ƴ�
	e_aid_converge_move, //�ۺ�Ԫ���ƶ�
	e_aid_converge_changestate,//�ۺ�Ԫ�ظı�״̬
	e_aid_converge_remove,//�ۺ�Ԫ���Ƴ�
	e_aid_tiger_eatanimal,//�ϻ���С����
	e_aid_tiger_changestate,//�ϻ��ı�״̬
	e_aid_particle_five,
	e_aid_particle_four,
	e_aid_particle_cross,
	e_aid_normal_explode_sum,
	e_aid_winnermode_starfly,
	e_aid_winnermode_start,
	e_aid_beforematch,
}AnimationID;

struct AnimationWraper
{
	AnimationPriority priority;
	AnimationID animationID;
	Gem* node;
	Point dist1;
	Point dist2;
	Point dist3;
	int score;
	int state;
	int index1;
	int index2;
	bool isSingleAnimation;
	MyPoint myDist1;
	MyPoint myDist2;
	MyPoint myDist3;
	Gem*  Gem1;
	Gem*  Gem2;
	Gem*  Gem3;

	Gem*  GemToRemove1;
	Gem*  GemToRemove2;
	Gem*  GemToRemove3;

	int flagArray[6];
	int flagArrayLength;

	AnimationWraper(Gem* f,AnimationID aid,AnimationPriority p)
		:node(f)
		,animationID(aid)
		,priority(p)
		,score(0)
		,isSingleAnimation(false)
	{
		for(int i=0;i<6;i++)
		{
			flagArray[i]=0;
		}
	}
	void setDistination( Point& p1, Point& p2, Point& p3)
	{
		dist1 = p1;
		dist2 = p2;
		dist3 = p3;
	}
};

typedef enum
{
	empty = 0,
	red=1,
	blue=2,
	green=3,
	white=4,
	yellow=5,
	purple=6,
	orange=7,
	snake,//��չ
	wolf,//��չ
	tiger=10,//10
	fishinbubble=11,//���������
	colorfullegg=12,//�ʵ�
	butterfly=13,//�ۺ�
	henhouse=14,//����
	grassguy=15,//�ݵ�������
	chicken=16,//����
	maxgem=17,
}GemType;

typedef enum
{
	NormalGem=0,
	SpecialGem,
}GemKind;

typedef enum
{
	Up=0,
	Left,
	Right,
	Down,
	LeftDown,
	RightDown,
	StandBy
}Direction;

typedef enum
{
	DeepEliminate=-2,
	Eliminate=-1,
	NoChange=0,
	Changed
} ChangeState;

typedef enum
{
	InFertile=-4,
	NoneInMiddle=-3,
	Fertile=-2,
	Random=-1,
	NoneInEdge=0
}MapType;

typedef enum
{
	pupa=0,//Ӽ
	bug//��
} ButterflyType;

typedef enum
{
	littlefrog=0,//С����
	frogwithtail,//���ȵ����
	tadpole//���
} FrogType;

typedef enum
{
	Henhouse_Eggs=2,
	Henhouse_Eggbroken=1,
	Henhouse_Birds=0,
}HenhouseState;

typedef enum //����
{
	PropNone=-1,
	PropSingle,
	PropAddOne,
	PropRow,
	PropOneKind,
	PropCureSick,
}PropKind;

typedef enum
{
	BossAppear=0,
	BossHurt1,
	BossHurt2,
	BossDie
}BossState;

typedef enum
{
	Spring=30,
	Summer=80,
	Autumn=120,
	Winter=162,
}BackgroundType;

typedef enum //�̳�
{
	ClassNone=-1,
	ClassSanXiao,//����
	ClassSiXiao,//����
	ClassShiZiXiao,//ʮ����
	ClassWuXiao,//����
	ClassFishinbubble,//���������
	ClassButterfly,//�ۺ�
	ClassRattan,//����
	ClassFertileLand,//��������
	ClassHenHouse,//����
	ClassBarrenLand,//ƶ�����
	ClassFrozen,//����
	ClassSeed,//�ݵ�������
	ClassTiger,//�ϻ�
	ClassColorfulLegg,//�ʵ�

	ClassPropSingle=14,//����
	ClassPropAddOne,
	ClassPropRow,
	ClassPropOneKind,
	ClassPropCureSick,

	ClassBuyProp=19,
	ClassBuyLife=20,

	ClassShareFaceBook

}ClassKind;

struct ConnectionArea//��������
{
	MyPoint startPoint;
	Direction direction;
	int count;

	MyPoint centerPoint;

	MyPoint particlePoint;

	bool isInCross;

	bool isContain(MyPoint& point)
	{
		if(point.x==-1)
		{
			return false;
		}
		if(direction==Up)
		{
			return (point.x==startPoint.x)&&(point.y-startPoint.y>=0)&&(point.y-startPoint.y<count);
		}
		else
		{
			return (point.y==startPoint.y)&&(point.x-startPoint.x>=0)&&(point.x-startPoint.x<count);
		}
	}

	bool isCross(const ConnectionArea& connectArea,MyPoint& crossPoint)
	{
		if(this==&connectArea)
		{
			return false;
		}
		for(int i=0;i<connectArea.count;i++)
		{
			if(connectArea.direction==Up)
			{
				MyPoint point(connectArea.startPoint.x,connectArea.startPoint.y+i);

				if(isContain(point))
				{
					crossPoint.setPosition(point.x, point.y);
					isInCross = true;
					return true;
				}
			}
			else
			{
				MyPoint point(connectArea.startPoint.x+i,connectArea.startPoint.y);

				if(isContain(point))
				{
					crossPoint.setPosition(point.x, point.y);
					isInCross = true;
					return true;
				}
			}
		}
		crossPoint.reset();

		return false;
	}
};

struct PotentialArea//Ǳ������
{
	MyPoint movePoint;
	Direction diretion;
	MyPoint point1;
	MyPoint point2;
};
typedef enum
{
	BuyDimondFail=-1,
	BuyDimond1=0,
	BuyDimond2=1,
	BuyDimond3=2,
	BuyDimond4=3,
	BuyDimond5=4,
}BuyDimondMessage;

// static const int  MAP_TEST[kMatrixWidth][kMatrixWidth] = 
// {
// 	{-1, -1, -1, -1, -1, -1, -1, -1},
// 	{-1, -1, -1, -1, -1, -1, -1, -1},
// 	{-1, -1, -1, -1, -1, -1, -1, -1},
// 	{-1, -1, -1, -1, -1, -1, -1, -1},
// 	{-1, -1, -1, -1, -1, -1, -1, -1},
// 	{-1, -1, -1, -1, -1, -1, -1, -1},
// 	{-1, -1, -1, -1, -1, -1, -1, -1},
// 	{-1, -1, -1, -1, -1, -1, -1, -1}
// };

static const int  MAP_TEST[kMatrixWidth][kMatrixWidth] = 
{
	{0, 0, 0, -1, -1, 0, 0, 0},
	{0, 0, -1, -1, -1, -1, 0, 0},
	{0, -1, -1, -1, -1, -1, -1, 0},
	{-1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1},
	{0, -1, -1, -1, -1, -1, -1, 0},
	{0, 0, -1, -1, -1, -1, 0, 0},
	{0, 0, 0, -1, -1, 0, 0, 0}
};

#endif   //__DRAGONEGGTRIPLE_DATATOOLS_H__