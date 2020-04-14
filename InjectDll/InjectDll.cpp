// InjectDll.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <string.h>
#define GameClassName "D3D Window"
#define dllName "MFC_DLL.dll"

/********************************************************************************************/
/* Description	: 注入某个路径的dll到游戏进程内存中
/* Return		: 成功返回真，失败返回假
/* Time			: 2019/12/23 13:14
/* Remark		: 要让游戏进程自己loadLibrary来加载dll,要先写入dll全路径,再创建远程线程让进程加载dll
/********************************************************************************************/
void InjectDll(HWND hGame,const char*DllFullNamePath) {
	DWORD pid = 0;
	HANDLE hProcess = NULL;
	LPDWORD lpdwAddr = NULL;
	DWORD byWriteSize = 0;
	HANDLE hThread = NULL;
	
	if (hGame != NULL)
	{
		GetWindowThreadProcessId(hGame, &pid);	//获取 进程ID
		if (pid != NULL)
		{
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);	// 获取 进程句柄
			if (hProcess != NULL)
			{
				lpdwAddr = (LPDWORD)VirtualAllocEx(hProcess,
					NULL,256, MEM_COMMIT,PAGE_READWRITE);	//在游戏进程中申请VA空间来存放要注入的dll的路径名
				if (lpdwAddr != NULL)
				{
					WriteProcessMemory(hProcess, lpdwAddr, DllFullNamePath,
						strlen(DllFullNamePath) + 1, &byWriteSize);	//写入dll全路径名
					if (byWriteSize >= strlen(DllFullNamePath))
					{
						hThread = CreateRemoteThread(hProcess, NULL, NULL,
							(LPTHREAD_START_ROUTINE)LoadLibraryA,	// 不同的程序Kernel32.dll加载的位置都是77270000,所以LoadLibraryA的地址也相同
							lpdwAddr, NULL, NULL);	//创建远程线程
						WaitForSingleObject(hThread, 0xFFFFFFFF);	//等待注入DLL的线程执行完
						VirtualFreeEx(hProcess, lpdwAddr, 256, MEM_DECOMMIT);
						CloseHandle(hThread);
						CloseHandle(hProcess);
						printf("dll注入成功！\n");
					}
					else
					{
						printf("写入dll路径失败\n");
					}
				}
			}
		}
	}
}

int main()
{
	printf("注入dll\n");
	char DirName[256] = "";	//存放目录名
	char dllNamePath[256] = "";	//存放Dll全路径名
	GetCurrentDirectoryA(sizeof(DirName), DirName);
	strcpy_s(dllNamePath,DirName);
	strcat_s(dllNamePath, "\\");
	strcat_s(dllNamePath, dllName);
	printf("%s\n", dllNamePath);
	HWND hGame = FindWindow(GameClassName, NULL);	//获取 游戏窗口句柄
	InjectDll(hGame,dllNamePath);
}

