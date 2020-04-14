#include "LuaGame.h"
#include "GameFunction.h"

lua_State* g_L = luaL_newstate();	// 创建LUA解释器指针

/* [2020/04/10 11:32]-[Remark: None] */
/* [注册所有C函数为LUA函数]-[Return:None] */
void RegAllLuaFunc(lua_State* L)
{
	static luaL_Reg RegLuaFuncArr[] = 
	{ 
		"tracePrint",cLua_DbgPrint,"调试输出",cLua_DbgPrint,

		"findWay",cLua_FindWay,"移动到",cLua_FindWay,

		"getRoleXY",cLua_GetRoleXY,"获取人物坐标",cLua_GetRoleXY,

		"useGoods",cLua_UseGoods,"使用物品",cLua_UseGoods,

		"useSkill",cLua_UseSkill,"使用技能",cLua_UseSkill,

		"SelNearMonster",cLua_SelNearMonster,"选中怪物",cLua_SelNearMonster,

		"Wait",cLua_Wait, "等待",cLua_Wait
	};
	for(int i = 0; i < sizeof(RegLuaFuncArr)/sizeof(luaL_Reg); ++i)
	{
		lua_register(L, RegLuaFuncArr[i].name, RegLuaFuncArr[i].func);
	}
}

//----------------------以下函数均为 lua调用的C++函数格式------------------------//
/* [2020/04/11 17:36]-[Remark: None] */
/* [调试输出]-[Return:None] */
int cLua_DbgPrint(lua_State* L)
{
	DWORD dwArgNum = lua_gettop(L);	// 获取参数个数

	const char* szp = NULL;
	char szStr[1000] = "Game:";
	if (dwArgNum == 0)	return FALSE;
	szp = lua_tostring(L, 1);
	strcat_s(szStr, szp);
	for(DWORD i = 2; i <= dwArgNum; ++i)
	{
		if (lua_isnumber(L,i) || lua_isstring(L,i))
		{
			szp = lua_tostring(L, i);
			strcat_s(szStr, ",");
			strcat_s(szStr, szp);
		}
		else
		{
			strcat_s(szStr, ",");
			strcat_s(szStr, "???");
		}
	}
	OutputDebugStringA(szStr);

	lua_pushboolean(L, TRUE);	// 正常执行,返回值为TRUE
	return 1;
}

/* [2020/04/11 18:37]-[Remark: None] */
/* [移动到]-[Return:None] */
int cLua_FindWay(lua_State* L)
{
	DWORD dwArgNum = lua_gettop(L);	// 获取参数个数

	if (dwArgNum < 2)
	{
		lua_pushboolean(L, FALSE);	// 未正常执行,返回值为FALSE
		return 1;
	}
	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);
	FindWay(x, y);

	lua_pushboolean(L, TRUE);	// 正常执行,返回值为TRUE
	return 1;
}

/* [2020/04/11 20:26]-[Remark: None] */
/* [获取人物坐标]-[Return:None] */
int cLua_GetRoleXY(lua_State * L)
{
	DWORD dwArgNum = lua_gettop(L);	// 获取参数个数
	
	int Pos[2];
	GetRoleXY(Pos);

	lua_pushinteger(L, Pos[0]);
	lua_pushinteger(L, Pos[1]);
	return 2;
}

/* [2020/04/12 08:56]-[Remark: None] */
/* [使用物品]-[Return:None] */
int cLua_UseGoods(lua_State * L)
{
	DWORD dwArgNum = lua_gettop(L);	// 获取参数个数
	if (dwArgNum == 0)
	{
		lua_pushinteger(L, FALSE);
		return 1;
	}
	for(DWORD i = 1; i <= dwArgNum; ++i)
	{
		if (lua_isstring(L,i) == FALSE)
		{
			continue;
		}
		const char* szGoodName = lua_tostring(L, i);
		UseGoodsByName(szGoodName);
		Sleep(300);
	}
	
	lua_pushboolean(L, TRUE);	// 正常执行,返回值为TRUE
	return 1;
}

/* [2020/04/12 10:39]-[Remark: None] */
/* [使用技能]-[Return:None] */
int cLua_UseSkill(lua_State * L)
{
	DWORD dwArgNum = lua_gettop(L);	// 获取参数个数
	if (dwArgNum == 0)
	{
		lua_pushinteger(L, FALSE);
		return 1;
	}
	const char* szSkillName = lua_tostring(L, 1);
	UseSkillByName(szSkillName);

	lua_pushboolean(L, TRUE);	// 正常执行,返回值为TRUE
	return 1;
}

/* [2020/04/12 11:45]-[Remark: None] */
/* [选中怪物]-[Return:None] */
int cLua_SelNearMonster(lua_State * L)
{
	DWORD dwArgNum = lua_gettop(L);	// 获取参数个数
	if (dwArgNum == 0)
	{
		SelNearMonster(NULL);
		lua_pushboolean(L, TRUE);
		return 1;
	}
	const char* szMonName = lua_tostring(L, 1);
	SelNearMonster(szMonName);

	lua_pushboolean(L, TRUE);	// 正常执行,返回值为TRUE
	return 1;
}

/* [2020/04/13 12:39]-[Remark: None] */
/* [等待]-[Return:None] */
int cLua_Wait(lua_State * L)
{
	DWORD dwArgNum = lua_gettop(L);	// 获取参数个数
	if (dwArgNum == 0)
	{
		Sleep(100);
		lua_pushinteger(L, TRUE);
		return 1;
	}
	Sleep(lua_tointeger(L, 1));

	lua_pushboolean(L, TRUE);	// 正常执行,返回值为TRUE
	return 1;
}
