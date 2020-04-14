#include "GameFunction.h"

uniform_int_distribution<int> rnd(-10, 10);//随机生成[-10,10] 均匀分布的整数
default_random_engine micro;	//生成随机数

/* [2020/02/18 13:22]-[Remark: None] */
/* [寻路到指定坐标才返回]-[成功返回真，否则返回假] */
BOOL FindWay(int x, int y)
{
	float flCurPosX;
	float flCurPosY;
	DWORD dwDistance = 0xFFFF;		//当前坐标 与 指定坐标的距离
	DWORD dwPastDistance = 0xFFFF;	//上一次距离,防卡时用
	DWORD dwWaitTime = 0;	//寻路过程耗费时间(秒)

	msg_RunToXY(x, y);
	while (g_cAutoPlay.LoopFlag)
	{
		Sleep(1000);
		dwWaitTime++;
		g_tPlayerObj.GetData();
		flCurPosX = g_tPlayerObj.flCurX;
		flCurPosY = g_tPlayerObj.flCurY;
		dwDistance = TwoPoint_Distance(flCurPosX, flCurPosY, (float)x, (float)y);
		DbgOutput("(%d,%d)寻路中...,还剩%d米", x, y, dwDistance);
		if (dwDistance == dwPastDistance)	//若这次的距离与上次距离相等,表示卡点
		{
			msg_RunToXY(x + rnd(micro), y + rnd(micro));	//再往另一个点跑
		}
		if (dwDistance < 20)		//到达则返回TRUE
		{
			DbgOutput("寻路成功!,耗时:%d 秒", dwWaitTime);
			return TRUE;
		}
		if (dwWaitTime > 6 * 60)	//6分钟还未到达则返回FALSE
		{
			DbgOutput("寻路失败!,耗时:%d 秒", dwWaitTime);
			return FALSE;
		}
		if (dwDistance >= 20)		//6分钟内 且 未到达则记录距离
		{
			dwPastDistance = dwDistance;	//记录这一次的距离,下一次循环获取距离后再比较二者
		}
	}
	return FALSE;
}

BOOL HookRefreshScreen(BOOL IsDisableRefresh)
{
	BYTE NewBytes[] = { 0x90,0x90,0x90 };
	BYTE OldBytes[] = { 0x50,0xFF,0xD2 };
	BOOL bRet = 0;
	if (IsDisableRefresh)
		bRet = WriteProcessMemory((HANDLE)-1, (DWORD*)Base_HookRefreshScreen, NewBytes, 3, NULL);
	else
		bRet = WriteProcessMemory((HANDLE)-1, (DWORD*)Base_HookRefreshScreen, OldBytes, 3, NULL);
	return bRet;
}

int* GetRoleXY(int* Pos)
{
	msg_GetRoleXY(Pos);
	return Pos;
}

void UseGoodsByName(const char * szGoodName)
{
	msg_UseGoodsByName(szGoodName);
}

void UseSkillByName(const char * szSkillName)
{
	msg_UseSkillByName(szSkillName);
}

void SelNearMonster(const char * szMonName)
{
	msg_SelNearMonster(szMonName);
}

