#pragma once

#include "CDialogMainTab.h"
// CDialogCtrlCenter 对话框

class CDialog_CtrlCenter : public CDialogEx
{
	DECLARE_DYNAMIC(CDialog_CtrlCenter)

public:
	CDialog_CtrlCenter(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialog_CtrlCenter();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_DIALOG_MAIN};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	CDialog_MainTab m_DialogMainTab;
	afx_msg void OnBnClickedButton7();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
};

