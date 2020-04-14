#pragma once
// 游戏数据结构 及偏移管理
#include <Windows.h>

// 类型别名 与 非结构成员函数声明
typedef unsigned __int64 QWORD;
void DbgOutput(const char *szFormat, ...);
HWND GetGameWndHandle();	// 获取游戏窗口句柄
DWORD TwoPoint_Distance(float x1, float y1, float x2, float y2);	// 计算两坐标点的距离
BOOL WSASendData(LPCVOID lpData, DWORD dwBufSize);	// 发送数据到服务器

#define AllTask 0x2332
#define SelfTask 0x2333
void PrintAllTask(DWORD dwTaskListID);

// 所有对象通用的宏
#define offset_Obj_Type 0x8		// 对象类型
#define offset_Obj_Index 0xC	// 对象在所有对象列表的下标

#pragma pack(1)	// 按一字节对齐，即不填充空格
// 批量存取仓库物品时需修改的数据,共A0个字节
struct TStorePickDepotData
{
	BYTE byData1[0xA];
	QWORD nqCmd;		// 存物：E6 CE BD 93 00 00 00 03
						// 取物：00 00 00 00 00 00 00 05
	DWORD dwType1;		// addr+0x12：[背包物品对象+0x54]
	BYTE byData2[0x4];	   
	WORD nwSaveNum;		// addr+0x1A：自行定义				要存入的数量
	BYTE byData3[0xE];	   
	QWORD nqID2;		// addr+0x2A：[背包物品对象+0x5C]
	DWORD dwType3;		// addr+0x32：[背包物品对象+0x54]
	BYTE byData4[0x4];	   
	WORD nwGoodsNum;	// addr+0x3A：[背包物品对象+0xC4C]	当前数量，非必需
	BYTE byData5[0x7];	   
	BYTE byIndex;		// addr+0x43：[背包物品对象+0x1FC]	在背包中的下标
	BYTE byData6[0x5D];	// A0-43=5D
};

//商店买物品时需修改的数据,共A0个字节
#define BuyGoods 0x01
#define SellGoods 0x02
struct TBuySellData
{
	BYTE byData1[0x2];
	DWORD dwCmd;			// addr+0x2：00880092是买物CALL，008C0094是批量存取仓库CALL
	DWORD dwBuySellType;	// addr+0x6：1是买，2是卖
	BYTE byData2[0x4];		// addr+0xA：0xEB1B5614
	DWORD dwType1;			// addr+0xE：[物品对象+0x54]
	BYTE byData3[0x4];		   
	WORD nwGoodsNum1;		// addr+0x16：要购买或出售的数量
	BYTE byData4[0xE];		   
	QWORD nqID2;			// addr+0x26：[背包物品对象+0x5C]
	DWORD dwType2;			// addr+0x2E：[物品对象+0x54]
	BYTE byData5[0x4];		   
	WORD nwGoodsNum2;		// addr+0x36：要出售的数量
	BYTE byData6[0x7];		   
	BYTE byIndex;			// addr+0x3F：[背包物品对象+0x1FC]	在背包中的下标
	BYTE byData7[0x61];		// A0-3F=61
};
#pragma pack()	//恢复默认对齐

//人物属性对象：[0x02C186D8]
struct TRoleProperty 
{
	char* szRoleName;					// 角色名：[人物属性对象+0x00]
	BYTE byLevel;						// 等级：[人物属性对象+0x34]
	BYTE byTransferNum;					// 转职次数：[人物属性对象+0x35]
	char* szRole名声;					// 名声：[人物属性对象+0x36]
	DWORD dwHp;							// 当前HP：[人物属性对象+0x80]
	DWORD dwMp;							// 当前MP：[人物属性对象+0x84]
	DWORD dwAngry;						// 当前愤怒：[人物属性对象+0x88]
	DWORD dwHpMax;						// 最大HP：[人物属性对象+0x8C]
	DWORD dwMpMax;						// 最大MP：[人物属性对象+0x90]
	DWORD dwAngryMax;					// 最大愤怒：[人物属性对象+0x94]
	QWORD nqExp;						// 当前经验：[人物属性对象+0x98]
	QWORD nqExpMax;						// 升级经验：[人物属性对象+0xA0]
	DWORD dw历练;						// 历练值：[人物属性对象+0xAC]
	DWORD dw心力;						// 心：[人物属性对象+0xB0]
	DWORD dw气力;						// 气：[人物属性对象+0xB4]
	DWORD dw体力;						// 体：[人物属性对象+0xB8]
	DWORD dw魂力;						// 魂：[人物属性对象+0xBC]
	DWORD dw攻击;						// 攻击：[人物属性对象+0xC8]
	DWORD dw防御;						// 防御：[人物属性对象+0xCC]
	DWORD dw命中;						// 命中：[人物属性对象+0xD0]
	DWORD dw回避;						// 回避：[人物属性对象+0xD4]
	QWORD nqMoney;						// 金钱：[人物属性对象+0xE4]
	WORD nw负重;							// 当前负重：[人物属性对象+0xEC]
	WORD nw负重Max;						// 最大负重：[人物属性对象+0xEE]
	BYTE byPoint气功;					// 未分配气功点：[人物属性对象+0xF0]
	BYTE byQiGongPoints[32];			// 各技能气功点：[人物属性对象+0xF4+4*i]
	DWORD dwHpPercent;					// 血量百分比
	DWORD dwMpPercent;					// 蓝量百分比
	TRoleProperty* GetData();	
	void PrintMsg();
};

//背包物品对象：[ [02E3D3E4]+43C+4*下标 ]
struct TGoodsObj 
{
	DWORD dwGoodsObj;	// 物品对象本身
	char* szName;		// 物品名字：[物品对象+0x64]
	char* szMsg;		// 物品功能描述：[物品对象+0xF9]
	char* szAttribute;	// 物品属性：[物品对象+0x238]
						   
	DWORD dwType1;		// Type1：[物品对象+0x54]
	QWORD nqID2;		// ID2：[物品对象+0x5C]
	WORD nwNum;			// 物品数量：[物品对象+0xC4C]
	BYTE byIndex;		// 在列表的下标：[物品对象+0x1FC]
};

//背包列表
struct TBackpackList
{	
	TGoodsObj tlist[36];	//背包列表里包含36个物品对象
	TBackpackList* GetData();
	void PrintMsg();
	BOOL UseGoodsByIndex(DWORD dwIndex);		// 使用背包物品,根据 下标
	int GetGoodsIndexByName(char* szName);		// 查询背包物品，有则返回下标，无则返回-1
	BOOL UseGoodsByName(char* szName);			// 使用背包物品,根据 名字
	DWORD SelectGoods(DWORD dwIndex);			// 选中背包指定格的物品
	DWORD GetGoodsNumByName(char* szName);		// 查询背包指定物品数量
	DWORD GetAllHpNum();						// 查询所有Hp药品数量
	DWORD GetAllMpNum();						// 查询所有Mp药品数量
	BOOL IsBackpackFull();						// 判断背包是否满

	BOOL MoveGoodsToDepot();					// 移动背包已选中物品到仓库
	BOOL MoveGoodsToDepot(char* szName);		// 移动背包指定名字的物品到仓库
	BOOL MoveGoodsToDepot(char* szName, WORD nwIndex);	// 移动背包指定名字的物品到仓库,需指定数量

	enum EquipType
	{
		dwClothes,			// 衣服
		dwHandL, dwHandR,	// 左右护手
		dwWeapon,			// 武器
		dwShoes,			// 鞋子
		dwArmor,			// 护甲
		dwNeck,				// 项链
		dwEarL, dwEarR,		// 左右耳环
		dwRingL, dwRingR	// 左右戒指
	};
	BOOL MoveGoodsToEquip(DWORD dwIndex);			// 移动背包已选中物品到装备列表指定格
	BOOL MoveGoodsToEquip_Clothes(char* szName);	// 移动背包指定名字的物品到装备衣服栏
	BOOL MoveGoodsToEquip_HandL(char* szName);		// 移动背包指定名字的物品到装备左护手
	BOOL MoveGoodsToEquip_HandR(char* szName);		// 移动背包指定名字的物品到装备右护手
	BOOL MoveGoodsToEquip_Weapon(char* szName);		// 移动背包指定名字的物品到装备武器
	BOOL MoveGoodsToEquip_Shoes(char* szName);		// 移动背包指定名字的物品到装备鞋子
	BOOL MoveGoodsToEquip_Armor(char* szName);		// 移动背包指定名字的物品到装备护甲
	BOOL MoveGoodsToEquip_Neck(char* szName);		// 移动背包指定名字的物品到装备项链
	BOOL MoveGoodsToEquip_EarL(char* szName);		// 移动背包指定名字的物品到装备左耳环
	BOOL MoveGoodsToEquip_EarR(char* szName);		// 移动背包指定名字的物品到装备右耳环
	BOOL MoveGoodsToEquip_RingL(char* szName);		// 移动背包指定名字的物品到装备左戒指
	BOOL MoveGoodsToEquip_RingR(char* szName);		// 移动背包指定名字的物品到装备右戒指
};

//仓库列表
struct TDepotList
{
	TGoodsObj tlist[60];	//仓库列表里包含60个物品对象
	TDepotList* GetData();
	void PrintMsg();
	int GetGoodsIndexByName(char* szName);		// 查询仓库物品，有则返回下标，无则返回-1
	DWORD GetGoodsNumByName(char* szName);		// 查询仓库物品数量
	DWORD SelectGoods(DWORD dwIndex);			// 选中仓库指定格的物品
												   
	BOOL MoveGoodsToBackPack();					// 移动仓库已选中物品到背包
	BOOL MoveGoodsToBackpack(char* szName);		// 移动仓库指定名字的物品到背包
	BOOL MoveGoodsToBackpack(char* szName, WORD nwIndex);	// 移动仓库指定名字的物品到背包,需指定数量
};

//商店列表
struct TShopList 
{
	DWORD GetShopBase();	// 获取商店列表基址
	TGoodsObj tlist[60];	// 商店列表里包含60个物品对象
	TShopList* GetData();
	void PrintMsg();
	int GetGoodsIndexByName(char* szName);		// 查询商店物品，有则返回下标，无则返回-1
	BOOL BuyGoodsByName(char* szName, WORD nwIndex);	// 通过物品名字，购买指定数量的物品
	BOOL SellGoodsByName(char* szName, WORD nwIndex);	// 通过物品名字，卖出指定数量的物品
};

//周围对象：[04280BA0+4*i]
struct TNearObj 
{
	DWORD dwNearObj;			// 周围对象
	DWORD dwType;				// 对象类型：[周围对象+0x8]，0x2E普通怪物，0x31玩家
	DWORD dwIndexInAllObj;		// 对象ID：[周围对象+0xC]
	BOOL IsSelected;			// 选中状态：[周围对象+0x354] 
	float flDistance;			// 与玩家的距离：[周围对象+0x35C]
	char* szName;				// 非玩家名称：[周围对象+0x360]   玩家名字：[周围对象+0x18]
	BYTE IsDead;				// 是否死亡：[周围对象+0x3C0]
	BYTE IsAbleKill;			// 是否可用必杀技: [周围对象+0x3C8]
	DWORD dwHP;					// 血量：[周围对象+0x5F4]    
	DWORD dwLevel;				// 等级：[周围对象+0x5F8]   
	float flX;					// 坐标X：[周围对象+0x1060]
	float flY;					// 坐标Y：[周围对象+0x1068]
};

#define Size_NearList 50
//周围对象列表
typedef struct TNearList
{
	TNearObj tlist[Size_NearList];	// 周围对象列表里大概包含Size_NearList个周围对象
	TNearList* GetData();
	void PrintMsg();
	DWORD GetNpcID(char* szName);
	BOOL ChatWithNPC(char* szName);	// 根据NPC名字，与指定NPC对话
	BOOL ClickChatMenuOption(DWORD dwIndex);	// 点击对话框菜单选项(尽量不用)
	BOOL EndChatWithNPC(char* szNpcName);		// 结束与NPC对话
	BOOL OpenShop(char* szNpcName);				// 打开仓库,买进/卖出
}TNearList;

//动作对象：[ [02E3DD58]+43C+4*i ]
typedef struct TActionObj 
{
	char* szName;		// 动作名称：[动作对象+0x64]
	DWORD dwActionID;	// 动作ID：[动作对象+0x54]
}TActionObj;

//动作对象列表
typedef struct TActionList 
{
	TActionObj tlist[12];	// 动作列表 有12个实用的动作对象
	TActionList* GetData();
	void PrintMsg();
	BOOL UseActionByIndex(DWORD dwIndex);	// 使用动作 通过下标
	BOOL UseActionByName(char* szActionName);	// 使用动作 通过动作名
}TActionList;

//玩家对象：[02E65A24]
#define offset_PlayerObj_dwSelIndex 0x1A64
#define offset_PlayerObj_flCurX 0x1C9C
#define offset_PlayerObj_flDesX 0x1C60
typedef struct TPlayerObj
{
	DWORD dwSelIndex;	// 玩家当前选中的对象的ID,也是所有对象数组的下标,未选则为0xFFFF
						// [玩家对象+0x1A64]
	float flDesX;		// [玩家对象+0x1C60]
	float flDesY;		// [玩家对象+0x1C68]
	float flCurX;		// [玩家对象+0x1C9C]
	float flCurY;		// [玩家对象+0x1CA4]

	TPlayerObj* GetData();
	void PrintMsg();
	BOOL SelObj(DWORD dwIndexInAllObj);	// 选择对象
	BOOL CancelSel();		// 取消选中对象
	BOOL SelNearMonster();	// 自动选近怪
	BOOL SelNearMonster(const char* szMonName);	// 根据怪物名选择最近的怪物
	BOOL AutoBeatMonster();	// 自动普攻打怪
	BOOL AutoBeatMonsterBySkill(char* szSkillName);	// 自动技能打怪
	DWORD GetSelObjType();	// 获得选中对象的类型
	BOOL IsSelObjDead();	// 选择的怪物是否死亡
	BOOL IsSelObjAbleKill();// 选择的怪物是否可被必杀
	BOOL RunToXY(float flx, float fly);	// 跑到指定坐标
}TPlayerObj;

// 技能对象：[ [02E3EF64]+0x43C+4*i ]
typedef struct TSkillObj
{
	DWORD dwSkillObj;			// 技能对象
	DWORD dwIndexForSkill;		// 位于技能的第几个
	DWORD dwType;				// 类型：[技能对象+0x8]
	DWORD dwIndexForAll;		// ID：[技能对象+0xC]
	char* szName;				// 名称：[技能对象+0x64]
	DWORD byLevelNeed;			// 等级需要：[技能对象+0xB4]
	BOOL IsLearned;             // 技能是否已学：[技能对象+0x1FE]
	DWORD dwExpNeed;			// 历练需要：[技能对象+0x270]
}TSkillObj;

// 技能列表
typedef struct TSkillList
{
	TSkillObj tlist[32];		// 攻击技能列表共32个技能，有8个是技能书，不可修炼
	TSkillList* GetData();
	void PrintMsg();
	BOOL DropSkillToF1F10(char *szSkillName,DWORD dwIndexF1F10);
	int DropSkillToF1F10(char *szSkillName);
	BOOL LearnSkillByIndex(DWORD dwIndex);		// 修炼技能，通过技能列表下标
	BOOL LearnSkillByName(char* szSkillName);	// 修炼技能，通过技能名字
	BOOL IsLearnable(DWORD dwIndex);			// 技能是否可修炼
}TSkillList;

// 快捷栏对象：[ [02E3EF08]+0*4+0x43C ]
typedef struct TShortcutObj
{
	DWORD dwScObj;	// 快捷栏对象
	DWORD dwType;	// 类型：[快捷栏对象+0x08]
	char* szName;	// 名称：[快捷栏对象+0x64]
}TShortcutObj;

// 快捷栏列表
typedef struct TShortcutList
{
	TShortcutObj tlist[10];	// 快捷栏列表有10个快捷栏对象
	TShortcutList* GetData();
	void PrintMsg();
	BOOL UseSkillByName(char* szSkillName);
	BOOL UseSkillByIndex(DWORD dwIndex);
	int GetIndexByName(char* szSkillName);
	BOOL GetScNullIndex();
}TShortcutList;

// 地上物品对象：[04280BA0+4*i]
#define offset_TGroundGoodsObj_szName 0x94
typedef struct TGroundGoodsObj
{
	DWORD dwType;	// 对象类型：[周围对象+0x8]，0x33为地上物品
	DWORD dwIndexInAllObj;		// 对象ID：[周围对象+0xC]
	char* szName;	// 名称：[周围对象+0x94]
}TGroundGoodsObj;

// 地上物品列表
typedef struct TGroundGoodsList 
{
	TGroundGoodsObj tlist[Size_NearList];
	TGroundGoodsList* GetData();
	void PrintMsg();
	BOOL SelGroundGoods();	// 选中地上物品
	BOOL PickupGoods();		// 自动捡物
}TGroundGoodsList;
