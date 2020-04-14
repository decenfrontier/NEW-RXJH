#pragma once
#include "CPage_GoodsManage.h"

// CPage_BuySell 对话框

class CPage_BuySell : public CDialogEx
{
	DECLARE_DYNAMIC(CPage_BuySell)

public:
	CPage_BuySell(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPage_BuySell();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_BUYSELL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CPage_GoodsManage m_Page_GoodsManage;

	BOOL m_chk_IsAutoBuyHpGoods;
	BOOL m_chk_IsAutoBuyMpGoods;
	DWORD m_edt_dwMinNum_HpGoods;
	DWORD m_edt_dwMinNum_MpGoods;
	CComboBox m_cmb_ctl_DrugShop;
	CComboBox m_cmb_ctl_Depot;
	DWORD m_edt_Num_DrugShop;
	DWORD m_edt_Num_Depot;
	afx_msg void OnBnClickedButton1();
	CListBox m_lst_ctl_SupplyGoods;
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton2();

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton10();
	DWORD m_edt_dwMaxNum_HpGoods;
	DWORD m_edt_dwMaxNum_MpGoods;
};
