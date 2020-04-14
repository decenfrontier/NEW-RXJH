#pragma once


// CPage_GoodsManage 对话框

class CPage_GoodsManage : public CDialogEx
{
	DECLARE_DYNAMIC(CPage_GoodsManage)

public:
	CPage_GoodsManage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPage_GoodsManage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_GOODSMANAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_lstC_GoodsTable;
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton1();
	CString m_edt_GoodsName;
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
