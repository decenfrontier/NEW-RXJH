// CPage_BuySell.cpp: 实现文件
//
#include "stdafx.h"
#include "pch.h"
#include "MFC_DLL.h"
#include "CPage_BuySell.h"
#include "afxdialogex.h"


// CPage_BuySell 对话框

IMPLEMENT_DYNAMIC(CPage_BuySell, CDialogEx)

CPage_BuySell::CPage_BuySell(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_BUYSELL, pParent)
	, m_chk_IsAutoBuyHpGoods(TRUE)
	, m_chk_IsAutoBuyMpGoods(TRUE)
	, m_edt_dwMinNum_HpGoods()
	, m_edt_dwMinNum_MpGoods()
	, m_edt_Num_DrugShop()
	, m_edt_Num_Depot()
	, m_edt_dwMaxNum_HpGoods()
	, m_edt_dwMaxNum_MpGoods()
{

}

CPage_BuySell::~CPage_BuySell()
{
}

void CPage_BuySell::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_chk_IsAutoBuyHpGoods);
	DDX_Check(pDX, IDC_CHECK2, m_chk_IsAutoBuyMpGoods);
	DDX_Text(pDX, IDC_EDIT1, m_edt_dwMinNum_HpGoods);
	DDX_Text(pDX, IDC_EDIT2, m_edt_dwMinNum_MpGoods);
	DDX_Control(pDX, IDC_COMBO1, m_cmb_ctl_DrugShop);
	DDX_Control(pDX, IDC_COMBO2, m_cmb_ctl_Depot);
	DDX_Text(pDX, IDC_EDIT3, m_edt_Num_DrugShop);
	DDX_Text(pDX, IDC_EDIT5, m_edt_Num_Depot);
	DDX_Control(pDX, IDC_LIST1, m_lst_ctl_SupplyGoods);
	DDX_Text(pDX, IDC_EDIT6, m_edt_dwMaxNum_HpGoods);
	DDX_Text(pDX, IDC_EDIT7, m_edt_dwMaxNum_MpGoods);
}


BEGIN_MESSAGE_MAP(CPage_BuySell, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage_BuySell::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON8, &CPage_BuySell::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON2, &CPage_BuySell::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON10, &CPage_BuySell::OnBnClickedButton10)
END_MESSAGE_MAP()


// CPage_BuySell 消息处理程序

//按钮事件: 添加 药店物品 到列表框
void CPage_BuySell::OnBnClickedButton1()
{
	UpdateData(TRUE);	//控件 -> 变量

	char szCmbSelText[30];
	m_cmb_ctl_DrugShop.GetWindowTextA(szCmbSelText, sizeof(szCmbSelText));
	CString strTmp;
	strTmp.Format("购买药符,%s:%d", szCmbSelText, m_edt_Num_DrugShop);
	//添加到补给列表框
	m_lst_ctl_SupplyGoods.AddString(strTmp);

	UpdateData(FALSE);	//变量 -> 控件
}

	
//按钮事件: 添加 仓库物品 到列表框
void CPage_BuySell::OnBnClickedButton8()
{
	UpdateData(TRUE);	//控件 -> 变量

	char szCmbSelText[30];
	m_cmb_ctl_Depot.GetWindowTextA(szCmbSelText, sizeof(szCmbSelText));
	CString strTmp;
	strTmp.Format("取出物品,%s:%d", szCmbSelText, m_edt_Num_Depot);
	//添加到补给列表框
	m_lst_ctl_SupplyGoods.AddString(strTmp);

	UpdateData(FALSE);	//变量 -> 控件
}

//按钮事件:  列表框选中项 清除
void CPage_BuySell::OnBnClickedButton10()
{
	UpdateData(TRUE);	//控件 -> 变量

	m_lst_ctl_SupplyGoods.DeleteString(m_lst_ctl_SupplyGoods.GetCurSel());

	UpdateData(FALSE);	//变量 -> 控件
}

//按钮事件: 设置出售物品
void CPage_BuySell::OnBnClickedButton2()
{
	m_Page_GoodsManage.ShowWindow(SW_SHOW);
}


BOOL CPage_BuySell::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Page_GoodsManage.Create(IDD_PAGE_GOODSMANAGE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



