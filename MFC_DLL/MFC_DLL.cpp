// MFC_DLL.cpp: 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "MFC_DLL.h"
#include "CDialogCtrlCenter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CMFCDLLApp

BEGIN_MESSAGE_MAP(CMFCDLLApp, CWinApp)
END_MESSAGE_MAP()


// CMFCDLLApp 构造

CMFCDLLApp::CMFCDLLApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


void DbgPrintMine(const char *szFormat, ...) {
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

// 唯一的 CMFCDLLApp 对象

CMFCDLLApp theApp;
CDialog_CtrlCenter *pMainDialog;

// 显示窗口 线程函数
DWORD WINAPI MyThreadProc(LPVOID lpParameter)
{
	//添加显示窗口的代码
	pMainDialog = new CDialog_CtrlCenter;
	pMainDialog->DoModal();	//阻塞在这里，除非关掉才能往下执行
	
	//关闭此窗口后先删掉指针，再释放dll，退出线程
	delete pMainDialog;
	FreeLibraryAndExitThread(theApp.m_hInstance,1);
	
	return TRUE;
}

// CMFCDLLApp 初始化

BOOL CMFCDLLApp::InitInstance()
{
	CWinApp::InitInstance();

	HANDLE bRet = ::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MyThreadProc, NULL, NULL, NULL);
	DbgPrintMine("MFCDLL.dll初始化创建函数结果:%X", bRet);
	return TRUE;
}
