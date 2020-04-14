#pragma once


// CPage_GuaJi 对话框

class CPage_GuaJi : public CDialogEx
{
	DECLARE_DYNAMIC(CPage_GuaJi)

public:
	CPage_GuaJi(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPage_GuaJi();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_GUAJI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_chk_IsAutoBeatMonster;
	BOOL m_chk_IsAutoUseHpGoods;
	BOOL m_chk_IsAutoUseMpGoods;
	CString m_cmb_szHpGoodsName;
	CString m_cmb_szMpGoodsName;
	DWORD m_edt_dwHpPercent;
	DWORD m_edt_dwMpPercent;
	virtual BOOL OnInitDialog();
	BOOL m_chk_IsRangeLimit;
	DWORD m_edt_hitMonsterRange;
	int m_edt_LimitPosX;
	int m_edt_LimitPosY;
	afx_msg void OnBnClickedButton1();
	BOOL m_chk_IsUseSkill;
	BOOL m_chk_IsPickupGoods;
	CString m_cmb_szSkillName;
	BOOL m_chk_IsAutoUseQingGong;
	CString m_cmb_szQingGongName;
	DWORD m_edt_CheckHpMpSpeed;
	BOOL m_chk_IsDisableRefresh;
};
