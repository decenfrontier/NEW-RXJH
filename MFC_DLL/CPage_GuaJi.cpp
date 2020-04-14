// CPage_GuaJi.cpp: 实现文件
//
#include "stdafx.h"
#include "pch.h"
#include "MFC_DLL.h"
#include "CPage_GuaJi.h"
#include "afxdialogex.h"
#include "global_Var.h"

// CPage_GuaJi 对话框

IMPLEMENT_DYNAMIC(CPage_GuaJi, CDialogEx)

CPage_GuaJi::CPage_GuaJi(CWnd* pParent /*=nullptr*/): CDialogEx(IDD_PAGE_GUAJI, pParent)
	, m_chk_IsAutoBeatMonster(TRUE)
	, m_chk_IsAutoUseHpGoods(TRUE)
	, m_chk_IsAutoUseMpGoods(TRUE)
	, m_cmb_szHpGoodsName(_T("金创药(小)"))
	, m_cmb_szMpGoodsName(_T("人参"))
	, m_edt_dwHpPercent(80)
	, m_edt_dwMpPercent(40)
	, m_chk_IsRangeLimit(TRUE)
	, m_edt_hitMonsterRange(200)
	, m_edt_LimitPosX(0)
	, m_edt_LimitPosY(0)
	, m_chk_IsUseSkill(FALSE)
	, m_chk_IsPickupGoods(FALSE)
	, m_cmb_szSkillName(_T(""))
	, m_chk_IsAutoUseQingGong(FALSE)
	, m_cmb_szQingGongName(_T(""))
	, m_edt_CheckHpMpSpeed(500)
	, m_chk_IsDisableRefresh(FALSE)
{

}

CPage_GuaJi::~CPage_GuaJi()
{
}

void CPage_GuaJi::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK3, m_chk_IsAutoBeatMonster);
	DDX_Check(pDX, IDC_CHECK1, m_chk_IsAutoUseHpGoods);
	DDX_Check(pDX, IDC_CHECK2, m_chk_IsAutoUseMpGoods);
	DDX_CBString(pDX, IDC_COMBO1, m_cmb_szHpGoodsName);
	DDX_CBString(pDX, IDC_COMBO2, m_cmb_szMpGoodsName);
	DDX_Text(pDX, IDC_EDIT1, m_edt_dwHpPercent);
	DDX_Text(pDX, IDC_EDIT2, m_edt_dwMpPercent);
	DDX_Check(pDX, IDC_CHECK4, m_chk_IsRangeLimit);
	DDX_Text(pDX, IDC_EDIT3, m_edt_hitMonsterRange);
	DDX_Text(pDX, IDC_EDIT4, m_edt_LimitPosX);
	DDX_Text(pDX, IDC_EDIT5, m_edt_LimitPosY);
	DDX_Check(pDX, IDC_CHECK5, m_chk_IsUseSkill);
	DDX_Check(pDX, IDC_CHECK6, m_chk_IsPickupGoods);
	DDX_CBString(pDX, IDC_COMBO3, m_cmb_szSkillName);
	DDX_Check(pDX, IDC_CHECK7, m_chk_IsAutoUseQingGong);
	DDX_CBString(pDX, IDC_COMBO4, m_cmb_szQingGongName);
	DDX_Text(pDX, IDC_EDIT8, m_edt_CheckHpMpSpeed);
	DDX_Check(pDX, IDC_CHECK8, m_chk_IsDisableRefresh);
}


BEGIN_MESSAGE_MAP(CPage_GuaJi, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage_GuaJi::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPage_GuaJi 消息处理程序


BOOL CPage_GuaJi::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateData(FALSE);	//变量 -> 控件

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//取当前坐标
void CPage_GuaJi::OnBnClickedButton1()
{
	UpdateData(TRUE);	//控件 -> 变量
	g_tPlayerObj.GetData();
	m_edt_LimitPosX = (int)g_tPlayerObj.flCurX;
	m_edt_LimitPosY = (int)g_tPlayerObj.flCurY;
	UpdateData(FALSE);	//变量 -> 控件
}
