#pragma once
#include <Windows.h>
#include <vector>
#include <random>    //随机数

using namespace std;

//物品补给方式的数据结构
typedef struct TSupplyData 
{
	char szAddr[30];		//去哪买
	char szGoodsName[30];	//买什么
	DWORD dwNum;			//买多少
}TSupplyData;

//战利品处理方式的数据结构
#define Goods_Manage_No 0		//不处理 000
#define Goods_Manage_Depot 1	//存仓库 001
#define Goods_Manage_Shop 2		//售商店 010
#define Goods_Manage_NoPick 4	//不拾取	100
typedef struct TGoodsManage 
{
	char szGoodsName[30];	//物品名字
	DWORD dwGoodsManageFlag;//物品处理方式
}TGoodsManage;

class CAutoPlay
{
public:
	CAutoPlay();
	~CAutoPlay();

	HANDLE ht_AutoGuaJi;	//挂机线程句柄

	//打怪相关
	BOOL IsAutoBeatMonster;		// 是否自动打怪
	BOOL IsRangeLimit;			// 是否限制打怪范围
	DWORD RangeOfHitMonster;	// 打怪范围半径
	int GuaJiPosX;				// 挂机点坐标X
	int GuaJiPosY;				// 挂机点坐标Y
	BOOL CheckGuaJiRange();		// 检查挂机范围
	BOOL IsUseSkill;			// 是否使用技能
	char szSkillName[20];		// 打怪用的技能
	BOOL IsPickupGoods;			// 是否自动捡物
	BOOL IsHaveThingToPick();	// 地面上是否有物品可捡

	//保护相关
	BOOL IsAutoUseHpGoods;	//是否在低HP时自动使用药品
	BOOL IsAutoUseMpGoods;	//是否在低MP时自动使用药品
	char szHpGoodsName[20];	//低HP时自动使用的物品名
	char szMpGoodsName[20];	//低MP时自动使用的物品名
	DWORD dwHpPercent;		//低HP判断百分比
	DWORD dwMpPercent;		//低MP判断百分比
	DWORD dwCheckHpMpSpeed;	//血蓝检测速度

	//轻功相关
	BOOL IsAutoUseQingGong;		//是否自动使用轻功
	char szQingGongName[20];	//轻功技能名

	//回城买卖相关
	BOOL IsAutoBuyHpGoods;		//低于设定Hp药品数量是否回城补给
	DWORD dwMinNum_HpGoods;		//Hp药品数量低于此值时回城补给
	DWORD dwMaxNum_HpGoods;		//Hp药品数量回城补给到该数值
	char szHpGoodsNameToSupply[20];	//回城补给的Hp药品名

	BOOL IsAutoBuyMpGoods;		//低于设定Mp药品数量是否回城补给
	DWORD dwMinNum_MpGoods;		//Mp药品数量低于此值时回城补给
	DWORD dwMaxNum_MpGoods;		//Mp药品数量回城补给到该数值
	char szMpGoodsNameToSupply[20];	//回城补给的Mp药品名

	BOOL IsNeedBackToCity();	//判断是否需要回城
	BOOL BackToCitySupply();	//回城补给
	vector<TSupplyData> vct_SupplyList;		//存放 补给物品列表 的容器
	vector<TGoodsManage> vct_GoodsMngList;	//存放 处理物品列表 的容器
	BOOL IsHpGoods(char* szName);			//根据物品名判断该物品是否是Hp药品
	BOOL IsMpGoods(char* szName);			//根据物品名判断该物品是否是Mp药品
	BOOL IsStone(char* szName);				//根据物品名判断该物品是否是强化石,寒玉石
	
	BOOL GoToShopForSupply();	//到商店 出售 与 购买 物品
	void SellSetGoodsToShop();	//出售所有设定物品到商店
	void BuySetGoodsFromShop();	//从商店购买所有设定物品

	BOOL GoToDepotForSupply();	//到仓库 存放 与 取出 物品
	void StoreSetGoodsToDepot();	//存放所有设定物品到仓库
	void PickSetGoodsFromDepot();	//从仓库取出所有设定物品

	void ThreadProc_AutoGuaJi(LPVOID lpData);	//自动挂机线程函数
	void LowHpMpProtect();

	void StartAutoPlay();	//开始挂机
	void StopAutoPlay();	//停止挂机
	BOOL LoopFlag;			//循环标志
};

