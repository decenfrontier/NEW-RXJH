#pragma once
#include "CPage_GuaJi.h"
#include "CPage_BuySell.h"
#include <fstream>

using namespace std;

#define ID_TIMER_QingGong  1
#define ID_TIMER_HMProtect 2
#define ID_TIMER_CpuLower 3

// CDialogMainTab 对话框

class CDialog_MainTab : public CDialogEx
{
	DECLARE_DYNAMIC(CDialog_MainTab)

public:
	CDialog_MainTab(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialog_MainTab();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TABWND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CPage_GuaJi m_Page_GuaJi;
	CPage_BuySell m_Page_BuySell;
	virtual BOOL OnInitDialog();
	CTabCtrl m_ctl_TabMain;
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonApplyset();	//应用配置
	afx_msg void OnBnClickedButtonStartautoplay();	//开始挂机
	CButton m_ctl_BtnStartOrStop;
	afx_msg void OnBnClickedButton1();	//保存配置
	afx_msg void OnBnClickedButton8();	//载入配置

	void SaveConfigToFile(char* szConfigFile);		//保存所有页面设置到文件
	void ReadConfigFromFile(char* szConfigFile);	//从文件读取所有页面设置
	void ApplyConfigToCtrl(fstream *fs,char* szLineData);		//从一行数据中读取出控件变量名与值,并设置控件变量值
	
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);	//时钟事件处理
};

void ReplaceChar(char* Text, char ToRpc, char RpcTo);	//替换指定字串中的某一个字符