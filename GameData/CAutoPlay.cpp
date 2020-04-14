#include "CAutoPlay.h"
#include "global_Var.h"
#include "StructGame.h"
#include "HookGameMainThread.h"
#include "GameFunction.h"
#include "TypeObj.h"

uniform_int_distribution<int> rndDelay(0, 200);//随机生成[0,200] 均匀分布的整数
default_random_engine e;	//随机数种子e,生成随机数

CAutoPlay g_cAutoPlay;

CAutoPlay::CAutoPlay() {
	//创新一个线程来专门给主线程发送消息
	DWORD proc = NULL;
	__asm {
		mov eax, ThreadProc_AutoGuaJi
		mov proc, eax
	}
	ht_AutoGuaJi = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)proc, this, CREATE_SUSPENDED, NULL);
}

CAutoPlay::~CAutoPlay() {
	TerminateThread(ht_AutoGuaJi, 1);
}

/********************************************************************************************/
/* Description	: 检查挂机范围
/* Return		: 超过返回真，没超过返回假
/* Time			: 2020/02/17 21:17
/* Remark		: 
/********************************************************************************************/
BOOL CAutoPlay::CheckGuaJiRange()
{
	g_tPlayerObj.GetData();
	float PlayerCurPosX = g_tPlayerObj.flCurX;
	float PlayerCurPosY = g_tPlayerObj.flCurY;
	DWORD dwDistance = TwoPoint_Distance(PlayerCurPosX, PlayerCurPosY, (float)GuaJiPosX, (float)GuaJiPosY);
	DbgOutput("与挂机点相距 %d米", dwDistance);
	if (dwDistance > RangeOfHitMonster)
	{
		msg_CancelSel();	//取消选中的怪
		DbgOutput("超出挂机范围,自动返回挂机点:%d,%d\n", GuaJiPosX, GuaJiPosY);
		FindWay(GuaJiPosX, GuaJiPosY);
		return TRUE;
	}
	return FALSE;
}

/* [2020/04/13 13:33]-[Remark: None] */
/* [地面上是否有物品可捡]-[Return: 有则返回1,没有返回0] */
BOOL CAutoPlay::IsHaveThingToPick()
{
	g_tGroundList.GetData();
	for (int i = 0; i < Size_NearList; i++)	//遍历地上物品列表
	{
		if (g_tGroundList.tlist[i].szName == NULL)	break;
		if (g_tGroundList.tlist[i].dwType == Type_GroundGoods)	return TRUE;
	}
	return FALSE;
}

/********************************************************************************************/
/* Description	: 判断是否需要回城
/* Return		: 需要回城返回真，不需要回城返回假
/* Time			: 2020/02/18 11:51
/* Remark		: 
/********************************************************************************************/
BOOL CAutoPlay::IsNeedBackToCity()
{
	g_tBackList.GetData();
	DWORD dwAllHpNum = g_tBackList.GetAllHpNum();
	DWORD dwAllMpNum = g_tBackList.GetAllMpNum();
	//Hp药品
	if (IsAutoBuyHpGoods)
	{
		if (dwAllHpNum < dwMinNum_HpGoods)
		{
			DbgOutput("Hp药品数量不足,需要回城补给\n");
			return TRUE;
		}
	}
	//Mp药品
	if (IsAutoBuyMpGoods)
	{
		if (dwAllMpNum < dwMinNum_MpGoods)
		{
			DbgOutput("Mp药品数量不足,需要回城补给\n");
			return TRUE;
		}
	}
	//背包满
	if (g_tBackList.IsBackpackFull())
	{
		DbgOutput("背包已满,需要回城出售物品,存放仓库\n");
		return TRUE;
	}
	//负重

	return FALSE;
}

/********************************************************************************************/
/* Description	: 回城补给
/* Return		: 成功返回真，失败返回假
/* Time			: 2020/02/18 15:48
/* Remark		: 
/********************************************************************************************/
BOOL CAutoPlay::BackToCitySupply()
{
	//取消选中对象
	msg_CancelSel();
	
	//到商店 出售 与 购买 物品
	GoToShopForSupply();

	//到仓库 存放 与 取出 物品
	GoToDepotForSupply();

	//返回到挂机坐标
	FindWay(GuaJiPosX, GuaJiPosY);
	return TRUE;
}

/********************************************************************************************/
/* Description	: 根据物品名判断该物品是否是Hp药品
/* Return		: 是则返回真，否则返回假
/* Time			: 2020/02/24 11:01
/* Remark		: 
/********************************************************************************************/
BOOL CAutoPlay::IsHpGoods(char* szName)
{
	if (strcmp(szName, "金创药(小)") == 0)	return TRUE;
	if (strcmp(szName, "金创药(中)") == 0)	return TRUE;
	if (strcmp(szName, "金创药(大)") == 0)	return TRUE;
	if (strcmp(szName, "金创药(特)") == 0)	return TRUE;
	if (strcmp(szName, "秘制金创药") == 0)	return TRUE;
	if (strcmp(szName, "药仙金创药") == 0)	return TRUE;
	return FALSE;
}

/********************************************************************************************/
/* Description	: 根据物品名判断该物品是否是Mp药品
/* Return		: 是 则返回真，否则返回假
/* Time			: 2020/02/24 11:06
/* Remark		: 
/********************************************************************************************/
BOOL CAutoPlay::IsMpGoods(char* szName)
{
	if (strcmp(szName, "人参") == 0)	return TRUE;
	if (strcmp(szName, "野人参") == 0)	return TRUE;
	if (strcmp(szName, "雪原参") == 0)	return TRUE;
	if (strcmp(szName, "益元参") == 0)	return TRUE;
	if (strcmp(szName, "秘制益元参") == 0)	return TRUE;
	if (strcmp(szName, "药仙益元参") == 0)	return TRUE;
	return FALSE;
}

BOOL CAutoPlay::IsStone(char * szName)
{
	if (strcmp(szName, "强化石") == 0)	return TRUE;
	if (strcmp(szName, "寒玉石") == 0)	return TRUE;
	if (strcmp(szName, "金刚石") == 0)	return TRUE;
	if (strcmp(szName, "热血石") == 0)	return TRUE;
	return FALSE;
}

/********************************************************************************************/
/* Description	: 到商店 出售 与 购买 物品
/* Return		: 成功返回真，失败返回假
/* Time			: 2020/02/22 16:07
/* Remark		: 
/********************************************************************************************/
BOOL CAutoPlay::GoToShopForSupply()
{
	//寻路到平十指
	BOOL bRet = FindWay(737, 1955);
	if (bRet == FALSE)	return FALSE;
	Sleep(2000 + rndDelay(e));

	//打开NPC对话
	msg_ChatWithNPC("平十指");
	Sleep(2000 + rndDelay(e));

	//打开商店
	msg_OpenShop("平十指");
	Sleep(2000 + rndDelay(e));

	//出售所有设定物品到商店
	SellSetGoodsToShop();
	Sleep(1000 + rndDelay(e));

	//从商店购买所有设定物品
	BuySetGoodsFromShop();
	Sleep(1000 + rndDelay(e));

	//结束与NPC对话
	msg_EndChatWithNPC("平十指");
	Sleep(1000 + rndDelay(e));
	return TRUE;
}

/********************************************************************************************/
/* Description	: 出售所有设定物品到商店
/* Return		: 
/* Time			: 2020/02/23 20:46
/* Remark		: 
/********************************************************************************************/
void CAutoPlay::SellSetGoodsToShop()
{
	for (vector<TGoodsManage>::iterator it = vct_GoodsMngList.begin();
		it < vct_GoodsMngList.end(); it++)
	{
		if ((it->dwGoodsManageFlag) & Goods_Manage_Shop)
		{
			//查询背包是否有此物品,有的话全部出售到商店
			int iIndex = g_tBackList.GetGoodsIndexByName(it->szGoodsName);
			if (iIndex >= 0)
			{
				DWORD dwNum = g_tBackList.GetGoodsNumByName(it->szGoodsName);
				msg_SellGoods(it->szGoodsName, dwNum);	//凡是涉及到CALL都要封装成消息
				Sleep(200);
			}
		}
	}
}

/********************************************************************************************/
/* Description	: 从商店购买所有设定物品
/* Return		: 
/* Time			: 2020/02/23 20:47
/* Remark		: 
/********************************************************************************************/
void CAutoPlay::BuySetGoodsFromShop()
{
	for (vector<TSupplyData>::iterator it = vct_SupplyList.begin();
		it < vct_SupplyList.end(); it++)
	{
		if (strcmp(it->szAddr, "购买药符") == 0)
		{
			if (IsHpGoods(it->szGoodsName))	//是Hp药品则最多购买到设定的最大值
			{
				int iIndex = g_tBackList.GetGoodsIndexByName(it->szGoodsName);	//查询背包是否有此物品
				if (iIndex >= 0)	
				{	//有则获取数量
					DWORD dwNum = g_tBackList.GetGoodsNumByName(it->szGoodsName);
					if ((dwNum + it->dwNum) > dwMaxNum_HpGoods)	//当设定购买数量与现存数量之和 大于 设定的最大值
						if (dwMaxNum_HpGoods > dwNum)
						{
							DbgOutput("商店购买物品:%s,%d", it->szGoodsName, dwMaxNum_HpGoods - dwNum);
							msg_BuyGoods(it->szGoodsName, dwMaxNum_HpGoods - dwNum);	//只购买到最大值
						}
				}
				else
				{	//没有则购买设定数量
					DbgOutput("商店购买物品:%s,%d", it->szGoodsName, it->dwNum);
					msg_BuyGoods(it->szGoodsName, it->dwNum);
				}
			}
			else if(IsMpGoods(it->szGoodsName))	//是Mp药品则最多购买到设定的最大值
			{
				
				int iIndex = g_tBackList.GetGoodsIndexByName(it->szGoodsName);	//查询背包是否有此物品
				if (iIndex >= 0)	
				{	//有则获取数量
					DWORD dwNum = g_tBackList.GetGoodsNumByName(it->szGoodsName);
					if ((dwNum + it->dwNum) > dwMaxNum_MpGoods)	//当设定购买数量与现存数量之和 大于 设定的最大值
						if (dwMaxNum_MpGoods > dwNum)
						{
							DbgOutput("商店购买物品:%s,%d", it->szGoodsName, dwMaxNum_MpGoods - dwNum);
							msg_BuyGoods(it->szGoodsName, dwMaxNum_MpGoods - dwNum);	//只购买到最大值
						}
				}
				else
				{	//没有则购买设定数量
					DbgOutput("商店购买物品:%s,%d", it->szGoodsName, it->dwNum);
					msg_BuyGoods(it->szGoodsName, it->dwNum);
				}
			}
			else   //不是HpMp药品则购买设定数量
			{
				DbgOutput("商店购买物品:%s,%d", it->szGoodsName, it->dwNum);
				msg_BuyGoods(it->szGoodsName, it->dwNum);	
			}
		}
	}
}

/********************************************************************************************/
/* Description	: 到仓库 存放 与 取出 物品
/* Return		: 成功返回真，失败返回假
/* Time			: 2020/02/22 16:08
/* Remark		: 
/********************************************************************************************/
BOOL CAutoPlay::GoToDepotForSupply()
{
	//寻路到韦大宝
	BOOL bRet = FindWay(151, 1786);
	if (bRet == FALSE)	return FALSE;
	Sleep(2000 + rndDelay(e));

	//打开NPC对话
	msg_ChatWithNPC("韦大宝");
	Sleep(2000 + rndDelay(e));

	//打开仓库
	msg_OpenShop("韦大宝");
	Sleep(2000 + rndDelay(e));

	//存放设定物品到仓库
	StoreSetGoodsToDepot();
	Sleep(1000 + rndDelay(e));

	//从仓库取出所有设定物品
	PickSetGoodsFromDepot();
	Sleep(1000 + rndDelay(e));

	//结束与NPC对话
	msg_EndChatWithNPC("韦大宝");
	Sleep(1000 + rndDelay(e));
	return TRUE;
}

/********************************************************************************************/
/* Description	: 存放所有设定物品到仓库
/* Return		: 成功返回真，失败返回假
/* Time			: 2020/02/22 19:46
/* Remark		: 
/********************************************************************************************/
void CAutoPlay::StoreSetGoodsToDepot()
{
	for (int i=0; i<36; i++)	//遍历背包
	{
		//背包对象不存在,则跳过本次循环
		if (g_tBackList.tlist[i].dwGoodsObj == NULL)	continue;
		char* szName = g_tBackList.tlist[i].szName;	//读取背包第i个物品的名字
		for (vector<TGoodsManage>::iterator it = vct_GoodsMngList.begin();
			it < vct_GoodsMngList.end(); it++)	//遍历整个 物品管理列表
		{
			if (strcmp(szName,it->szGoodsName)==0)
			{
				if ( (it->dwGoodsManageFlag) & Goods_Manage_Depot )
				{
					//查询背包是否有此物品,有的话全部存到仓库
					int iIndex = g_tBackList.GetGoodsIndexByName(it->szGoodsName);
					if (iIndex >= 0)
					{
						DWORD dwNum = g_tBackList.GetGoodsNumByName(it->szGoodsName);
						msg_MoveGoodsToDepot(it->szGoodsName, dwNum);	//凡是涉及到CALL都要封装成消息
					}
				}
				break;	//找到对应物品后,就不再遍历 物品管理列表
			}
		}
		Sleep(50 + rndDelay(e));
	}
}

/********************************************************************************************/
/* Description	: 从仓库取出所有设定物品
/* Return		: 成功返回真，失败返回假
/* Time			: 2020/02/22 19:49
/* Remark		: 
/********************************************************************************************/
void CAutoPlay::PickSetGoodsFromDepot()
{
	for (vector<TSupplyData>::iterator it = vct_SupplyList.begin();
		it < vct_SupplyList.end(); it++)
	{
		if (strcmp(it->szAddr, "取出物品") == 0)
		{
			//查询仓库是否有此物品
			int iIndex = g_tDeptList.GetGoodsIndexByName(it->szGoodsName);
			if (iIndex >= 0)
			{
				DWORD dwNum = g_tDeptList.GetGoodsNumByName(it->szGoodsName);
				//再比较仓库中物品数量 与 设定要取出的数量,取当中的较小值
				if (dwNum > it->dwNum)	dwNum = it->dwNum;
				msg_MoveGoodsToBcpk(it->szGoodsName, dwNum);
			}
		}
	}
}




/********************************************************************************************/
/* Description	: 自动挂机线程函数
/* Return		: 成功返回真，失败返回假
/* Time			: 2020/01/23 11:18
/* Remark		: 
/********************************************************************************************/
void CAutoPlay::ThreadProc_AutoGuaJi(LPVOID lpData)
{
	__asm {
		mov ecx,lpData
		mov this,ecx
	}
	while (LoopFlag)
	{
		//判断是否需要回城补给
		if (IsNeedBackToCity())
			BackToCitySupply();

		//捡物
		if (IsPickupGoods)
			if (IsHaveThingToPick())
			{
				msg_PickupGoods();
				Sleep(1000);
				msg_CancelSel();
			}
		Sleep(200 + rndDelay(e));

		//定点范围检测
		if (IsRangeLimit)
			CheckGuaJiRange();

		//打怪
		if (IsAutoBeatMonster)
		{
			if (IsUseSkill)
				msg_AutoBeatMonsterBySkill(szSkillName);
			else
				msg_AutoBeatMonsterBySkill("攻击");
		}
		DbgOutput("-----------------------------------------------------------");
		Sleep(200 + rndDelay(e));
	}
}

void CAutoPlay::LowHpMpProtect()
{
	g_tRoleProperty.GetData();
	g_tBackList.GetData();
	if (IsAutoUseHpGoods)
	{
		if (g_tRoleProperty.dwHpPercent <= dwHpPercent)
		{
			if (g_tBackList.GetGoodsIndexByName(szHpGoodsName) == -1)	// 若最初选择的药品没了
			{
				strcpy_s(szHpGoodsName, "小九转丹(礼物)");
				if (g_tBackList.GetGoodsIndexByName(szHpGoodsName) == -1)	// 若最初选择的药品没了
				{
					strcpy_s(szHpGoodsName, "九转丹(礼物)");
				}
			}
			msg_UseGoodsByName(szHpGoodsName);
		}
	}
	if (IsAutoUseMpGoods)
	{
		if (g_tRoleProperty.dwMpPercent <= dwMpPercent)
		{
			if (g_tBackList.GetGoodsIndexByName(szHpGoodsName) == -1)	// 若最初选择的药品没了
			{
				strcpy_s(szHpGoodsName, "千年雪参(礼物)");
			}
			msg_UseGoodsByName(szMpGoodsName);
		}
	}
}

void CAutoPlay::StartAutoPlay()
{
	LoopFlag = TRUE;
	ResumeThread(ht_AutoGuaJi);		//恢复线程执行
}

void CAutoPlay::StopAutoPlay()
{
	LoopFlag = FALSE;
	SuspendThread(ht_AutoGuaJi);		//挂起线程
}
