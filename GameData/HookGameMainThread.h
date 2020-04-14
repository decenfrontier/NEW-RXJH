#pragma once
#include "windows.h"

#define MSG_UseGoodsByName 1	// 使用物品
#define MSG_DropSkillToF1F10 2	// 放置技能到快捷栏
#define MSG_UseSkillByName 3	// 使用技能
#define MSG_AutoBeatMonsterBySkill 4	// 使用技能自动打怪
#define MSG_RunToXY 5			// 寻路
#define MSG_ChatWithNPC 6		// 与NPC对话
#define MSG_EndChatWithNPC 7	// 结束对话
#define MSG_OpenShop 8			// 打开商店
#define MSG_BuyGoods 9			// 购买物品
#define MSG_SellGoods 10		// 出售物品
#define MSG_CancelSel 11		// 取消选中人对象
#define MSG_MoveGoodsToDepot 12	// 存放物品到仓库
#define MSG_MoveGoodsToBcpk	 13	// 从仓库取出物品
#define MSG_PickupGoods 14		// 捡物
#define MSG_GetRoleXY 15		// 获取人物坐标
#define MSG_SelNearMonster 16	// 自动选择近怪

#define MSG_TEST1 888
#define MSG_TEST2 888+1
#define MSG_TEST3 888+2

//以下函数可以在 非主线程执行
BOOL HookMainThread();
BOOL UnHookMainThread();
void msg_UseGoodsByName(const char* szName);
void msg_DropSkillToF1F10(const char* szSkillName);
void msg_UseSkillByName(const char* szSkillName);
void msg_AutoBeatMonsterBySkill(char* szSkillName);
void msg_RunToXY(int iX,int iY);
void msg_ChatWithNPC(char* szNpcName);
void msg_EndChatWithNPC(char* szNpcName);
void msg_OpenShop(char* szNpcName);
void msg_BuyGoods(char* szName, WORD nwIndex);
void msg_SellGoods(char* szName, WORD nwIndex);
void msg_CancelSel();
void msg_MoveGoodsToDepot(char* szName, WORD nwIndex);
void msg_MoveGoodsToBcpk(char* szName, WORD nwIndex);
void msg_PickupGoods();
void msg_GetRoleXY(int* Pos);
void msg_SelNearMonster(const char* szMonName);

void msg_Test1(LPVOID lpData);
void msg_Test2(LPVOID lpData);
void msg_Test3(LPVOID lpData);

//消息传参用的结构体
typedef struct Goods
{
	char szName[30];
	WORD nwIndex;
}Goods;
