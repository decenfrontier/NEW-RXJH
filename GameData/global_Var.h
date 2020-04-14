#pragma once
#include "StructGame.h"
#include "CAutoPlay.h"
#include "GameFunction.h"

extern TRoleProperty g_tRoleProperty;	//角色属性
extern TPlayerObj g_tPlayerObj;			//玩家对象

extern TBackpackList g_tBackList;		//背包列表
extern TDepotList g_tDeptList;			//仓库列表
extern TShopList g_tShopList;			//商店列表
extern TNearList g_tNearList;			//周围列表
extern TActionList g_tActList;			//动作列表
extern TSkillList g_tSkiList;			//技能列表
extern TShortcutList g_tScList;			//快捷栏列表
extern TGroundGoodsList g_tGroundList;	//地面物品列表

extern CAutoPlay g_cAutoPlay;			//挂机类对象