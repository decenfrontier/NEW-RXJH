// CDialogCtrlCenter.cpp: 实现文件
//

#include "stdafx.h"
#include "MFC_DLL.h"
#include "CDialogCtrlCenter.h"
#include "afxdialogex.h"
#include "BaseGame.h"
#include "StructGame.h"
#include "HookGameMainThread.h"
#include "GameFunction.h"
#include "LuaGame.h"

#pragma comment(lib,"GameData.lib")
#pragma comment(lib,"LUA5_3_5.lib")
// CDialogCtrlCenter 对话框

IMPLEMENT_DYNAMIC(CDialog_CtrlCenter, CDialogEx)



CDialog_CtrlCenter::CDialog_CtrlCenter(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CTRLCENTER, pParent)
{

}

CDialog_CtrlCenter::~CDialog_CtrlCenter()
{
}

void CDialog_CtrlCenter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialog_CtrlCenter, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialog_CtrlCenter::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CDialog_CtrlCenter::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialog_CtrlCenter::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDialog_CtrlCenter::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CDialog_CtrlCenter::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDialog_CtrlCenter::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CDialog_CtrlCenter::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON11, &CDialog_CtrlCenter::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CDialog_CtrlCenter::OnBnClickedButton12)
END_MESSAGE_MAP()


// CDialogCtrlCenter 消息处理程序
//挂接主线程
void CDialog_CtrlCenter::OnBnClickedButton2()
{
	HookMainThread();
}

//卸载主线程
void CDialog_CtrlCenter::OnBnClickedButton3()
{
	UnHookMainThread();
}

//test1
void CDialog_CtrlCenter::OnBnClickedButton1()
{
	msg_Test1("韦大宝");
}

//test2
void CDialog_CtrlCenter::OnBnClickedButton5()
{
	msg_Test2("韦大宝");
}

//test3
void CDialog_CtrlCenter::OnBnClickedButton7()
{
	msg_Test3("韦大宝");
}

//LUA脚本线程函数
void CALLBACK ThreadFunc_LUA(LPVOID lP)
{
	DbgOutput("LUA脚本开始执行\n");
	// test.lua若为绝对路径,应放至dll注入后的目录 D:/热血江湖兵临城下/Client
	luaL_dofile(g_L, "D:/热血江湖兵临城下/Client/test.lua");
	DbgOutput("LUA脚本执行完毕\n");
}

//测试1
void CDialog_CtrlCenter::OnBnClickedButton4()
{
	luaL_openlibs(g_L);		// 打开所有LUA库支持
	RegAllLuaFunc(g_L);		// 注册所有LUA函数

	HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadFunc_LUA,
						NULL, NULL, NULL);
}

//测试2
void CDialog_CtrlCenter::OnBnClickedButton11()
{
	g_tNearList.PrintMsg();
	g_tPlayerObj.IsSelObjAbleKill();
}

//内存优化
void CDialog_CtrlCenter::OnBnClickedButton12()
{
	SetProcessWorkingSetSize((HANDLE)-1, -1, -1);
}

//显示外挂
void CDialog_CtrlCenter::OnBnClickedButton6()
{
	m_DialogMainTab.ShowWindow(SW_SHOW);
}

//窗口初始化
BOOL CDialog_CtrlCenter::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_DialogMainTab.Create(IDD_DIALOG_MAINTAB);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}




