#include "pch.h"
#include "BaseGame.h"
#include "StructGame.h"
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include "TypeObj.h"
#include "global_Var.h"
#include "HookGameMainThread.h"

TRoleProperty g_tRoleProperty;	//角色属性
TPlayerObj g_tPlayerObj;		//玩家对象

TBackpackList g_tBackList;	//背包列表
TDepotList g_tDeptList;		//仓库列表
TShopList g_tShopList;		//商店列表
TNearList g_tNearList;		//周围列表
TActionList g_tActList;		//动作列表
TSkillList g_tSkiList;		//技能列表
TShortcutList g_tScList;	//快捷栏列表
TGroundGoodsList g_tGroundList;	//地面物品列表

DWORD dwShopBase = NULL;	// 商店列表首地址


void DbgOutput(const char *szFormat, ...) {
#ifdef _DEBUG
	char szbufFormat[0x1000];
	char szBufFormat_Game[0x1008] = "Game:";
	va_list argList;
	va_start(argList, szFormat);     //参数列表初始化
	vsprintf_s(szbufFormat, szFormat, argList);
	strcat_s(szBufFormat_Game, szbufFormat);
	OutputDebugStringA(szBufFormat_Game);
	va_end(argList);
#endif
}

/* [2020/01/18 13:33]-[Remark: None] */
/* [获取游戏窗口句柄]-[成功返回窗口句柄，否则返回NULL] */
HWND GetGameWndHandle() {
	HWND hGame;
	__try
	{
		hGame = *(HWND *)Base_GameWndHandle;
	}
	__except (1)
	{
		DbgOutput("HWND GetGameWndHandle() 异常\n");
		return NULL;
	}
	return hGame;
}

/* [2020/01/18 13:20]-[Remark: None] */
/* [计算两坐标点的距离]-[返回两点间距离_float x1, float y1, float x2, float y2] */
DWORD TwoPoint_Distance(float x1, float y1, float x2, float y2) {
	DWORD dwDistance = 0xFFFF;
	double a = double(x1 - x2);
	double b = double(y1 - y2);
	double c = sqrt(a * a + b * b);
	dwDistance = (DWORD)c;
	return dwDistance;
}

/* [2020/03/01 12:59]-[Remark: None] */
/* [发送数据到服务器]-[成功返回真，否则返回假] */
BOOL WSASendData(LPCVOID lpData, DWORD dwBufSize)
{
	__try
	{
		__asm
		{
			push dwBufSize
			push lpData
			mov ecx, dword ptr ds : [Call_SendData_Ecx]
			mov eax, Call_SendData
			call eax
		}
	}
	__except (1)
	{
		DbgOutput("BOOL WSASendData(LPCVOID lpData, DWORD dwBufSize) 异常\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 12:59]-[Remark: 参数为任务列表ID] */
/* [打印当前任务列表]-[无返回值] */
void PrintAllTask(DWORD dwTaskListID)
{
	DWORD Reg_edi;
	DWORD dwEnd;
	DWORD Reg_esi;
	BYTE byLevel;
	char* szName;
	DWORD dw0x10;
	DWORD Reg_eax;
	DbgOutput("进入void PrintAllTask(DWORD dwTaskListID)\n");
	__try
	{	//先点击选项卡
		__asm
		{
			mov ecx, Base_AllObjList
			mov ecx, dword ptr [ecx + 4 * 0x633]
			mov edx, dword ptr [ecx]
			mov edx, dword ptr [edx + 0x4]
			mov Reg_edi, edx
			push 0x0
			push dwTaskListID
			push 0x3F4
			call edx
		}
		DbgOutput("edx=%X\n", Reg_edi);
		DbgOutput("初始化数据完成,即将读取数据\n");
		Reg_esi = *(DWORD*)(*(DWORD*)(Base_Unknown) + 0x2BC);
		Reg_edi = *(DWORD*)(Reg_esi + 0x4F0);
		dwEnd = *(DWORD*)(Reg_esi + 0x4F4);
		DbgOutput("Reg_esi:%X\n", Reg_esi);
		DbgOutput("Reg_edi:%X\n", Reg_edi);
		DbgOutput("dwEnd:%X\n", dwEnd);
		while (Reg_edi != dwEnd)
		{
			//任务等级
			byLevel = *(BYTE*)(*(DWORD*)(Base_Unknown2) + (*(DWORD*)Reg_edi) * 0xD4 + 0x20);
			//任务名称
			Reg_eax = *(DWORD*)(Base_Unknown2) + (*(DWORD*)Reg_edi) * 0xD4 + 0x4;
			dw0x10 = *(DWORD*)(*(DWORD*)(Base_Unknown2) + (*(DWORD*)Reg_edi) * 0xD4 + 0x18);
			if (dw0x10 >= 0x10)	Reg_eax = *(DWORD*)Reg_eax;
			szName = (char*)Reg_eax;
			DbgOutput("[%d] %s\n", byLevel, szName);
			Reg_edi += 8;
		}
	}
	__except(1)
	{
		DbgOutput("void PrintAllTask(DWORD dwTaskListID) 异常\n");
	}
}

TRoleProperty* TRoleProperty::GetData()
{
	__try
	{
		szRoleName = (char *)Base_RoleProperty;
		byLevel = *(BYTE *)(Base_RoleProperty + 0x34);
		byTransferNum = *(BYTE *)(Base_RoleProperty + 0x35);
		szRole名声 = (char *)(Base_RoleProperty + 0x36);
		dwHp = *(DWORD *)(Base_RoleProperty + 0x80);
		dwMp = *(DWORD *)(Base_RoleProperty + 0x84);
		dwAngry = *(DWORD *)(Base_RoleProperty + 0x88);
		dwHpMax = *(DWORD *)(Base_RoleProperty + 0x8C);
		dwMpMax = *(DWORD *)(Base_RoleProperty + 0x90);
		dwAngryMax = *(DWORD *)(Base_RoleProperty + 0x94);
		nqExp = *(QWORD *)(Base_RoleProperty + 0x98);
		nqExpMax = *(QWORD *)(Base_RoleProperty + 0xA0);
		dw历练 = *(DWORD *)(Base_RoleProperty + 0xAC);
		dw心力 = *(DWORD *)(Base_RoleProperty + 0xB0);
		dw气力 = *(DWORD *)(Base_RoleProperty + 0xB4);
		dw体力 = *(DWORD *)(Base_RoleProperty + 0xB8);
		dw魂力 = *(DWORD *)(Base_RoleProperty + 0xBC);
		dw攻击 = *(DWORD *)(Base_RoleProperty + 0xC8);
		dw防御 = *(DWORD *)(Base_RoleProperty + 0xCC);
		dw命中 = *(DWORD *)(Base_RoleProperty + 0xD0);
		dw回避 = *(DWORD *)(Base_RoleProperty + 0xD4);
		nqMoney = *(QWORD *)(Base_RoleProperty + 0xE4);
		nw负重 = *(WORD *)(Base_RoleProperty + 0xEC);
		nw负重Max = *(WORD *)(Base_RoleProperty + 0xEE);
		byPoint气功 = *(BYTE *)(Base_RoleProperty + 0xF0);
		for (int i = 0; i < 32; i++)
		{
			byQiGongPoints[i] = *(BYTE *)(Base_RoleProperty + 0xF4 + 4 * i);
		}
		dwHpPercent = 100*dwHp / dwHpMax;
		dwMpPercent = 100*dwMp / dwMpMax;
	}
	__except (1)
	{
		DbgOutput("TRoleProperty* TRoleProperty::GetData() 异常\n");
		return NULL;
	}
	return this;
}

void TRoleProperty::PrintMsg()
{
	GetData();
	DbgOutput("角色名：%s\n", szRoleName);
	DbgOutput("等级：%d\n", byLevel);
	DbgOutput("转职次数：%d\n", byTransferNum);
	DbgOutput("名声：%s\n", szRole名声);
	DbgOutput("HP：%d/%d\n", dwHp,dwHpMax);
	DbgOutput("MP：%d/%d\n", dwMp,dwMpMax);
	DbgOutput("愤怒：%d/%d\n", dwAngry,dwAngryMax);
	DbgOutput("Exp：%d/%d\n", nqExp, nqExpMax);
	DbgOutput("负重：%d/%d\n", nw负重, nw负重Max);
	DbgOutput("历练值：%d\n", dw历练);
	DbgOutput("心：%d\n", dw心力);
	DbgOutput("气：%d\n", dw气力);
	DbgOutput("体：%d\n", dw体力);
	DbgOutput("魂：%d\n", dw魂力);
	DbgOutput("攻击：%d\n", dw攻击);
	DbgOutput("防御：%d\n", dw防御);
	DbgOutput("命中：%d\n", dw命中);
	DbgOutput("回避：%d\n", dw回避);
	DbgOutput("金钱：%d\n", nqMoney);
	DbgOutput("未分配气功点：%d\n", byPoint气功);
	for (int i=0;i<32;i++)
	{
		DbgOutput("技能%d,已分配气功点：%d\n", i, byQiGongPoints[i]);
	}
}

TBackpackList* TBackpackList::GetData()
{
	memset(tlist, 0, sizeof(tlist));
	__try
	{
		DWORD dwBase = *(DWORD *)Base_BackPackList;
		DWORD dwFirstGoodsBase = dwBase + 0x43C;
		DWORD dwObj = NULL;
		for (int i=0;i<36;i++)
		{
			dwObj = *(DWORD *)(dwFirstGoodsBase + 4 * i);	//取出第i格对象地址
			tlist[i].dwGoodsObj = dwObj;
			if (tlist[i].dwGoodsObj == NULL)	continue;
			tlist[i].szName = (char *)(dwObj + 0x64);
			tlist[i].szMsg = (char *)(dwObj + 0x0F9);
			tlist[i].szAttribute = (char *)(dwObj + 0x238);
			tlist[i].nwNum = *(WORD *)(dwObj + 0xC4C);

			tlist[i].dwType1 = *(DWORD*)(dwObj + 0x54);
			tlist[i].nqID2 = *(QWORD*)(dwObj + 0x5C);
			tlist[i].byIndex = *(BYTE*)(dwObj + 0x1FC);
		}
	}
	__except (1)
	{
		DbgOutput("TBackpackList* TBackpackList::GetData() 异常\n");
		return NULL;
	}
	return this;
}

void TBackpackList::PrintMsg()
{
	GetData();
	for (int i = 0; i < 36; i++)
	{
		if (tlist[i].dwGoodsObj == NULL)
		{
			continue;
		}
		DbgOutput("物品名（%s）:物品数量=%d,物品功能描述=%s,物品属性=%s\n",
			tlist[i].szName,
			tlist[i].nwNum,
			tlist[i].szMsg,
			tlist[i].szAttribute
			);
	}
}


/* [2020/03/01 13:00]-[Remark: None] */
/* [根据下标使用背包物品]-[成功返回真，否则返回假] */
BOOL TBackpackList::UseGoodsByIndex(DWORD dwIndex) {
	GetData();
	__try
	{ 
		__asm
		{
			mov eax, dwIndex
			push eax
			push 1
			push 0
			mov ecx, Base_BackPackList
			mov ecx, [ecx]
			mov eax, Call_UseObjForIndex
			call eax    //根据下标使用背包物品 函数的首地址
		}
	}
	__except (1)
	{
		DbgOutput("GameDebug:UseGoodsByIndex(DWORD dwIndex) 异常\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:00]-[Remark: None] */
/* [根据名字查找背包内物品，获取物品所在背包列表的下标]-[成功返回物品所在背包列表的下标，失败返回-1] */
int TBackpackList::GetGoodsIndexByName(char *szName) {
	GetData();
	for (int i = 0;i<36; i++)
	{
		if (tlist[i].dwGoodsObj == NULL)	continue;
		if (strcmp(szName,tlist[i].szName) == 0)
		{
			return i;
		}
	}
	return -1;
}

/* [2020/03/01 13:01]-[Remark: None] */
/* [使用背包物品 根据名称]-[成功返回真，否则返回假] */
BOOL TBackpackList::UseGoodsByName(char *szName) {
	DWORD dwIndex = GetGoodsIndexByName(szName);	//通过名字查物品获取下标
	if (dwIndex == -1)
	{
		return FALSE;
	}
	UseGoodsByIndex(dwIndex);	//通过下标使用物品
	return TRUE;
}

/* [2020/03/01 13:01]-[Remark: 只是让选中对象变为物品对象，但并不会拖起这个物品] */
/* [选中背包指定格的物品]-[成功返回物品对象，失败返回NULL] */
DWORD TBackpackList::SelectGoods(DWORD dwIndex)
{
	DWORD dwObj = NULL;
	__try
	{
		__asm
		{
			mov ebx, dwIndex
			mov edi, dword ptr ds:[Base_BackPackList]
			mov edi, dword ptr ds:[edi + 0x43C + ebx*4]
			mov dwObj,edi
			mov eax, dword ptr ds:[Base_MouseSelObj]
			mov dword ptr ds:[eax + 0x230], edi
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TBackpackList::SelectGoods(DWORD dwIndex) 异常\n");
		return NULL;
	}
	DbgOutput("选中对象dwObj = %X\n",dwObj);
	return dwObj;
}

/* [2020/03/01 13:02]-[Remark: None] */
/* [查询背包指定物品数量]-[成功返回真，否则返回假] */
DWORD TBackpackList::GetGoodsNumByName(char* szName)
{
	for (int i = 0; i < 36; i++)
	{
		if (tlist[i].dwGoodsObj == NULL)	continue;
		if (strcmp(szName, tlist[i].szName) == 0)
		{
			return tlist[i].nwNum;
		}
	}
	return 0;
}

/* [2020/03/01 13:02]-[Remark: None] */
/* [查询所有Hp药品数量]-[返回Hp药品数量] */
DWORD TBackpackList::GetAllHpNum()
{
	GetData();
	DWORD dwAllHpNum = 0;
	dwAllHpNum += GetGoodsNumByName("小九转丹(礼物)");
	dwAllHpNum += GetGoodsNumByName("九转丹(礼物)");
	if (dwAllHpNum > 0)
	{
		return 1000;	//只要有九转丹,就不缺药,直接认为药品数量有1000个
	}
	dwAllHpNum += GetGoodsNumByName("金创药(小)");
	dwAllHpNum += GetGoodsNumByName("金创药(中)");
	dwAllHpNum += GetGoodsNumByName("金创药(大)");
	dwAllHpNum += GetGoodsNumByName("金创药(特)");
	dwAllHpNum += GetGoodsNumByName("秘制金创药");
	dwAllHpNum += GetGoodsNumByName("药仙金创药");
	return dwAllHpNum;
}

/* [2020/03/01 13:02]-[Remark: None] */
/* [查询所有Mp药品数量]-[返回Mp药品数量] */
DWORD TBackpackList::GetAllMpNum()
{
	GetData();
	DWORD dwAllMpNum = 0;
	dwAllMpNum += GetGoodsNumByName("千年雪参(礼物)");
	if (dwAllMpNum > 0)
	{
		return 1000;	//只要有千年雪参,就不缺药,直接认为药品数量有1000个
	}
	dwAllMpNum += GetGoodsNumByName("人参");
	dwAllMpNum += GetGoodsNumByName("野山参");
	dwAllMpNum += GetGoodsNumByName("雪原参");
	dwAllMpNum += GetGoodsNumByName("益元参");
	dwAllMpNum += GetGoodsNumByName("秘制益元参");
	dwAllMpNum += GetGoodsNumByName("药仙益元参");
	return dwAllMpNum;
}

/* [2020/03/01 13:02]-[Remark: None] */
/* [判断背包是否满]-[背包满返回真，没满返回假] */
BOOL TBackpackList::IsBackpackFull()
{
	for (int i=0; i < 36; i++)
	{
		if (tlist[i].nwNum == 0)
		{
			return FALSE;	//只要有一格没有物品,背包就没有满
		}
	}
	return TRUE;	//背包满
}

/* [2020/03/01 13:03]-[Remark: 可不传参，默认为1] */
/* [移动已选中物品到仓库]-[成功返回真，失败返回假] */
BOOL TBackpackList::MoveGoodsToDepot()
{
	__try
	{
		__asm
		{
			mov edi, dword ptr ds:[Base_DepotList]
			mov ecx, dword ptr ds:[edi+0x1634]
			mov edx, dword ptr ds:[edi+0x1C60]
			mov eax, 1
			push eax
			push ecx
			push edx
			
			mov ecx, edi
			mov eax, Call_MoveObjToList
			call eax
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TBackpackList::MoveGoodsToDepot() 异常\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:03]-[Remark: None] */
/* [移动背包指定名字的物品到仓库]-[成功返回真，否则返回假] */
BOOL TBackpackList::MoveGoodsToDepot(char* szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToDepot();
	return bRet;
}

/* [2020/03/01 13:04]-[Remark: None] */
/* [移动背包指定名字的物品到仓库,需指定数量]-[成功返回真，否则返回假] */
BOOL TBackpackList::MoveGoodsToDepot(char * szName, WORD nwIndex)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	BYTE byData[] = {
		0x00,0x00,0x94,0x00,0x8C,0x00,0x01,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x11,0x52,
		0x1C,0xEB,0x68,0xCA,0x9A,0x3B,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0xC5,0x57,0x0D,0x00,0x00,0x00,0x00,0x00,0x05,0x5E,0xB9,0xCA,0x17,0x0E,
		0xC4,0x06,0x68,0xCA,0x9A,0x3B,0x00,0x00,0x00,0x00,0x54,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x20,0xEC,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0xEC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x21,0xEC,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x58,0x21,0xEC,0x00,0x00,
		0x00,0x00,0x00,0x00,0x21,0xEC,0x18,0x5B,0x21,0xEC,0x16,0x58,0x20,0xEC,0x15,0x56,
		0x20,0xEC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	TStorePickDepotData* pTStorePickDepotData = (TStorePickDepotData*)byData;
	
	__try 
	{
		pTStorePickDepotData->dwType1 = tlist[iIndex].dwType1;
		pTStorePickDepotData->nwSaveNum = nwIndex;
		pTStorePickDepotData->nqID2 = tlist[iIndex].nqID2;	//注意：高位字节要在高位
		pTStorePickDepotData->dwType3 = tlist[iIndex].dwType1;
		pTStorePickDepotData->nwGoodsNum = tlist[iIndex].nwNum;
		pTStorePickDepotData->byIndex = tlist[iIndex].byIndex;
		for (int i = 0; i < 0xA; i++)
		{
			DbgOutput("%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
				byData[i * 0x10 + 0x0], byData[i * 0x10 + 0x1], byData[i * 0x10 + 0x2], byData[i * 0x10 + 0x3],
				byData[i * 0x10 + 0x4], byData[i * 0x10 + 0x5], byData[i * 0x10 + 0x6], byData[i * 0x10 + 0x7],
				byData[i * 0x10 + 0x8], byData[i * 0x10 + 0x9], byData[i * 0x10 + 0xA], byData[i * 0x10 + 0xB],
				byData[i * 0x10 + 0xC], byData[i * 0x10 + 0xD], byData[i * 0x10 + 0xE], byData[i * 0x10 + 0xF]
			);
		}
		WSASendData(pTStorePickDepotData, 0x8E);
	}
	__except (1)
	{
		DbgOutput("BOOL TBackpackList::MoveGoodsToDepot(char * szName, DWORD dwIndex) 异常\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:04]-[Remark: None] */
/* [移动背包已选中物品到装备列表指定格]-[成功返回真，否则返回假] */
BOOL TBackpackList::MoveGoodsToEquip(DWORD dwIndex)
{
	__try
	{
		__asm
		{
			mov edi, dword ptr ds : [Base_EquipList]
			mov ecx, dword ptr ds : [edi + 0x1634]
			mov edx, dword ptr ds : [edi + 0x1C60]
			mov eax, dwIndex
			push eax
			push ecx
			push edx

			mov ecx, edi
			mov eax, Call_MoveObjToList
			call eax
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TBackpackList::MoveGoodsToEquip(DWORD dwIndex) 异常\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:04]-[Remark: None] */
/* [移动背包指定名字的物品到装备衣服栏]-[成功返回真，否则返回假] */
BOOL TBackpackList::MoveGoodsToEquip_Clothes(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwClothes);
	return bRet;
}

/* [2020/03/01 13:05]-[Remark: None] */
/* [移动背包指定名字的物品到装备左护手]-[成功返回真，否则返回假] */
BOOL TBackpackList::MoveGoodsToEquip_HandL(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwHandL);
	return bRet;
}

/* [2020/03/01 13:05]-[Remark: None] */
/* [移动背包指定名字的物品到装备右护手]-[成功返回真，否则返回假] */
BOOL TBackpackList::MoveGoodsToEquip_HandR(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwHandR);
	return bRet;
}

/* [2020/03/01 13:05]-[Remark: None] */
/* [移动背包指定名字的物品到装备武器]-[成功返回真，否则返回假] */
BOOL TBackpackList::MoveGoodsToEquip_Weapon(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwWeapon);
	return bRet;
}

/* [2020/03/01 13:05]-[Remark: None] */
/* [移动背包指定名字的物品到装备鞋子]-[成功返回真，否则返回假] */
BOOL TBackpackList::MoveGoodsToEquip_Shoes(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwShoes);
	return bRet;
}

/* [2020/03/01 13:05]-[Remark: None] */
/* [移动背包指定名字的物品到装备护甲]-[成功返回真，否则返回假] */
BOOL TBackpackList::MoveGoodsToEquip_Armor(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwArmor);
	return bRet;
}

/* [2020/03/01 13:06]-[Remark: None] */
/* [移动背包指定名字的物品到装备项链]-[成功返回真，否则返回假] */
BOOL TBackpackList::MoveGoodsToEquip_Neck(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwNeck);
	return bRet;
}

/* [2020/03/01 13:06]-[Remark: None] */
/* [移动背包指定名字的物品到装备左耳环]-[成功返回真，否则返回假] */
BOOL TBackpackList::MoveGoodsToEquip_EarL(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwEarL);
	return bRet;
}

/* [2020/03/01 13:06]-[Remark: None] */
/* [移动背包指定名字的物品到装备右耳环]-[成功返回真，否则返回假] */
BOOL TBackpackList::MoveGoodsToEquip_EarR(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwEarR);
	return bRet;
}

/* [2020/03/01 13:06]-[Remark: None] */
/* [移动背包指定名字的物品到装备左戒指]-[成功返回真，否则返回假] */
BOOL TBackpackList::MoveGoodsToEquip_RingL(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwRingL);
	return bRet;
}

/* [2020/03/01 13:06]-[Remark: None] */
/* [移动背包指定名字的物品到装备右戒指]-[成功返回真，否则返回假] */
BOOL TBackpackList::MoveGoodsToEquip_RingR(char* szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToEquip(g_tBackList.dwRingR);
	return bRet;
}

TDepotList* TDepotList::GetData()
{
	memset(tlist, 0, sizeof(TDepotList));
	__try
	{
		DWORD dwBase = *(DWORD *)Base_DepotList;
		DWORD dwFirstGoodsBase = dwBase + 0x43C;
		DWORD dwObj = NULL;
		for (int i = 0; i < 60; i++)
		{
			dwObj = *(DWORD *)(dwFirstGoodsBase + 4 * i);	//取出第i格对象地址
			tlist[i].dwGoodsObj = dwObj;
			if (tlist[i].dwGoodsObj == NULL)	continue;
			tlist[i].szName = (char *)(dwObj + 0x64);
			tlist[i].szMsg = (char *)(dwObj + 0x0F9);
			tlist[i].szAttribute = (char *)(dwObj + 0x238);
			tlist[i].nwNum = *(WORD *)(dwObj + 0xC4C);

			tlist[i].dwType1 = *(DWORD*)(dwObj + 0x54);
			tlist[i].nqID2 = *(QWORD*)(dwObj + 0x5C);
			tlist[i].byIndex = *(BYTE*)(dwObj + 0x1FC);
		}
	}
	__except (1)
	{
		DbgOutput("TDepotList* TDepotList::GetData() 异常\n");
		return NULL;
	}
	return this;
}

void TDepotList::PrintMsg()
{
	GetData();
	for (int i = 0; i < 60; i++)
	{
		if (tlist[i].dwGoodsObj == NULL)
		{
			continue;
		}
		DbgOutput("物品名（%s）:物品数量=%d,物品类型=%X\n,物品ID2=%X\n",
			tlist[i].szName,
			tlist[i].nwNum,
			tlist[i].dwType1,
			tlist[i].nqID2
		);
	}
}

/* [2020/03/01 13:07]-[Remark: None] */
/* [根据名字查找仓库内物品，获取物品所在仓库列表的下标]-[成功返回物品所在仓库列表的下标，失败返回-1] */
int TDepotList::GetGoodsIndexByName(char * szName)
{
	GetData();
	for (int i = 0; i < 60; i++)
	{
		if (tlist[i].dwGoodsObj == NULL)	continue;
		if (strcmp(szName, tlist[i].szName) == 0)
		{
			return i;
		}
	}
	return -1;
}

/* [2020/03/01 13:07]-[Remark: None] */
/* [查询仓库物品数量]-[成功返回真，否则返回假] */
DWORD TDepotList::GetGoodsNumByName(char * szName)
{
	for (int i = 0; i < 60; i++)
	{
		if (tlist[i].dwGoodsObj == NULL)	continue;
		if (strcmp(szName, tlist[i].szName) == 0)
		{
			return tlist[i].nwNum;
		}
	}
	return 0;
}

/* [2020/03/01 13:07]-[Remark: 只是让选中对象变为物品对象，但并不会拖起这个物品] */
/* [选中仓库指定格的物品]-[成功返回物品对象，失败返回NULL] */
DWORD TDepotList::SelectGoods(DWORD dwIndex)
{
	DWORD dwObj = NULL;
	__try
	{
		__asm
		{
			mov ebx, dwIndex
			mov edi, dword ptr ds : [Base_DepotList]
			mov edi, dword ptr ds : [edi + 0x43C + ebx * 4]
			mov dwObj, edi
			mov eax, dword ptr ds : [Base_MouseSelObj]
			mov dword ptr ds : [eax + 0x230], edi
		}
	}
	__except (1)
	{
		DbgOutput("DWORD TDepotList::SelectGoods(DWORD dwIndex) 异常\n");
		return NULL;
	}
	DbgOutput("选中对象dwObj = %X\n", dwObj);
	return dwObj;
}

/* [2020/03/01 13:08]-[Remark: None] */
/* [移动仓库已选中物品到背包]-[成功返回真，否则返回假] */
BOOL TDepotList::MoveGoodsToBackPack()
{
	__try
	{
		__asm
		{
			mov edi, dword ptr ds : [Base_BackPackList]
			mov ecx, dword ptr ds : [edi + 0x1634]
			mov edx, dword ptr ds : [edi + 0x1C60]
			mov eax, 1
			push eax
			push ecx
			push edx

			mov ecx, edi
			mov eax, Call_MoveObjToList
			call eax
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TDepotList::MoveGoodsToBackPack() 异常\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:08]-[Remark: None] */
/* [移动仓库指定名字的物品到背包]-[成功返回真，否则返回假] */
BOOL TDepotList::MoveGoodsToBackpack(char * szName)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	DWORD dwObj = SelectGoods((DWORD)iIndex);
	if (dwObj == NULL)	return FALSE;
	BOOL bRet = MoveGoodsToBackPack();
	return bRet;
}

/* [2020/03/01 13:08]-[Remark: None] */
/* [移动仓库指定名字的物品到背包,需指定数量]-[成功返回真，否则返回假] */
BOOL TDepotList::MoveGoodsToBackpack(char * szName, WORD nwIndex)
{
	int iIndex = GetGoodsIndexByName(szName);
	if (iIndex == -1)	return FALSE;
	BYTE byData[] = {
		0x00,0x00,0x94,0x00,0x8C,0x00,0x01,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x68,0xCA,0x9A,0x3B,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x53,0x84,0x0D,0x00,0x00,0x00,0x00,0x00,0xAD,0x11,0x0F,0x1D,0x1C,0x22,
		0xC4,0x06,0x68,0xCA,0x9A,0x3B,0x00,0x00,0x00,0x00,0x85,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x08,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
	TStorePickDepotData* pTStorePickDepotData = (TStorePickDepotData*)byData;
	__try
	{
		pTStorePickDepotData->dwType1 = tlist[iIndex].dwType1;
		pTStorePickDepotData->nwSaveNum = nwIndex;
		pTStorePickDepotData->nqID2 = tlist[iIndex].nqID2;	//注意：高位字节要在高位
		pTStorePickDepotData->dwType3 = tlist[iIndex].dwType1;
		pTStorePickDepotData->nwGoodsNum = tlist[iIndex].nwNum;
		pTStorePickDepotData->byIndex = tlist[iIndex].byIndex;
		for (int i = 0; i < 0xA; i++)
		{
			DbgOutput("%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
				byData[i * 0x10 + 0x0], byData[i * 0x10 + 0x1], byData[i * 0x10 + 0x2], byData[i * 0x10 + 0x3],
				byData[i * 0x10 + 0x4], byData[i * 0x10 + 0x5], byData[i * 0x10 + 0x6], byData[i * 0x10 + 0x7],
				byData[i * 0x10 + 0x8], byData[i * 0x10 + 0x9], byData[i * 0x10 + 0xA], byData[i * 0x10 + 0xB],
				byData[i * 0x10 + 0xC], byData[i * 0x10 + 0xD], byData[i * 0x10 + 0xE], byData[i * 0x10 + 0xF]
			);
		}
		WSASendData(pTStorePickDepotData, 0x8E);
	}
	__except (1)
	{
		DbgOutput("BOOL TDepotList::MoveGoodsToBackpack(char * szName, WORD nwIndex) 异常\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/04/13 18:33]-[Remark: 商店列表首地址: [[dwShopBase]+4]+0x43C+4*0    ] */
/* [获取商店列表基址]-[Return:None] */
DWORD TShopList::GetShopBase()
{
	if (dwShopBase != NULL)	return dwShopBase;	// 之前已经找到商店列表首地址
	// 商店列表首地址：[[Base_ShopList+4D2*4+4]+4]+0x43C+4*0,商店列表基址每次进游戏可能不一样,但在 0x4D2 附近
	DWORD dwBase = NULL;
	DWORD dwObj = NULL;

	for (DWORD i = 0x4D2 - 0x100; i < 0x4D2 + 0x100; ++i)
	{
		__try
		{
			DbgOutput("i = %X\n", i);
			dwBase = Base_ShopList + 4 * i;
			dwObj = *(DWORD*)dwBase;
			if (dwObj == NULL)	continue;
			dwObj = *(DWORD*)(dwObj + 4);
			if (dwObj == NULL)	continue;
			dwObj = *(DWORD*)(dwObj + 0x43C + 4 * 0);	// 取商店列表的第一格
			if (dwObj == NULL)	continue;

			DWORD dwType = *(DWORD*)(dwObj + offset_Obj_Type);
			if (dwType == Type_GoodsObj)
			{
				WORD nwNum = *(WORD*)(dwObj + 0xC4C);
				if (nwNum == 0xFFFF)
				{
					dwShopBase = dwBase;
					DbgOutput("获取商店列表基址成功,商店列表首地址: %X\n", dwShopBase);
					return dwShopBase;
				}
			}
		}
		__except (1)
		{
			continue;
		}
	}
	DbgOutput("获取商店列表基址失败\n");
	return NULL;
}

TShopList* TShopList::GetData()
{
	memset(tlist, 0, sizeof(TShopList));
	GetShopBase();
	__try
	{
		DWORD dwObj = NULL;
		for (int i = 0; i < 60; i++)
		{
			//取出第i格对象地址
			dwObj = *(DWORD *)(dwShopBase);	
			dwObj = *(DWORD *)(dwObj + 4);
			dwObj = *(DWORD *)(dwObj + 0x43C + 4*i);

			tlist[i].dwGoodsObj = dwObj;
			if (tlist[i].dwGoodsObj == NULL)	return this;	//商店物品是连续存放的
			tlist[i].szName = (char *)(dwObj + 0x64);
			tlist[i].szMsg = (char *)(dwObj + 0x0F9);
			tlist[i].szAttribute = (char *)(dwObj + 0x238);
			tlist[i].nwNum = *(WORD *)(dwObj + 0xC4C);

			tlist[i].dwType1 = *(DWORD*)(dwObj + 0x54);
			tlist[i].nqID2 = *(QWORD*)(dwObj + 0x5C);
			tlist[i].byIndex = *(BYTE*)(dwObj + 0x1FC);
		}
	}
	__except (1)
	{
		DbgOutput("TShopList* TShopList::GetData() 异常\n");
		return NULL;
	}
	return this;
}

void TShopList::PrintMsg()
{
	GetData();
	for (int i = 0; i < 60; i++)
	{
		if (tlist[i].dwGoodsObj == NULL)
		{
			return;
		}
		DbgOutput("物品名（%s）:物品类型=%X\n,物品ID2=%X\n",
			tlist[i].szName,
			tlist[i].dwType1,
			tlist[i].nqID2
		);
	}
}

/* [2020/03/01 13:09]-[Remark: None] */
/* [根据名字查找商店内物品]-[成功返回物品所在商店列表的下标，失败返回-1] */
int TShopList::GetGoodsIndexByName(char* szName)
{
	GetData();
	for (int i = 0; i < 60; i++)
	{
		if (tlist[i].dwGoodsObj == NULL)	return -1;
		if (strcmp(szName, tlist[i].szName) == 0)
		{
			return i;
		}
	}
	return -1;
}

/* [2020/03/01 13:09]-[Remark: None] */
/* [通过物品名字，购买指定数量的物品]-[成功返回真，否则返回假] */
BOOL TShopList::BuyGoodsByName(char* szName, WORD nwIndex)
{
	int iIndex = GetGoodsIndexByName(szName);
	DbgOutput("物品下标为%d\n",iIndex);
	if (iIndex < 0)	return FALSE;
	BYTE byData[] = {
		0x00,0x00,0x92,0x00,0x88,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x65,0xCA,
		0x9A,0x3B,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	};
	TBuySellData* pTBuySellData = (TBuySellData*)byData;
	__try
	{
		pTBuySellData->dwCmd = 0x00880092;
		pTBuySellData->dwBuySellType = BuyGoods;
		pTBuySellData->dwType1 = tlist[iIndex].dwType1;
		pTBuySellData->nwGoodsNum1 = nwIndex;
		WSASendData(pTBuySellData, 0x8E);
	}
	__except (1)
	{
		DbgOutput("BOOL TShopList::BuyGoodsByName(char* szName, WORD nwIndex) 异常\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:09]-[Remark: None] */
/* [通过物品名字，卖出指定数量的物品]-[成功返回真，否则返回假] */
BOOL TShopList::SellGoodsByName(char * szName, WORD nwIndex)
{
	int iIndex = g_tBackList.GetGoodsIndexByName(szName);
	DbgOutput("物品在背包列表下标为%d\n", iIndex);
	if (iIndex < 0)	return FALSE;
	BYTE byData[] = {
		0x00,0x00,0x92,0x00,0x88,0x00,0x02,0x00,0x00,0x00,0x14,0x56,0x1B,0xEB,0x65,0xCA,
		0x9A,0x3B,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x71,
		0x0D,0x00,0x00,0x00,0x00,0x00,0x45,0xB4,0x87,0x55,0xAB,0x0D,0xC4,0x06,0x65,0xCA,
		0x9A,0x3B,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,
		0x00,0x00,0x01,0x00,0x16,0xEC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEC,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x17,0xEC,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x52,0x17,0xEC,0x00,0x00,0x00,0x00,0x00,0x00,
		0x17,0xEC,0x0D,0x52,0x18,0xEC,0x0D,0x52,0x18,0xEC,0x0E,0x53,0x19,0xEC,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	};
	TBuySellData* pTBuySellData = (TBuySellData*)byData;
	__try
	{
		pTBuySellData->dwCmd = 0x00880092;
		pTBuySellData->dwBuySellType = SellGoods;
		pTBuySellData->dwType1 = g_tBackList.tlist[iIndex].dwType1;
		pTBuySellData->nwGoodsNum1 = nwIndex;
		pTBuySellData->nqID2 = g_tBackList.tlist[iIndex].nqID2;
		pTBuySellData->dwType2 = g_tBackList.tlist[iIndex].dwType1;
		pTBuySellData->nwGoodsNum2 = nwIndex;
		pTBuySellData->byIndex = g_tBackList.tlist[iIndex].byIndex;
		WSASendData(pTBuySellData, 0x8E);
	}
	__except (1)
	{
		DbgOutput("BOOL TShopList::SellGoodsByName(char * szName, WORD nwIndex) 异常\n");
		return FALSE;
	}
	return TRUE;
}


TNearList* TNearList::GetData() {
	memset(tlist, 0, sizeof(tlist));
	__try
	{
		DWORD dwObj = NULL;
		for (int i = 0; i< Size_NearList; i++)
		{
			dwObj = *(DWORD*)(Base_NearObjList + 4 * i);
			tlist[i].dwNearObj = dwObj;
			if (dwObj == NULL )	//周围对象不存在
			{
				continue; 
			}

			tlist[i].dwType = *(DWORD*)(dwObj + 0x8);
			tlist[i].dwIndexInAllObj = *(DWORD*)(dwObj + 0xC);
			g_tPlayerObj.GetData();
			
			if (tlist[i].dwType == Type_Monster)
			{
				tlist[i].szName = (char*)(dwObj + 0x360);	//NPC或怪物名称
			}
			else
			{
				tlist[i].szName = (char*)(dwObj + 0x18);	//玩家名字
				continue;
			}
			//NPC或怪物专属属性
			tlist[i].IsSelected = *(DWORD *)(dwObj + 0x354);
			tlist[i].flDistance = *(float*)(dwObj + 0x35C);
			tlist[i].IsDead = *(BYTE *)(dwObj + 0x3C0);
			tlist[i].IsAbleKill = *(BYTE *)(dwObj + 0x3C8);
			tlist[i].dwHP = *(DWORD *)(dwObj + 0x5F4);
			tlist[i].dwLevel = *(DWORD *)(dwObj + 0x5F8);
			tlist[i].flX = *(float *)(dwObj + 0x1060);
			tlist[i].flY = *(float *)(dwObj + 0x1068);
		}
	}
	__except (1)
	{
		DbgOutput("TNearList* TNearList::GetData() 异常\n");
		return NULL;
	}
	return this;
}

void TNearList::PrintMsg() {
	GetData();
	for (int i=0;i< Size_NearList;i++)
	{
		if (tlist[i].dwNearObj == NULL)
		{
			continue;
		}
		if (tlist[i].dwType != Type_Monster)
			DbgOutput("%s:对象=%X,ID=%X", tlist[i].szName, tlist[i].dwNearObj, tlist[i].dwIndexInAllObj);
		else
			DbgOutput("%s:对象=%X,ID=%X,等级=%d,HP=%d,坐标=(%f,%f),IsDead=%d,IsAbleKill=%d,IsSelected=%d，flDistance=%f\n",
						tlist[i].szName,
						tlist[i].dwNearObj,
						tlist[i].dwIndexInAllObj,
						tlist[i].dwLevel,
						tlist[i].dwHP,
						tlist[i].flX,
						tlist[i].flY,
						tlist[i].IsDead,
						tlist[i].IsAbleKill,
						tlist[i].IsSelected,
						tlist[i].flDistance);
	}
}

/* [2020/03/01 13:11]-[Remark: 如1CA4] */
/* [根据NPC名字，获得NPC的ID]-[成功返回真，否则返回假] */
DWORD TNearList::GetNpcID(char* szName)
{
	GetData();	//初始化周围对象列表
	DbgOutput("传入的NPC名字：%s\n", szName);
	for (int i = 0; i < Size_NearList; i++)
	{
		if (tlist[i].dwNearObj == NULL)
		{
			break;
		}
		DbgOutput("%d,名称=%s\n", i, tlist[i].szName);
		if ( strcmp(tlist[i].szName, szName) == 0 )
		{
			DbgOutput("名称匹配成功\n");
			DWORD dwID = tlist[i].dwIndexInAllObj;
			DbgOutput("NPC的ID=%d\n", dwID);
			return dwID;
		}
	}
	return NULL;
}

/* [2020/03/01 13:11]-[Remark: None] */
/* [根据NPC名字，与指定NPC对话]-[成功返回真，否则返回假] */
BOOL TNearList::ChatWithNPC(char* szName)
{
	DWORD dwIndex = GetNpcID(szName);
	DbgOutput("ChatWithNPC NPC的ID=%d\n", dwIndex);
	if (dwIndex == NULL)	return FALSE;
	g_tPlayerObj.SelObj(dwIndex);
	__try
	{
		__asm
		{
			mov eax, dwIndex
			mov ecx, dword ptr ds : [eax * 4 + Base_AllObjList]
			mov edx, dword ptr ds : [ecx]
			mov eax, dword ptr ds : [edx + 0x4]
			push 0
			push 0
			push 0x401
			call eax
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TNearList::ChatWithNPC(char* szName) 异常\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:12]-[Remark: 5打开仓库，4确认任务，3打开商店] */
/* [点击对话框菜单选项]-[成功返回真，否则返回假] */
BOOL TNearList::ClickChatMenuOption(DWORD dwIndex)
{
	__try
	{
		__asm
		{
			mov ecx, dword ptr ds : [Base_Unknown]
			mov ecx, dword ptr ds : [ecx + 0x2EC]

			push dwIndex
			mov eax, Call_ClickChatMenuOption
			call eax
		}
	}
	__except (1)
	{
		DbgOutput("BOOL ClickChatMenuOption(DWORD dwIndex) 异常\n");
		return FALSE;
	}
	return TRUE;
}

TActionList* TActionList::GetData() {
	DWORD dwFirstObjAddr = NULL;	//第一个对象的地址
	DWORD dwObj = NULL;
	__try
	{
		dwFirstObjAddr = *(DWORD*)(Base_ActionList) + 0x43C;
		for (int i=0;i<12;i++)
		{
			dwObj = *(DWORD *)(dwFirstObjAddr + 4 * i);
			tlist[i].dwActionID = *(DWORD*)(dwObj + 0x54);
			tlist[i].szName = (char *)(dwObj + 0x64);
		}
	}
	__except (1)
	{
		DbgOutput("TActionList* TActionList::GetData() 异常\n");
	}
	return this;
}

/* [2020/03/01 13:12]-[Remark: None] */
/* [结束与NPC对话]-[成功返回真，否则返回假] */
BOOL TNearList::EndChatWithNPC(char* szNpcName)
{
	DWORD byData[0x24] = { 0 };	//0x24 * 4 = 0x90全部初始化为0
	byData[0] = 0x00900000;
	byData[1] = 0x00020018;
	if (strcmp(szNpcName, "韦大宝") == 0)
	{
		byData[3] = 0x00010000;
	}
	else if (strcmp(szNpcName, "刀剑笑") == 0)
	{
		byData[3] = 0x00020000;
	}
	else if (strcmp(szNpcName, "平十指") == 0)
	{
		byData[3] = 0x00030000;
	}
	BOOL bRet = WSASendData(byData, 0x1E);
	return bRet;
}

/* [2020/03/01 13:12]-[Remark: None] */
/* [打开仓库(韦大宝),买进/卖出(平十指,刀剑笑)]-[成功返回真，否则返回假] */
BOOL TNearList::OpenShop(char* szNpcName)
{
	DWORD byData[0x24] = { 0 };	//0x24 * 4 = 0x90全部初始化为0
	byData[0] = 0x00900000;
	byData[5] = 0x00010000;
	if (strcmp(szNpcName,"韦大宝") == 0)
	{
		byData[1] = 0x00050018;
		byData[3] = 0x00010000;
	}
	else if (strcmp(szNpcName, "刀剑笑") == 0)
	{
		byData[1] = 0x00030018;
		byData[3] = 0x00020000;
	}
	else if (strcmp(szNpcName, "平十指") == 0)
	{
		byData[1] = 0x00030018;
		byData[3] = 0x00030000;
	}
	BOOL bRet = WSASendData(byData, 0x1E);
	return bRet;
}

void TActionList::PrintMsg() {
	GetData();
	for (int i = 0; i < 12; i++)
	{
		DbgOutput("%s[%d] ID=%d\n", tlist[i].szName, i, tlist[i].dwActionID);
	}
}

/* [2020/03/01 13:13]-[Remark: None] */
/* [根据动作列表的下标使用动作]-[成功返回真，否则返回假] */
BOOL TActionList::UseActionByIndex(DWORD dwIndex) {
	GetData();
	DWORD dwEcx = Call_ActionUse_Ecx;
	DWORD dwArg1 = tlist[dwIndex].dwActionID;
	__try
	{
		__asm {
			mov ecx, dwArg1	//取 动作ID 到 ecx
			push ecx		//把 动作ID 入栈
			mov ecx, dwEcx
			mov edx, Call_ActionUse
			call edx
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TActionList::UseAction(DWORD dwIndex) 异常\n");
	}
	return TRUE;
}

/* [2020/03/01 13:13]-[Remark: None] */
/* [根据动作名使用动作，如：“攻击”]-[成功返回真，否则返回假] */
BOOL TActionList::UseActionByName(char * szActionName) {
	GetData();
	for (int i =0;i<12;i++)
	{
		//DbgOutput(tlist[i].szName);
		if (strcmp(tlist[i].szName, szActionName) == 0)
		{
			UseActionByIndex(i);
			return TRUE;
		}
	}
	return FALSE;
}

TPlayerObj* TPlayerObj::GetData() {
	__try
	{
		DWORD dwObj = *(DWORD*)Base_PlayerObj;
		dwSelIndex = *(DWORD*)(dwObj + offset_PlayerObj_dwSelIndex);
		flDesX = *(float*)(dwObj + offset_PlayerObj_flDesX);
		flDesY = *(float*)(dwObj + offset_PlayerObj_flDesX + 8);
		flCurX = *(float*)(dwObj + offset_PlayerObj_flCurX);
		flCurY = *(float*)(dwObj + offset_PlayerObj_flCurX + 8);
	}
	__except (1)
	{
		DbgOutput("TPlayerObj* TPlayerObj::GetData() 异常\n");
		return NULL;
	}
	return this;
}

void TPlayerObj::PrintMsg() {
	GetData();
	DbgOutput("玩家选中对象ID=%X,玩家当前坐标(X:%f,Y:%f),玩家目的坐标(X:%f,Y:%f)", dwSelIndex,
				flCurX,flCurY,flDesX,flDesY);
}

/* [2020/03/01 13:13]-[Remark: None] */
/* [选中周围对象]-[成功返回真，否则返回假] */
BOOL TPlayerObj::SelObj(DWORD dwIndexInAllObj)
{
	GetData();
	DWORD dwObjRole;
	__try 
	{
		__asm {
			//取选中怪物ID
			mov edi, dword ptr ds : [Base_PlayerObj]
			mov eax, dword ptr ds : [edi + offset_PlayerObj_dwSelIndex]
			cmp eax, 0xFFFF       //是否选中怪物
			jz NEXTLABEL  //eax==0xFF,意味着没选怪,直接跳到NEXTLABEL
			//获得选中对象
			mov ecx, dword ptr ds : [Base_AllObjList + eax * 4]
			mov edx, dword ptr ds : [ecx]
			mov eax, dword ptr ds : [edx + 0x04]
			push 0
			push 0 //取消选中
			push 0x450
			call eax
		}
	NEXTLABEL:
		//把新的玩家选中怪物的ID 传给 [ [Base_PlayerObj]+offset_PlayerObj_dwSelIndex ]
		dwObjRole = *(DWORD*)Base_PlayerObj;
		*(DWORD*)(dwObjRole + offset_PlayerObj_dwSelIndex) = dwIndexInAllObj;       //传参，必须在ASM外面
		__asm {
			//上面的寄存器在语句块结束时被回收，无法继续生效，需要再赋值
			mov edi, dword ptr ds : [Base_PlayerObj]
			mov eax, dword ptr ds : [edi + offset_PlayerObj_dwSelIndex]    //eax=新选中怪物ID
			//取出新对象
			mov ecx, dword ptr ds : [Base_AllObjList + eax * 4]
			mov edx, dword ptr ds : [ecx]
			mov eax, dword ptr ds : [edx + 0x04]
			push 0
			push 1 //显示选中
			push 0x450
			call eax
		}
	}
	__except(1) 
	{
		DbgOutput("BOOL TPlayerObj::SelObj(DWORD dwIndexInAllObj) 异常\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:13]-[Remark: None] */
/* [取消选中周围对象]-[成功返回真，否则返回假] */
BOOL TPlayerObj::CancelSel()
{
	DWORD dwObjRole = NULL;
	__try
	{
		dwObjRole = *(DWORD*)Base_PlayerObj;
		*(DWORD*)(dwObjRole + offset_PlayerObj_dwSelIndex) = 0xFFFF;
	}
	__except(1)
	{
		DbgOutput("BOOL TPlayerObj::CancelSel() 异常\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:14]-[Remark: None] */
/* [选中离玩家最近的怪物]-[成功返回真，否则返回假] */
BOOL TPlayerObj::SelNearMonster()
{
	GetData();
	DbgOutput("进入SelNearMonster\n");
	DWORD dwIndex = 0xFFFF;
	float flMinDistance = 9999.9F;
	g_tNearList.GetData();
	for (int i=0;i< Size_NearList;i++)	//遍历整个周围列表，找到距离最近的周围对象的ID
	{
		if (g_tNearList.tlist[i].dwNearObj == NULL)
		{
			continue;
		}
		if (g_tNearList.tlist[i].dwLevel == 0)	//过滤NPC
		{
			continue;
		}
		if (g_tNearList.tlist[i].dwType != Type_Monster)	//过滤玩家,宠物,地面物品等
		{
			continue;
		}
		if (g_tNearList.tlist[i].flDistance < flMinDistance)
		{
			flMinDistance = g_tNearList.tlist[i].flDistance;	
			dwIndex = g_tNearList.tlist[i].dwIndexInAllObj;	//令dwIndex = 距离最近的周围对象ID
			DbgOutput("怪物ID：%X，怪物距离：%f\n",dwIndex, flMinDistance);
		}
	}
	if (dwIndex == 0xFFFF)
	{
		return FALSE;
	}
	BOOL bRet = 0;
	bRet = SelObj(dwIndex);
	DbgOutput("SelNearMonster返回值：%d\n", bRet);
	return bRet;
}

/* [2020/04/12 11:01]-[Remark: None] */
/* [根据怪物名选择最近的怪物]-[Return:None] */
BOOL TPlayerObj::SelNearMonster(const char* szMonName)
{
	if (szMonName == NULL)	return SelNearMonster();

	GetData();
	DbgOutput("进入SelNearMonster(const char* szMonName)\n");
	DWORD dwIndex = 0xFFFF;
	float flMinDistance = 9999.9F;
	g_tNearList.GetData();
	for (int i = 0; i < Size_NearList; i++)	//遍历整个周围列表，找到距离最近的周围对象的ID
	{
		if (g_tNearList.tlist[i].dwNearObj == NULL)
		{
			continue;
		}
		if (g_tNearList.tlist[i].dwLevel == 0)	//过滤NPC
		{
			continue;
		}
		if (g_tNearList.tlist[i].dwType != Type_Monster)	//过滤玩家,宠物,地面物品等
		{
			continue;
		}
		if (strcmp(g_tNearList.tlist[i].szName, szMonName) == 0)
		{
			DbgOutput("找到怪物名:%s\n", szMonName);
			if (g_tNearList.tlist[i].flDistance < flMinDistance)
			{
				flMinDistance = g_tNearList.tlist[i].flDistance;
				dwIndex = g_tNearList.tlist[i].dwIndexInAllObj;	//令dwIndex = 距离最近的周围对象ID
				DbgOutput("怪物ID：%X，怪物距离：%f\n", dwIndex, flMinDistance);
			}
		}
		
	}
	if (dwIndex == 0xFFFF)
	{
		return FALSE;
	}
	BOOL bRet = 0;
	bRet = SelObj(dwIndex);
	DbgOutput("SelNearMonster返回值：%d\n", bRet);
	return bRet;
}

/* [2020/03/01 13:15]-[Remark: None] */
/* [自动普攻打怪]-[成功返回真，否则返回假] */
BOOL TPlayerObj::AutoBeatMonster()
{
	BOOL bRet = 1;
	
	if (GetSelObjType() != Type_Monster)	//判断当前是否选中怪物,没选中就选最近的怪
	{
		bRet = SelNearMonster();
	}
	
	if (bRet)	//选了就直接攻击
	{
		bRet = g_tActList.UseActionByName("攻击");	//使用普攻
		if (bRet)	return TRUE;
	}
	return FALSE;
}

/* [2020/03/01 13:15]-[Remark: None] */
/* [自动使用技能打怪]-[成功返回真，否则返回假] */
BOOL TPlayerObj::AutoBeatMonsterBySkill(char * szSkillName)
{
	BOOL bRet = SelNearMonster();
	if (bRet)	//选了就直接攻击
	{
		if (g_tPlayerObj.IsSelObjAbleKill())
			bRet = g_tScList.UseSkillByName("天魔贯穿");	//使用必杀技
		else
			bRet = g_tScList.UseSkillByName(szSkillName);	//使用技能
		DbgOutput("使用技能结果：%d", bRet);
		if (bRet)	return TRUE;
		else
		{
			bRet = g_tActList.UseActionByName("攻击");	//使用普攻
			if (bRet)	return TRUE;
		}
	}
	return FALSE;
}

/* [2020/03/01 13:16]-[Remark: None] */
/* [获取玩家选中对象的类型]-[成功返回对象类型，失败则返回NULL] */
DWORD TPlayerObj::GetSelObjType()
{
	GetData();
	DWORD dwType = NULL;
	DWORD dwCurObj = NULL;
	if (dwSelIndex == 0xFFFF)
	{
		return NULL;
	}
	__try
	{
		//根据 当前选中ID 取得 选中对象
		dwCurObj = *(DWORD*)(Base_AllObjList + 4 * dwSelIndex);
		dwType = *(DWORD*)(dwCurObj + 0x8);
		DbgOutput("选中对象ID：%X，选中对象：%X，类型：%X\n", dwSelIndex, dwCurObj, dwType);
	}
	__except (1)
	{
		DbgOutput("DWORD TPlayerObj::GetSelObjType() 异常\n");
		return NULL;
	}
	return dwType;
}

BOOL TPlayerObj::IsSelObjDead()
{
	GetData();
	BOOL isDead = FALSE;
	DWORD dwCurObj = NULL;
	if (dwSelIndex == 0xFFFF)
	{
		return FALSE;
	}
	__try
	{
		//根据 当前选中ID 取得 选中对象
		dwCurObj = *(DWORD*)(Base_AllObjList + 4 * dwSelIndex);
		isDead = *(BYTE*)(dwCurObj + 0x3C0);
		DbgOutput("选中对象ID：%X，选中对象：%X，死亡：%d\n", dwSelIndex, dwCurObj, isDead);
	}
	__except (1)
	{
		DbgOutput("BOOL TPlayerObj::IsSelObjDead() 异常\n");
		return NULL;
	}
	return isDead;
}

/* [2020/04/14 09:54]-[Remark: None] */
/* [选择的怪物是否可被必杀]-[Return:None] */
BOOL TPlayerObj::IsSelObjAbleKill()
{
	GetData();
	g_tNearList.GetData();
	if (dwSelIndex == 0xFFFF)	return FALSE;
	DWORD indexList = 0;
	for(DWORD i = 0; i < Size_NearList; ++i)
	{
		if (g_tNearList.tlist[i].dwIndexInAllObj == dwSelIndex)
		{
			indexList = i;
		}
	}
	BYTE byRet = g_tNearList.tlist[indexList].IsAbleKill;
	if (byRet)
		DbgOutput("怪物可被必杀,byRet=%d,dwSelIndex=%X,indexList=%d\n",byRet, dwSelIndex, indexList);
	else
		DbgOutput("怪物不可被必杀\n");
	return byRet;
}

/* [2020/03/01 13:16]-[Remark: 看到103课时更改] */
/* [跑到指定坐标]-[成功返回真，否则返回假] */
BOOL TPlayerObj::RunToXY(float flx, float fly)
{
	__try
	{
		__asm 
		{
			//显示红点寻路标记: [[[Base_Unknown]+2B0]+230] = 1
			mov eax, Base_Unknown
			mov eax, [eax]
			mov eax, [eax + 0x2B0]
			mov byte ptr ds : [eax + 0x230], 1
			//寻路
			sub esp, 0x54
			mov eax, esp
			mov ecx, flx
			mov edx, fly
			mov dword ptr ss : [eax + 0x00], ecx
			mov dword ptr ss : [eax + 0x04], 0x00000000
			mov dword ptr ss : [eax + 0x08], edx
			mov dword ptr ss : [eax + 0x0C], 0x00000000
			mov dword ptr ss : [eax + 0x10], 0x00000000
			mov dword ptr ss : [eax + 0x14], 0x00000000
			mov dword ptr ss : [eax + 0x18], 0x0000FFFF
			mov dword ptr ss : [eax + 0x1C], 0x00000001
			mov dword ptr ss : [eax + 0x20], 0x00000001
			push 0x54
			push eax
			push 0x3EF
			mov ecx, dword ptr ds : [Base_PlayerObj]
			mov edx, dword ptr ds : [ecx]
			mov edx, dword ptr ds : [edx + 0x4]
			call edx
			add esp, 0x54
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TPlayerObj::RunToXY(float flx, float fly) 异常\n");
		return FALSE;
	}
	return TRUE;
}

TSkillList* TSkillList::GetData()
{
	memset(tlist, 0, sizeof(tlist));
	DWORD dwFirstSkillObjAddr = NULL; //第一个对象的地址
	DWORD dwObj = NULL;
	__try
	{
		dwFirstSkillObjAddr = *(DWORD*)(Base_SkillList)+0x43C;
		for (int i = 0; i < 32; i++)
		{
			tlist[i].dwIndexForSkill = i;
			dwObj = *(DWORD*)(dwFirstSkillObjAddr + 4 * i);
			tlist[i].dwSkillObj = dwObj;
			if (dwObj == NULL)   continue;
			tlist[i].dwType = *(DWORD*)(dwObj + 0x8);
			tlist[i].dwIndexForAll = *(DWORD*)(dwObj + 0xC);
			tlist[i].szName = (char*)(dwObj + 0x64);
			tlist[i].byLevelNeed = *(DWORD*)(dwObj + 0xB4);
			tlist[i].IsLearned = *(WORD*)(dwObj + 0x1FE);
			tlist[i].dwExpNeed = *(DWORD*)(dwObj + 0x270);
		}
	}
	__except (1)
	{
		DbgOutput("TSkillList * TSkillList::GetData() 异常\n");
		return NULL;
	}
	return this;
}

void TSkillList::PrintMsg()
{
	GetData();
	for (int i = 0; i < 32; i++)
	{
		if (tlist[i].dwSkillObj == NULL)	continue;
		DbgOutput("%s %d %2X %4X 是否已学（%d），需要等级（%d），需要历练（%d）\n",
			tlist[i].szName,
			tlist[i].dwIndexForSkill,
			tlist[i].dwType,
			tlist[i].dwIndexForAll,
			tlist[i].IsLearned,
			tlist[i].byLevelNeed,
			tlist[i].dwExpNeed);
	}
}

/* [2020/03/01 13:17]-[Remark: None] */
/* [根据技能名，把技能拖到快捷栏上的指定空位处]-[成功返回真，否则返回假] */
BOOL TSkillList::DropSkillToF1F10(char *szSkillName, DWORD dwIndexF1F10)
{
	GetData();
	DWORD dwSkiObj = NULL;
	__try
	{
		for (int i = 0; i <32; i++)	//先遍历技能列表，找到技能名对应的下标
		{
			if (tlist[i].dwSkillObj == NULL)	continue;
			DbgOutput("第%d个技能名：%s", i, tlist[i].szName);
			if (strcmp(tlist[i].szName, szSkillName)  == 0)
			{
				dwSkiObj = tlist[i].dwSkillObj;
				__asm
				{
					//写入对象 到 鼠标拖起的对象的基址
					mov eax,Base_MouseSelObj
					mov eax,[eax]
					mov ecx, dwSkiObj
					mov [eax+0x230], ecx
				}
				break;
			}
		}
		if (dwSkiObj == NULL)	//说明没找到相应的技能
		{
			return FALSE;
		}
		__asm
		{
			//把对象放入快捷栏
			mov edi, Base_ShortcutBar
			mov edi, [edi]
			mov ecx, dword ptr ds : [edi + 0x1634]
			mov edx, dword ptr ds : [edi + 0x1C60]
			mov eax, dwIndexF1F10
			push eax
			push ecx
			push edx
			mov ecx, edi
			mov eax, Call_MoveObjToList
			call eax
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TSkillList::DropSkillToF1F10(char *szSkillName, DWORD dwIndexF1F10) 异常\n");
	}
	return TRUE;
}

/* [2020/03/01 13:17]-[Remark: None] */
/* [根据技能名，把技能拖到快捷栏上的第一个空位处]-[成功则返回放置在快捷栏列表的下标，失败返回-1] */
int TSkillList::DropSkillToF1F10(char* szSkillName)
{
	//先检测技能名是否为空
	if (g_cAutoPlay.szSkillName == NULL || g_cAutoPlay.IsUseSkill == FALSE)
	{
		DbgOutput("未设置使用技能\n");
		return -1;
	}
	//先检测技能是否已经在快捷栏上
	int niIndex = g_tScList.GetIndexByName(szSkillName);
	if (niIndex >= 0)	return niIndex;
	//找空位放置技能
	niIndex = g_tScList.GetScNullIndex();
	if (niIndex == -1)
	{
		DbgOutput("快捷栏已经满了！\n");
		return -1;
	}
	BOOL bRet = DropSkillToF1F10(szSkillName, niIndex);
	if (bRet == TRUE)
	{
		return niIndex;
	}
	return -1;
}

/* [2020/03/01 13:18]-[Remark: None] */
/* [修炼技能，通过技能列表下标]-[成功返回真，否则返回假] */
BOOL TSkillList::LearnSkillByIndex(DWORD dwIndex)
{
	if (IsLearnable(dwIndex) == FALSE)	return FALSE;
	GetData();
	__try
	{
		__asm
		{
			mov edi, dword ptr ds : [Base_SkillList]
			mov edx, dwIndex
			mov edx, dword ptr ds : [edi + 0x43C + 4 * edx]
			mov ecx, dword ptr ds : [Call_LearnSkill_Ecx]
			mov eax, dword ptr ds : [edx + 0x54]
			push ecx
			push eax
			lea ecx, dword ptr ss : [ebp - 0x280C]
			push ecx
			mov ecx, dword ptr ds : [Base_PlayerObj]
			mov eax, Call_LearnSkill
			call eax
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TSkillList::LearnSkill(DWORD dwIndex) 异常\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:18]-[Remark: None] */
/* [修炼技能，通过技能名字]-[成功返回真，否则返回假] */
BOOL TSkillList::LearnSkillByName(char * szSkillName)
{
	GetData();
	for (int i=0;i<32;i++)	
	{
		if (tlist[i].dwSkillObj == NULL)	continue;
		DbgOutput("技能[%d] = %s\n", i, tlist[i].szName);
		if (strcmp(szSkillName,tlist[i].szName) == 0)
		{
			BOOL bRet = LearnSkillByIndex(i);
			return bRet;
		}
	}
	return FALSE;
}

/* [2020/03/01 13:18]-[Remark: 主要看：1.是否已学 2.是否达到等级 3.是否达到历练] */
/* [根据技能列表下标判断技能是否可学]-[可修炼返回真，不可修炼则返回假] */
BOOL TSkillList::IsLearnable(DWORD dwIndex)
{
	//初始化数据
	g_tRoleProperty.GetData();
	g_tSkiList.GetData();
	//技能对象是否存在
	if (g_tSkiList.tlist[dwIndex].dwSkillObj == NULL) 
	{
		DbgOutput("技能[%d] 技能对象不存在\n", dwIndex);
		return FALSE;
	}	
	//技能是否已学
	BOOL bIsLearned = g_tSkiList.tlist[dwIndex].IsLearned;		
	if (bIsLearned == TRUE)
	{
		DbgOutput("技能[%d]：%s 已经学习\n", dwIndex, tlist[dwIndex].szName);
		return FALSE;
	}
	//是否达到等级
	BOOL bNeedLevel = g_tSkiList.tlist[dwIndex].byLevelNeed;	//需要等级
	BOOL bRoleLevel = g_tRoleProperty.byLevel;					//角色等级
	if (bRoleLevel < bNeedLevel)
	{
		DbgOutput("技能[%d]：%s 尚未达到要求等级\n", dwIndex, tlist[dwIndex].szName);
		return FALSE;
	}
	//是否达到历练
	BOOL bNeedExp = g_tSkiList.tlist[dwIndex].dwExpNeed;		//需要历练
	BOOL bRoleExp = g_tRoleProperty.dw历练;						//角色历练
	if (bRoleExp < bNeedExp)
	{
		DbgOutput("技能[%d]：%s 尚未达到要求历练\n", dwIndex, tlist[dwIndex].szName);
		return FALSE;
	}
	DbgOutput("技能[%d]：%s 可以修炼\n", dwIndex, tlist[dwIndex].szName);
	return TRUE;
}

#define offset_ScObjType 0x08
#define offset_ScObjName 0x64
TShortcutList* TShortcutList::GetData()
{
	memset(tlist, 0, sizeof(tlist));
	DWORD dwFirstSCObjAddr = NULL;	//第一个对象的地址
	DWORD dwObj = NULL;
	__try
	{
		dwFirstSCObjAddr = *(DWORD*)(Base_ShortcutBar)+0x43C;
		for (int i=0;i<10;i++)
		{
			dwObj = *(DWORD*)(dwFirstSCObjAddr + 4 * i);	//没装东西则为0
			tlist[i].dwScObj = dwObj;
			if (dwObj == NULL)	continue;
			tlist[i].dwType = *(WORD*)(dwObj + offset_ScObjType);
			tlist[i].szName = (char*)(dwObj + offset_ScObjName);
		}
	}
	__except (1)
	{
		DbgOutput("TShortcutList * TShortcutList::GetData() 异常\n");
		return NULL;
	}
	return this;
}


void TShortcutList::PrintMsg()
{
	GetData();
	for (int i = 0; i < 10; i++)
	{
		DbgOutput("快捷栏%d:对象名称(%s),对象类型（%X）\n",i,
			tlist[i].szName,
			tlist[i].dwType);
	}
}

/* [2020/03/01 13:19]-[Remark: None] */
/* [根据技能名返回在快捷栏的下标]-[找到该技能则返回下标，没找到则返回-1] */
int TShortcutList::GetIndexByName(char * szSkillName)
{
	GetData();
	for (int i = 0; i < 10; i++)
	{
		if (tlist[i].dwScObj == NULL)	continue;
		if (strcmp(tlist[i].szName, szSkillName) == 0)	return i;
	}
	return -1;
}

/* [2020/03/01 13:19]-[Remark: None] */
/* [根据快捷栏的下标使用技能]-[成功返回真，否则返回假] */
BOOL TShortcutList::UseSkillByIndex(DWORD dwIndex)
{
	//DbgOutput("进入UseSkillByIndex(%d) \n", dwIndex);
	DWORD dwEcx = Call_F1F10Use_Ecx;
	__try
	{
		__asm
		{
			mov eax, dwIndex
			push eax
			mov ecx, dwEcx
			mov eax, Call_F1F10Use
			call eax
		}
	}
	__except (1)
	{
		DbgOutput("BOOL TShortcutList::UseSkillByIndex(DWORD dwIndex) 异常\n");
		return FALSE;
	}
	return TRUE;
}

/* [2020/03/01 13:20]-[Remark: None] */
/* [根据技能名字使用技能，快捷栏没有则拖到快捷栏后再使用]-[成功返回真，否则返回假] */
BOOL TShortcutList::UseSkillByName(char *szSkillName)
{
	//获取技能在快捷栏的下标
	int niIndex = g_tScList.GetIndexByName(szSkillName);
	if (niIndex == -1)	return FALSE;
	BOOL bRet = UseSkillByIndex(niIndex);
	return bRet;
}

/* [2020/03/01 13:20]-[Remark: None] */
/* [获得快捷栏第一个空位的下标]-[成功返回下标，没找到空位返回-1] */
BOOL TShortcutList::GetScNullIndex()
{
	GetData();
	for (int i=0;i<10;i++)
	{
		if (tlist[i].dwScObj == NULL)	return i;
	}
	return -1;
}

TGroundGoodsList * TGroundGoodsList::GetData()
{
	DWORD dwObj = NULL;
	memset(tlist, 0, sizeof(tlist));
	__try
	{
		int i = 0;
		for (int j=0;j< Size_NearList;j++)
		{
			dwObj = *(DWORD*)(Base_NearObjList + 4 * j);
			if(dwObj==NULL)	continue;
			if(*(DWORD*)(dwObj+offset_Obj_Type) != Type_GroundGoods)	continue;
			tlist[i].dwType = *(DWORD*)(dwObj + offset_Obj_Type);
			tlist[i].szName = (char*)(dwObj + offset_TGroundGoodsObj_szName);
			tlist[i].dwIndexInAllObj = *(DWORD*)(dwObj + offset_Obj_Index);
			i++;
		}
	}
	__except(1)
	{
		DbgOutput("TGroundGoodsList * TGroundGoodsList::GetData() 异常\n");
		return NULL;
	}
	return this;
}

void TGroundGoodsList::PrintMsg()
{
	GetData();
	for (int i = 0; i < Size_NearList; i++)
	{
		if (tlist[i].szName == NULL)	break;
		DbgOutput("%s,%X\n", tlist[i].szName, tlist[i].dwIndexInAllObj);
	}
}

/* [2020/03/01 13:20]-[Remark: None] */
/* [选中地上物品]-[成功返回真，否则返回假] */
BOOL TGroundGoodsList::SelGroundGoods()
{
	BOOL IsPickup = 1;	//设置为默认拾取该物品
	GetData();
	for (int i = 0; i < Size_NearList; i++)	//遍历地上物品列表
	{
		if (tlist[i].szName == NULL)	break;	//跳出遍历地上物品列表
		for (vector<TGoodsManage>::iterator it = g_cAutoPlay.vct_GoodsMngList.begin();
			it < g_cAutoPlay.vct_GoodsMngList.end(); it++)	//遍历物品管理列表,过滤不拾取的物品
		{
			if (strcmp(tlist[i].szName, it->szGoodsName) == 0) {
				if ((it->dwGoodsManageFlag) & Goods_Manage_NoPick) {
					IsPickup = 0;	//不拾取
				}
				else {
					IsPickup = 1;	//拾取
				}
				break;	//跳出遍历物品管理列表
			}
		}
		if (IsPickup)
		{
			DWORD dwPlayerObj = *(DWORD*)(Base_PlayerObj);
			*(DWORD*)(dwPlayerObj + offset_PlayerObj_dwSelIndex) = tlist[i].dwIndexInAllObj;	
			break;	//选中后跳出遍历地上物品列表
		}
	}
	return TRUE;
}

/* [2020/03/01 13:20]-[Remark: 拾取动作无法实现选择性捡物] */
/* [自动捡物]-[成功返回真，否则返回假] */
BOOL TGroundGoodsList::PickupGoods()
{
	BOOL bRet = SelGroundGoods();
	if (bRet)
	{
		bRet = g_tActList.UseActionByName("攻击");	//攻击动作可实现选择性捡物
		g_tBackList.UseGoodsByName("香木宝盒");
	}
	return bRet;
}
