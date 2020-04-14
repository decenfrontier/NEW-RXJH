// CPage_GoodsManage.cpp: 实现文件
//
#include "stdafx.h"
#include "pch.h"
#include "MFC_DLL.h"
#include "CPage_GoodsManage.h"
#include "afxdialogex.h"
#include "StructGame.h"


// CPage_GoodsManage 对话框

IMPLEMENT_DYNAMIC(CPage_GoodsManage, CDialogEx)

CPage_GoodsManage::CPage_GoodsManage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_GOODSMANAGE, pParent)
	, m_edt_GoodsName(_T(""))
{

}

CPage_GoodsManage::~CPage_GoodsManage()
{
}

void CPage_GoodsManage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstC_GoodsTable);
	DDX_Text(pDX, IDC_EDIT1, m_edt_GoodsName);
}


BEGIN_MESSAGE_MAP(CPage_GoodsManage, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON9, &CPage_GoodsManage::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage_GoodsManage::OnBnClickedButton1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CPage_GoodsManage::OnLvnItemchangedList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CPage_GoodsManage::OnNMClickList1)
END_MESSAGE_MAP()


// CPage_GoodsManage 消息处理程序


BOOL CPage_GoodsManage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//设置 列表控件 的风格
	DWORD dwStyle = ::GetWindowLong(m_lstC_GoodsTable.m_hWnd, GWL_STYLE);
	dwStyle |= LVS_REPORT;
	::SetWindowLong(m_lstC_GoodsTable.m_hWnd, GWL_STYLE, dwStyle);

	//设置 列表控件 的扩展风格
	DWORD dwExStyle = m_lstC_GoodsTable.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT;	//选中某行使整行高亮
	dwExStyle |= LVS_EX_GRIDLINES;		//网格线
	m_lstC_GoodsTable.SetExtendedStyle(dwExStyle);

	//表格插入列
	m_lstC_GoodsTable.InsertColumn(0, "物品名", LVCFMT_LEFT, 140, 11);
	m_lstC_GoodsTable.InsertColumn(1, "存仓库", LVCFMT_LEFT, 60, 0);
	m_lstC_GoodsTable.InsertColumn(2, "卖商店", LVCFMT_LEFT, 60, 0);
	m_lstC_GoodsTable.InsertColumn(3, "不拾取", LVCFMT_LEFT, 60, 1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//按钮事件: 返回 买卖 窗口
void CPage_GoodsManage::OnBnClickedButton9()
{
	ShowWindow(SW_HIDE);
}

//按钮事件: 添加
void CPage_GoodsManage::OnBnClickedButton1()
{
	UpdateData(TRUE);	//控件 -> 变量
	m_lstC_GoodsTable.InsertItem(m_lstC_GoodsTable.GetItemCount(), m_edt_GoodsName);
}

//列表控件事件: 选择改变
void CPage_GoodsManage::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// TODO: 在此添加额外代码
	
	*pResult = 0;
}

//列表控件事件: 鼠标单击
void CPage_GoodsManage::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int x, y;
	CPoint point;
	GetCursorPos(&point);
	m_lstC_GoodsTable.ScreenToClient(&point);
	
	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;
	int Item = m_lstC_GoodsTable.SubItemHitTest(&lvinfo);
	x = lvinfo.iSubItem;	//第x列
	y = lvinfo.iItem;		//第y行

	if (Item != -1 && x > 0)
	{
		//该项已经被勾选√, 就取消勾选
		char szText[10];
		m_lstC_GoodsTable.GetItemText(y, x, szText, sizeof(szText));
		if (strcmp(szText, "√") == 0)
			m_lstC_GoodsTable.SetItemText(y, x, "");
		else
			m_lstC_GoodsTable.SetItemText(y, x, "√");
		
		//如果 选中了 存仓库, 就取消勾选 卖商店
		if (x == 1)
		{
			m_lstC_GoodsTable.SetItemText(y, 2, "");
		}
		if (x == 2)
		{
			m_lstC_GoodsTable.SetItemText(y, 1, "");
		}
		DbgOutput("单击的是第%d行第%d列", lvinfo.iItem, lvinfo.iSubItem);
	}

	*pResult = 0;
}
